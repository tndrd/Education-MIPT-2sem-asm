global _start
extern _stdlib_in_

section .data
	val: dq 0

	newline db 10

	in_buffer dq 64 dup 0
	in_char db 0

section .bss
	res: resq 1

section .text

_start:

	call _stdlib_in_

	exit:
    mov rdi, 0
    mov rax, 60
    syscall
	