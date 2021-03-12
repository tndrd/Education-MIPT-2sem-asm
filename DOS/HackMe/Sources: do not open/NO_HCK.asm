.model tiny
.code
.186
org 100h

SALT                equ 100h - "$" ; salt value for cipher
EMPTY_CIPHER_CHAR   equ "*"        ; empty char for buffers
MAX_PASSWORD_LENGTH equ 16         ; 

start:

    mov     si, offset StartMsg
    call    DisplayMessage

    call    ProcessPassword    


Exit    proc

    mov     ax, 4c00h
    int     21h
    ret

Exit    endp


ProcessPassword proc

    call    GetPassword
    call    Cipher
    call    CompareWithCipher
    call    Exit

ProcessPassword endp

Retry   proc

    ;--------------
    ; Restarts the program
    ;--------------

    call CleanBuffers

    mov si, offset RestartMsg
    call DisplayMessage

    call ProcessPassword
    call Exit

Retry   endp

CleanBuffers    proc

    ;--------------
    ; Cleans buffers before restart
    ; Params:  -
    ; Destroy: al, di, cx
    ;-------------- 

    cld
    mov al, EMPTY_CIPHER_CHAR
    mov di, offset InputBuffer
    mov cx, MAX_PASSWORD_LENGTH
    rep stosb

    mov InputBuffer[di], SALT

    cld
    mov al, EMPTY_CIPHER_CHAR
    mov di, offset CipherBuffer
    mov cx, MAX_PASSWORD_LENGTH
    rep stosb

    ret

CleanBuffers    endp

DisplayMessage proc
    
    ;--------------
    ; Displays a message ending with "$"
    ; Params:  si - string offset
    ; Destroy: dx, ah, si
    ;-------------- 

    mov     ah, 09h
    mov     dx, si
    int     21h
    ret

DisplayMessage endp

GetPassword proc

    ;--------------
    ; Reads a password from stdin to the buffer
    ; Params:  -
    ; Destroy: bx, ax
    ;--------------

    mov     bx, 0

    input_loop:

        mov     ah, 01h
        int     21h

        cmp     al, 0dh
        je      end_input         

        mov     InputBuffer[bx], al

        ;cmp     bx, MAX_PASSWORD_LENGTH + 1
        ;je      ManageMaxLength

        inc     bx

    jmp input_loop

        end_input:  cmp bx, 0
                    je exit
                    ret

GetPassword endp

    
;ManageMaxLength:                        ;
;    mov     si, offset Buf_oflow_Msg    ; stops the input if password is too long
;    call    DisplayMessage              ;
;    call    Retry                       ;

InBufSalt       db 23
InputBuffer     db MAX_PASSWORD_LENGTH dup (EMPTY_CIPHER_CHAR), SALT
CipherBuffer    db MAX_PASSWORD_LENGTH dup (EMPTY_CIPHER_CHAR)

Cipher    proc

    ;--------------
    ; Ciphers the password from buffer
    ; Params:  -
    ; Destroy: si, cx, ah
    ; Return: CipherBuffer
    ; Works with formula:
    ;   h[n] = (h[n - 1] + SALT) >> (b[n - 1] - h[n - 1]) - (h[n - 1] + SALT) + b[n],
    ;   where h = CipherBuffer,
    ;         b = InputBuffer 
    ;--------------

    mov     si, 0

    cipher_loop:
        
        cmp     si, MAX_PASSWORD_LENGTH
        je      end_cipher

        mov     ch, CipherBuffer[si - 1]
        mov     cl, InputBuffer[si - 1] 
        sub     cl, ch
        add     ch, SALT
        mov     ah, ch
        ror     ch, cl
        sub     ch, ah
        add     ch, InputBuffer[si]

        mov     CipherBuffer[si], ch
        inc     si

    jmp cipher_loop

    end_cipher: ret

Cipher    endp


CompareWithCipher proc

    ;--------------
    ; Compares input password cipher with original password cipher.
    ; Finishes program if password is correct, or restarts it if password is wrong
    ; Params:  -
    ; Destroy: si, ax, dx
    ; Return: -
    ;--------------

    mov     si, 0
    
    compare_loop:
        
        cmp     si, MAX_PASSWORD_LENGTH
        je      PasswordOK

        mov     al, CipherBuffer[si]
        cmp     al, PWordCipher[si]
        jne     PasswordBad

        inc     si
    jmp compare_loop
    
    PasswordOK:
        mov     si, offset PW_OK_Msg
        call    DisplayMessage
        call    Exit

    PasswordBad:
        mov     si, offset PW_Bad_Msg
        call    DisplayMessage
        call    Retry

CompareWithCipher endp



StartMsg        db "Hello, stranger. What's a password? (Maximum 16 symbols). Empty string - exit.", 0Ah,0Ah, "Say it: ", "$"
RestartMsg      db "Try again. May the luck be with you. Empty string - exit", 0Ah, 0Ah, "So, it is: ", "$"
PW_Bad_Msg      db "You look suspicious. Access denied.", 0Ah, "$"
PW_OK_Msg       db "Self-destruct begin successfully. The computer will blow up in 10 seconds", 0Ah, "$"
Buf_oflow_Msg   db 0Ah, "You've entered too much symbols, password should be shorter", 0Ah, "$"

PWordCipher     db 160, 226, 129, 152, "Q &*********", 0ah, "$"

end start