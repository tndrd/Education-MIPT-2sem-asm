bits 64

global _start

extern _stdlib_in_
extern _stdlib_out_
extern _stdlib_hlt_

section .data
	value_buffer dq 0
    imm4 dq 1.000000
    imm3 dq 1.000000
    imm2 dq 1.000000
    imm1 dq 1.000000
    imm0 dq 1.000000

section .bss
section .text

_start:

push qword [rax + value_buffer]
