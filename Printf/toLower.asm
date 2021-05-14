extern	toLower

section .data
    string	      db  "HoChUjRaT''!!", 10, 0 
    string_length equ $ - string

section .text
    global main
    
main:

    mov rsi, string
    mov rdx, string_length
    call StringOut
    
    push    rbp

    mov rdi, string
    call    toLower

    pop	    rbp

    mov rsi, string
    mov rdx, string_length
    call StringOut

    mov	    rax,60
    mov     rdi,0
    syscall

StringOut:

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
