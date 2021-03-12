extern	printf

section .data
    love_string	   db "I love", 0 
    format         db "%s %x at %d %%111111", 10, 0

section .text
    global main
    
main:
    
    push    rbp

    mov rcx, 100
    mov rdx, 0EDAh
    mov rsi, love_string
    mov rdi, format

    mov	    rax,0
    call    printf

    pop	    rbp

    mov	    rax,60
    mov     rdi,0
    syscall