global _start

section .data
	val: dq 423.139
	dot db "."
	mns db "-"
	zer db "0"

	pr equ 8
	precision dq 1000000

	bufsize equ 16

	something dq 64 dup 0
	bindec dq 64 dup 0
	obuf   dq bufsize*4 dup 0

	integer dq 64 dup 0
	decimal dq 64 dup 0

	smth2  dq 64 dup 0

section .bss
	res: resq 1

section .text

_start:

	call _stdlib_out_

	exit:
    mov rdi, 0
    mov rax, 60
    syscall

_stdlib_out_:
    fst qword [val]
    call PrintFloat64
    ret

PrintFloat64:
	
	mov r10, qword [val]
	shr r10, 63
	cmp r10, 0
	je SkipMinus

	mov rsi, mns
	mov rdx, 1
	call PutToStdout

	mov r10, qword[val]

	shl r10, 1
	shr r10, 1

	mov qword[val], r10

	SkipMinus:

	fld   qword [val]
	fbstp [bindec]

	fld   qword [val]
	fistp dword [integer]

	cmp dword [integer], 0
	je SkipZero_int


	mov r13, 1
	call ExploreBinDec
	jmp Decimal

	SkipZero_int:

	mov rsi, zer
	mov rdx, 1
	call PutToStdout
	
	Decimal:

	fld  qword [val]
	fild qword [integer]
	fsub

	fild qword [precision]
	fmul
	fbstp [bindec]

	cmp qword [bindec], 0
	je NoDecimal

	mov rsi, dot
	mov rdx, 1
	call PutToStdout


	mov r13, 0
	call ExploreBinDec


	NoDecimal:
	ret



ExploreBinDec:

	xor rbx, rbx
	mov rdx, bufsize
	xor r12, r12

	cmp r13, 0
	je no_right_zeros

	zeros_loop:
		
		cmp byte [bindec + rdx - 1], 0
		jne start_print
		sub rdx, 1
		jmp zeros_loop

	no_right_zeros:
	sub rdx, bufsize - pr + 5
	jmp here

	start_print:
	
	mov r10, rdx

	mov al, byte [bindec + rdx - 1]
	and al, 0F0h
	cmp al, 0
	jne expl_loop 

	mov bl, byte [bindec + rdx - 1]
	add bl, "0"
	mov byte [obuf], bl

	mov rdx, 1
	mov rsi, obuf
	call PutToStdout

	sub r10, 1

	jmp skip_this_shit
	
	here:
	mov r10, rdx

	skip_this_shit:

	expl_loop:
		
		mov rdx, r10

		mov bl, byte [bindec + rdx - 1]
		sub r10, 1

		mov r12, rbx
		shr r12, 4

		mov [obuf], r12
		add byte [obuf], '0'

		mov rsi, obuf
		mov rdx, 1
		call PutToStdout

		mov r12, rbx
		and r12, 0Fh

		mov [obuf], r12
		add byte [obuf], '0'

		mov rsi, obuf
		mov rdx, 1
		call PutToStdout

		cmp r10, 0
		je exit_expl

		jmp expl_loop

	exit_expl: 

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