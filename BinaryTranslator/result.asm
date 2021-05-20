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

_start:jmp label_2

label_0:
push rbx
push qword [imm0]
fld qword [rsp]
add rsp, 1
fld qword [rsp]
add rsp, 1
fcompp
fstsw ax
sahf
je label_1
push rbx
push rcx
fld qword [rsp]
add rsp, 1
fld qword [rsp]
add rsp, 1
fmul
sub rsp, 1
fst qword [rsp]
pop rcx
push qword [imm1]
push rbx
fld qword [rsp]
add rsp, 1
fld qword [rsp]
add rsp, 1
fsubr
sub rsp, 1
fst qword [rsp]
pop rbx
call label_0

label_1:
ret

label_2:
call _stdlib_in_
pop rbx
push rbx
push qword [imm2]
fld qword [rsp]
add rsp, 1
fld qword [rsp]
add rsp, 1
fcompp
fstsw ax
sahf
ja label_3
push qword [imm3]
pop rcx
call label_0
push rcx
call _stdlib_out_
call _stdlib_hlt_

label_3:
push qword [imm4]
call _stdlib_out_
call _stdlib_hlt_
