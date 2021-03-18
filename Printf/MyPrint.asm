bits 64
section .text
	GLOBAL MyPrint 

MyPrint:

	;---------------------------
	; Works like printf func from C
	; Format tags:
	;	%s - string
	;	%c - char
	;	%d - decimal	 number
	;	%h - hexadecimal number
	;	%o - octodecimal number
	; 	%b - binary		 number
	; params:
	;	func takes params in cdecl style (meant to be pushed in reverse order)
	;	first param	- format string
	;   next params - values to be print (format specified by tags in format string)
	; spoils:
	;	rax*, rbx, rdx,  rdi, rsi*
	; 	* - is being spoiled when certain format tags are used (see related calls)
	; Doesn't affect stack, calling side should clean it by itself
	;-------------------------------------------------------------------------------
	; Stack at start looks like:						addr -->
	;- - - - format - p_1 - p_2 - p_3 - ... - p_n - prev_bp - ret_addr - * - * - ...
	;			^							   ^
	;			sp							   bp
	;-------------------------------------------------------------------------------

	pop r14		; 
				;
    push r9		;
    push r8		; make System V call look like cdecl
    push rcx	;
    push rdx	;
    push rsi	;
    push rdi	;

	mov 	rbx, rsp		; rbx = &first param
	mov 	rcx, [rbx]		; rcx = format_string
	add 	rbx, 8			; move bx to next param

	ReadFormat:

		CheckEnd:
			cmp byte [rcx], 0	 	; if (format[rcx] == 0) return
			jne CheckFormat		 	;
			add rsp, 48				; clear stack	
			push r14				; restore return adress
			ret						; return

		CheckFormat:
			cmp byte [rcx], '%'		; if (format[rcx] == '%')
			jne PrintChar			;
			inc rcx					; skip '%'
			call ManageFormat		; print the value according to tag
			inc rcx					; skip tag char
			;add rbx, 8				; move bx to next param
			jmp ReadFormat			

		PrintChar:					;
			mov rsi, rcx			;
			call CharOut			; print format[di++]
			inc rcx					;
			jmp ReadFormat			;



%macro	Deg2Print	2  
	
	; 1 - proc name, 2 - base degree
	; template of functions that print the number
	; in count system, which base is a degree of 2

    %1:

	;---------------------------
	; Prints number in count system 2^BASE_DEGREE
	; params:
	;	rax - input number
	; spoils:
	;	rax, rbx, rdx, rsi, rdi
	;---------------------------

	mov 	rsi, 0	; digits counter
	mov		rbx, 0	; trash clean
	mov		rdx, 0	; trash clean

	ConvertLoop_start_%1:	

        mov 	rbx, rax
        
        and     rbx, ~((~0) << %2)          ; rbx = mod
		shr 	rax, %2		                ; rax = div

		mov 	dl,  byte DigitBuffer[rbx] 		; put the digit in OutputBuffer
		mov 	byte  OutputBuffer[rsi], dl		;	
	
		cmp 	rax, 0						; do while ax > 0
		je 		ConvertLoop_end_%1	        ;
		
		inc 	rsi								

	jmp ConvertLoop_start_%1

	ConvertLoop_end_%1:	
		call FlushOBuffer_rev	; print the number
		ret

    %endmacro

Deg2Print HexOut, 4
Deg2Print OctOut, 3
Deg2Print BinOut, 1


ManageFormat:

	;---------------------------
	; Checks format tag and calls related printing func
	; params:
	;	rcx - pointer to tag char
	; spoils (depends on tag):
	; 	si, dl, ax
	;---------------------------
	
	xor rax, rax
	mov al, byte [rcx]
	xor rsi, rsi
	mov rsi, [FmtTagJumpTable + rax*8]

	call rsi
	ret

	Do_StringOut:
		mov 	rsi, [rbx]
		call 	StringOut
		add rbx, 8				; move bx to next param
		ret
	
	Do_CharOut:
		mov 	rsi, rbx
		call 	CharOut
		add rbx, 8				; move bx to next param
		ret

	%macro CheckNumber 1 ;PR_FUNC
	
	Do_%1:
		push 	rbx
		push	rcx
		
		mov 	rax, [rbx]
		call	%1
		
		pop 	rcx
		pop		rbx
		add rbx, 8				; move bx to next param
		ret

	%endmacro

	CheckNumber DecOut
	CheckNumber HexOut
	CheckNumber OctOut
	CheckNumber BinOut


DecOut:

	;---------------------------
	; Prints number in count system 10
	; params:
	;	rax - input number
	; spoils:
	;	ax, bx, dx, si
	; Works like Deg2Print funcs
	;---------------------------

	mov 	rsi, 0
	mov 	rbx, 0
    mov 	rcx, 10

	ConvertLoop_start_DecOut:	

		mov 	rdx, 0

        div     rcx     ; rax = div
                        ; rdx = mod
		
        mov rbx, DigitBuffer[rdx]  ; move digit to output buffer
        mov OutputBuffer[rsi], rbx ;

		cmp 	rax, 0
		je 		ConvertLoop_end_DecOut

		inc 	rsi		

	jmp 	ConvertLoop_start_DecOut

	ConvertLoop_end_DecOut:
		call 	FlushOBuffer_rev
		ret


PutToStdout:

    ;---------------
    ; Does 01h syscall (write) 
    ; Params:
    ;   rsi - pointer to buffer
	;	rdx - buffer size
    ; Spoils:
    ;   rax, rdi, rdx, goddamn rcx (!) 
    ;---------------

	mov 	rax, 1
    mov 	rdi, 1
    syscall
	ret

TagError:
	mov 	rsi, FmtTagErrMsg
	mov 	dl, byte [rcx]
    mov 	FmtTagErrMsg[FmtTagErrMsg_len - 3], dl
	mov		dx, FmtTagErrMsg_len
	call 	PutToStdout
	add rbx, 8				; move bx to next param
	ret
	
PercentOut:
	mov		rsi, "%"
	mov 	OutputBuffer[0], rsi
	mov 	rsi, OutputBuffer
	call 	CharOut
	ret


CharOut:

    ;---------------
    ; Prints out a char
    ; Params:
    ;   rsi - pointer to char
    ; Spoils:
    ;   rax, rdi, rdx, goddamn rcx (!) 
    ;---------------

	push rcx

    mov rax, 1
    mov rdi, 1
    mov rdx, 1
    syscall

	pop rcx

    ret

StringOut:
    ;---------------
    ; Prints out a char
    ; Params:
    ;   rsi - pointer to register
    ; Spoils:
    ;   rax, rdi, rdx, rsi 
    ;---------------

    cmp byte [rsi], 0
    je StringOut_end

    call CharOut
    inc rsi

    jmp StringOut

    StringOut_end: ret


FlushOBuffer_rev:

	;---------------------------
	; Flushes the OutputBuffer into stdout in reverse order
	; params:
	;	rsi - number of bytes in OutputBuffer
	; spoils:
	;	rax, rdx, rsi
	;---------------------------

    lea rsi, [OutputBuffer + rsi]

	PrintLoop_Start:
	
		call 	CharOut
		
		cmp 	rsi, OutputBuffer
		je 		PrintLoop_end
		
		dec 	rsi
	
	jmp PrintLoop_Start
	
	PrintLoop_end: ret


section .data
	
	msg 			db 	'CHECKING$$$PROGRAM', 0
	FormatString 	db 'print test: char is %c, string is %s, oct is %o, dec is %d, hex is %g and bin is %b ', 0

    OutputBuffer    db 64 dup "*"
    DigitBuffer     db '0123456789ABCDEF'

	FmtTagErrMsg	 db "Unknown format tag: a", 0ah, 0
	FmtTagErrMsg_len equ $ - FmtTagErrMsg


    FmtTagJumpTable     times ('%')             dq TagError
                                                dq PercentOut
                        times ('a' - '%')       dq TagError                    
                                                dq Do_BinOut
                                                dq Do_CharOut
                                                dq Do_DecOut
                        times ('n' - 'd')       dq TagError
                                                dq Do_OctOut
                        times ('r' - 'o')       dq TagError
                                                dq Do_StringOut
                        times ('w' - 's')       dq TagError
												dq Do_HexOut   
						times (255 - 'h')		dq TagError

section .bss