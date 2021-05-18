global _start
extern _stdlib_in_
extern _stdlib_out_

section .data
	startmsg db "Enter a decimal number:", 10
	startlen equ $ - startmsg

	inmsg db "In worked!", 10
	inlen equ $ - inmsg

	outmsg db "Out worked!", 10
	outlen equ $ - outmsg

section .bss
	res: resq 1

section .text

_start:

	mov rsi, startmsg
	mov rdx, startlen
	call PutToStdout

	call _stdlib_in_
	
	mov rsi, inmsg
	mov rdx, inlen
	call PutToStdout
	
	call _stdlib_out_

	mov rsi, outmsg
	mov rdx, outlen
	call PutToStdout

	exit:
    mov rdi, 0
    mov rax, 60
    syscall
	

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