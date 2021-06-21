bits 64

global _start

extern _stdlib_in_
extern _stdlib_out_
extern _stdlib_hlt_

section .data
	value_buffer dq 0
l=Ñ    imm4 dq 1.000000
    imm3 dq 1.000000
    imm2 dq 1.000000
    imm1 dq 1.000000
    imm0 dq 1.000000

section .bss
section .text

_start:jmp label_2

label_0:
push r10
push qword [imm0]
fld qword [rsp]
add rsp, 8
fld qword [rsp]
add rsp, 8
fcompp
fstsw ax
sahf
je label_1
push r10
push r11
fld qword [rsp]
add rsp, 8
fld qword [rsp]
add rsp, 8
fmul
sub rsp, 8
fstp qword [rsp]
pop r11
push qword [imm1]
push r10
fld qword [rsp]
add rsp, 8
fld qword [rsp]
add rsp, 8
fsub
sub rsp, 8
fstp qword [rsp]
pop r10
call label_0

label_1:
ret

label_2:
call _stdlib_in_
pop r10
push r10
push qword [imm2]
fld qword [rsp]
add rsp, 8
fld qword [rsp]
add rsp, 8
fcompp
fstsw ax
sahf
jb label_3
push qword [imm3]
pop r11
call label_0
push r11
call _stdlib_out_
call _stdlib_hlt_

label_3:
push qword [imm4]
call _stdlib_out_
call _stdlib_hlt_
