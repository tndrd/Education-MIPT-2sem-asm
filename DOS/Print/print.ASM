.model tiny
.code
.186
org 100h

PrevBp equ 0FFFFh ; Constant for testing recovering old bp from stack 

start:	jmp ParamBlock_end
	
	msg 			db 	'CHECKING$$$PROGRAM', 0
	FormatString 	db 'print test: char is %c, string is %s, oct is %o, dec is %d, hex is %x and bin is %b ', 0
	
	DigitBuffer 	db '0123456789ABCDEF'
	PrintBuffer 	db 16 dup ('*')

	ParamBlock_end:

	push 	'!'				; value to test if print spoils stack
	mov  	bp, PrevBp		; pretending that bp is something

	call 	TestPrintCall	; calling print

	pop 	dx				; now dx should be '!'
	call 	CharOut			; print it out and see

	mov ax, bp				; checking if PrevBp is not changed
	call HexOut				; print it out and see

	mov 	ax, 4c00h
	int 	21h


TestPrintCall	proc

	;---------------------------
	; Makes test print call in cdecl style
	;---------------------------

	push 	offset PrintReturn	; pushing return address
	push 	bp					; pushing previous bp
	
	mov 	bp, sp 				; make bp point to first arg
	dec		bp					;
	dec		bp					;

	push 	0FFFFh				; pushing params in reversed order
	push 	3802				;
	push 	235					;
	push 	64					;
	push 	offset msg			;
	push 	'&'					;
	push 	offset FormatString	;
	
	jmp 	Print				; calling the print
	
	PrintReturn:

	mov 	bp, [bp+2]			; make bp = PrevBp
	add 	sp, 18				; clean the stack after call

	ret

TestPrintCall	endp


StringOut	proc

	;---------------------------
	; Puts the string in stdo
	; params:
	;	si - input string
	; spoils:
	;	ax, dx, si
	;---------------------------

	StringOut_loopstart:
		
		mov 	dl, byte ptr [si]
		inc 	si

		cmp 	dl, 0
		je 		StringOut_loopend

		mov 	ah, 02h
		int 	21h

	jmp StringOut_loopstart

	StringOut_loopend: ret

StringOut	endp


FlushOBuffer_rev	proc

	;---------------------------
	; Flushes the OutputBuffer into stdo in reverse order
	; params:
	;	si - number of bytes in OutputBuffer
	; spoils:
	;	ax, dx, si
	;---------------------------

	PrintLoop_Start:

		mov 	dl, PrintBuffer[si]
		call 	CharOut
		
		cmp 	si, 0
		je 		PrintLoop_end
		
		dec 	si

		jmp PrintLoop_Start
	
	PrintLoop_end: ret

FlushOBuffer_rev	endp


Deg2Print	macro PROC_NAME, BASE_DEGREE 

	; template of functions that print the number
	; in count system, which base is a degree of 2

PROC_NAME proc

	;---------------------------
	; Prints number in count system 2^BASE_DEGREE
	; params:
	;	ax - input number
	; spoils:
	;	ax, bx, dx, si
	;---------------------------

	mov 	si, 0	; digits counter
	mov		bx, 0	; trash clean
	mov		dx, 0	; trash clean

	ConvertLoop_start_&PROC_NAME:	

		ror 	ax, BASE_DEGREE				; 
		mov 	bl, ah  					; bx = ax mod 2^BaseDegree
		shr 	bl, 8 - BASE_DEGREE			;		
	
		and 	ax, 0FFFh  					; ax = ax div 2^BaseDegree

		mov 	bl, DigitBuffer[bx] 		; put the digit in OutputBuffer
		mov 	PrintBuffer[si], bl			;	
	
		cmp 	ax, 0						; do while ax > 0
		je 		ConvertLoop_end_&PROC_NAME	;
		
		inc 	si								

	jmp ConvertLoop_start_&PROC_NAME

	ConvertLoop_end_&PROC_NAME:	
		call FlushOBuffer_rev	; print the number
		ret

PROC_NAME endp

endm


Deg2Print HexOut, 4	; Create funcs from template
Deg2Print OctOut, 3	;
Deg2Print BinOut, 1	;


DecOut	proc

	;---------------------------
	; Prints number in count system 10
	; params:
	;	ax - input number
	; spoils:
	;	ax, bx, dx, si
	; Works like Deg2Print funcs
	;---------------------------

	mov 	si, 0
	mov 	bx, 0
	mov		dx, 0

	ConvertLoop_start_DecOut:	
	
		mov 	bl, 10	
		div 	bl     ; ah = ax mod 10, al = ax div 10
		mov 	bl, ah ; bl = mod	
		mov 	ah, 0  ; ax = div

		mov 	bl, DigitBuffer[bx]
		mov 	PrintBuffer[si], bl		
		
		cmp 	al, 0
		je 		ConvertLoop_end_DecOut

		inc 	si		

	jmp 	ConvertLoop_start_DecOut

	ConvertLoop_end_DecOut:
		call 	FlushOBuffer_rev
		ret

DecOut	endp


CharOut	proc

	;---------------------------
	; Prints the char
	; params:
	;	dl - char
	; spoils ax
	;---------------------------

	mov 	ah, 02h	
	int 	21h
	ret
CharOut	endp


Print	proc

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
	;	ax*, bx, dx,  di, si*
	; 	* - is being spoiled when certain format tags are used (see related calls)
	; Doesn't affect stack, calling side should clean it by itself
	;-------------------------------------------------------------------------------
	; Stack at start looks like:						addr -->
	;- - - - format - p_1 - p_2 - p_3 - ... - p_n - prev_bp - ret_addr - * - * - ...
	;			^							   ^
	;			sp							   bp
	;-------------------------------------------------------------------------------

	mov bx, sp		; bx = &first param
	mov di, [bx]	; di = format_string
	inc bx			; move bx to next param
	inc bx			;

	ReadFormat:

		CheckEnd:
			cmp byte ptr [di], 0 	; if (format[di] == 0) return
			jne CheckFormat		 	;
			jmp [bp+4]			 	; [bp+4] = ret_addr

		CheckFormat:
			cmp byte ptr [di], '%'	; if (format[di] == '%')
			jne PrintChar			;
			inc di					; skip '%'
			call ManageFormat		; print the value according to tag
			inc di					; skip tag char
			inc bx					; move bx to next param
			inc bx					;
			jmp ReadFormat			

		PrintChar:					;
			mov dl, byte ptr [di]	;
			call CharOut			; print format[di++]
			inc di					;
			jmp ReadFormat			;

Print	endp


ManageFormat	proc

	;---------------------------
	; Checks format tag and calls related printing func
	; params:
	;	di - pointer to tag char
	; spoils (depends on tag):
	; 	si, dl, ax
	;---------------------------


	Check_StringOut:
		cmp 	byte ptr [di], 's'
		jne		Check_CharOut
		mov 	si, [bx]
		call 	StringOut
		ret
	
	Check_CharOut:
		cmp 	byte ptr [di], 'c'
		jne 	CheckDec
		mov 	dl, [bx]
		call 	CharOut
		ret

	CheckNumber	macro TAG, PR_FUNC, NEXT_LABEL
	
	Check_&PR_FUNC:
		cmp		byte ptr [di], TAG
		jne		NEXT_LABEL
		push 	bx si
		mov 	ax, [bx]
		call	PR_FUNC
		pop 	si bx
		ret

	endm

	CheckNumber 'd', DecOut, Check_HexOut
	CheckNumber 'x', HexOut, Check_OctOut
	CheckNumber 'o', OctOut, Check_BinOut
	CheckNumber 'b', BinOut, EndCheck

	EndCheck: 	ret

ManageFormat	endp

end start