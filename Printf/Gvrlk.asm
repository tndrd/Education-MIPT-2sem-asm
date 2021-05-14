extern GvrlkSay

section .data
    love_string	   db "I love", 0 
    format         db "%s aaasdasdasd", 10, 0

section .text
    global main
    
main:
    
    push    rbp

    mov r8,  100
    mov rcx, 0EDAh
    mov rdx, love_string
    mov rsi, format
    mov rdi, 1

    mov	    rax,0
    call    GvrlkSay

    pop	    rbp

    mov	    rax,60
    mov     rdi,0
    syscall