global _stdlib_in_

section .data
	val:      dq 0
	newline   db 10
	in_buffer dq 64 dup 0
	in_char   db 0

section .bss
	res: resq 1

section .text

_stdlib_in_:

	call FillInBuffer

	cmp byte [in_buffer], "-"
	jne Positive

	Negative:
		add  rcx, 1
		push 1
		mov byte [in_buffer], "0"
		jmp  StartParsing

	Positive:
		push 0
		jmp  StartParsing

	StartParsing: call ParseDouble

	pop rdx
	cmp rdx, 1
	jne Return

	fchs

    Return: ret

    EmptyInputHandler:
        pop rbx
        mov  qword  [val], 0
        fild qword [val]

    ret

FillInBuffer:

	; reads from stdin and returns number of read chars in rbx

	xor rbx, rbx

	read_loop:

		mov rax, 0
		mov rdi, 0
		mov rsi, in_char
		mov rdx, 1
		syscall

		mov dl, byte [in_char] 

		cmp dl, 10
		je end_reading

		cmp dl, 4
		je end_reading

		mov [in_buffer + rbx], dl
		add rbx, 1

		jmp read_loop
	
    end_reading:
    
        cmp rbx, 0
        je EmptyInputHandler
    
    ret

ParseDouble:

	; takes rbx - number of chars in in_buffer

	xor rax, rax ; would be value where we store our number's integer and decimal parts

	xor rcx, rcx

	DotSearchLoop:

		cmp rcx, rbx
		je ParseIntegerEntry

		cmp byte [in_buffer + rcx], "."
		je ParseIntegerEntry

		add rcx, 1

		jmp DotSearchLoop

	ParseIntegerEntry: call ParseInteger

	mov [val], r14
	fild qword [val]

    cmp rbx, rcx
    je end

	add rcx, 1
	sub rbx, 1

	call ParseDecimal

	mov [val], r14
	fild qword [val]


	mov rax, 1
	mov r12, rbx
	sub r12, rcx
	add r12, 1
	call Pow10

	mov [val], rax
	fild qword[val]

	fdiv
	fadd

	end: ret

ParseInteger:

		; parses number from in_buffer, starting from rcx going to zero
		; rcx - dot index in buffer
		; return in r14

		xor r14, r14
		xor rax, rax

		mov rsi, rcx
		sub rsi, 1
		xor r12, r12

		NumberLoop:
			
			xor rax, rax

			mov al, byte [in_buffer + rsi] ; al = digit
			sub al, "0"					   ; r12 = degree

			;now we need to pow(al, r12)
			call Pow10

			add r14, rax
			add r12, 1

			cmp rsi, 0
			je end_parsing

			sub rsi, 1
		
		jmp NumberLoop

end_parsing: ret


ParseDecimal:

		; parses number from in_buffer, starting from rbx going to rcx
		; rcx - dot index in in_buffer
		; rbx - end of number index in in_buffer
		; return in r14

		xor r14, r14
		xor rax, rax

		mov rsi, rbx
		xor r12, r12

		NumberLoop_dec:
			
			xor rax, rax

			mov al, byte [in_buffer + rsi] ; al = digit
			sub al, "0"					; r12 = degree

			;now we need to pow(al, r12)
			call Pow10

			add r14, rax
			add r12, 1

			cmp rsi, rcx
			je end_parsing_dec

			sub rsi, 1
		
		jmp NumberLoop_dec

end_parsing_dec: ret

Pow10:
	; al - number
	; r12 - degree
	; return in rax

	mov r13, r12
	
	pow_loop:

		cmp r13, 0
		je end_pow

		mov r15, 10
		mul r15
		sub r13, 1

	jmp pow_loop

end_pow: ret
