bits 64

global _stdlib_hlt_

section .data

section .bss
	res: resq 1

section .text

_stdlib_hlt_:
    mov rdi, 0
    mov rax, 60
    syscall
