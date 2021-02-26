.model tiny
.code
.186
org 100h

; --------------------parameters-------------------------
FRAME_X equ 149					;
FRAME_Y equ -1					; frame position and size
FRAME_W equ 9					;
FRAME_H equ 12					;
TABLE_X equ 2					; X offset of registers table
TABLE_Y equ 1					; Y offset of registers table

DFLT_VA equ 0bh

;SetChar macro NAME, VALUE
;	NAME equ DFLT_VA%VALUE
;endm

;SetChar BGRD_CH, 00h

BGRD_CH equ 0b00h				; 
VBND_CH equ 0bBAh				;
HBND_CH equ 0bCDh				; frame chars
CRNR_TL equ 0bC9h				;
CRNR_TR equ 0bBBh				;
CRNR_BR equ 0bBCh				;
CRNR_BL equ 0bC8h				;
; -------------------------------------------------------

ACT_KEY equ 2Bh

; -------------------adresation--------------------------
VLINE_W equ 160							; width of vmem in bytes

TL_ADDR equ 160 * FRAME_Y + 2 * FRAME_X	; address of top left corner of frame
; -------------------------------------------------------

NDIGS_O equ 4

Start:	jmp	ParamBlockEnd

		DigitBuffer  db '0123456789ABCDEF'
        old09int 	 dd 0

	ParamBlockEnd:

	call SetNew09h			
	
jmp	endprogram


DrawFrame	proc

	call	FillBgrd		; fill the background

	mov 	di, TL_ADDR		; di points to top left corner address
	cld						; set the direction
	
	mov 	dx, CRNR_TL		; draw the corner
	mov 	es:[di], dx		; 
	inc		di				; move pointer to next char 
	inc		di				; 
	call	DrawHBound		; draw the top bound; now di points to top right corner
	
	mov 	dx, CRNR_TR		; draw the corner
	mov 	es:[di], dx		; 
	add		di, VLINE_W		; move pointer to the next char
	mov 	dx, VLINE_W		; make drawing direction down
	call	DrawVBound		; draw right bound; now di points to bottom right corner
	
	std						; switch the direction
	mov 	dx, CRNR_BR		; draw the corner		
	mov 	es:[di], dx		;
	dec		di				; move pointer to the next char
	dec		di				;
	call	DrawHBound		; draw bottom bound; now di points to bottom left corner
	
	mov 	dx, CRNR_BL		; draw the corner
	mov 	es:[di], dx		; 
	sub		di, VLINE_W		; move pointer to the next char
	mov 	dx, -VLINE_W	; make drawing direction up
	call	DrawVBound		; draw left bound; now di points to top left corner

	ret

DrawFrame	endp
		

DrawHBound	proc

	;---------------------------
	; draws the horizontal bound
	; spoils ax, cx
	; moves di to the next of last drawn char (stosw)
	;---------------------------

	mov		ax, HBND_CH
	mov		cx, FRAME_W
	rep		stosw
	ret

DrawHBound	endp 


DrawVBound	proc

	;---------------------------
	; draws the vertical bound
	; takes dx - the space between chars in bytes (should be +- VLINE_W)
	; spoils ax, cx
	; moves di to the next of last drawn char
	;---------------------------

	mov 	cx, FRAME_H		; cx = height
	mov 	ax, VBND_CH		; ax = desired_char (VBND_CH)

	VBound_loop:

		cmp 	cx, 0		; while cx > 0
		je 		QuitVBound	; 
		
		mov 	es:[di], ax	; putc(bound_char)
		add 	di, dx		; move di by one line (direction depends on dx param)
		dec 	cx 			; cx--

	jmp VBound_loop

	QuitVBound: ret

DrawVBound	endp


FillBgrd	proc

	;---------------------------
	; fills the frame background
	; spoils di, ax, bx, cx
	;---------------------------
		
	cld

	mov 	di, TL_ADDR			; set top left corner address
	mov 	ax, BGRD_CH			; set background fill char
	mov 	bx, FRAME_H + 1		; bx = frame_height + 1

	FillBgrd_loop:

		cmp 	bx, 0			; while bx > 0
		je 		QuitFillBgrd	; 
		
		push 	di				; save pointer to the current row start
		mov		cx, FRAME_W + 2	; line_length = frame_width + 2
		rep		stosw			; draw line (line_length); di now points to the end of current row
		pop 	di				; recover pointer to the current row

		add 	di, VLINE_W		; move pointer down one line
		dec 	bx 				; bx--

	jmp 	FillBgrd_loop

	QuitFillBgrd: ret

FillBgrd	endp

SetNew09h	proc

	;---------------------------
	; sets new 09h interrupt and makes the program resident
	; spoils bx, es, ax, dx
	;---------------------------
	
    xor 	bx, bx	  ;
	mov 	es, bx	  ; es = 0
	mov 	bx, 9 * 4 ; bx = addr of 09h in interrupt table

	cli

	mov 	ax, word ptr es:[bx]				; save old interrupt in old09int
	mov 	word ptr old09int, ax				; 
	mov 	ax, word ptr es:[bx + 2]			;
	mov 	word ptr old09int + 2, ax			;

	mov 	word ptr es:[bx], offset NewHandler	; rewrite 09h interrupt handler address
	mov 	ax, cs								; 
	mov 	word ptr es:[bx + 2], ax			;
	sti

	mov 	ax, 3100h							; make the program resident and quit
	mov 	dx, offset EndProgram				;
	shr 	dx, 4								;
	inc 	dx									;
	int 	21h									;

SetNew09h 	endp


NewHandler		proc

	;---------------------------
	; 09h handler that opens frame on screen with current registers values on it. Activates on ACT_KEY scancode
	; After execution calls old 09h interrupt
	;---------------------------

	pushf		; save flags
	push 	ax	; save ax
	
	in 		al, 60h			; call new interrupt if ACT_KEY is pressed
	cmp 	al, ACT_KEY		;
	pop 	ax				; 
	je  	HandlerActivate	;
	jmp 	cs:EndNew09h	;
	
	HandlerActivate:

	push 	cs ds es bp sp ss di si dx cx bx ax ; save registers

	mov 	bx, 0b800h			;	 
	mov		es, bx				; set es to vmem
	mov 	bp, sp
	call	DrawFrame			; draw the frame

	PrintChar	macro CHAR, OFFSET, N_LINE													; prints the char in desired position
																							; related to frame
		mov 	dh, DFLT_VA																	;
		mov 	dl, CHAR																	;
		mov 	word ptr es:[OFFSET + TABLE_X*2 + TL_ADDR + VLINE_W * (N_LINE+TABLE_Y)], dx	;
																							;
	endm																					;


	PrintReg	macro	CH1, CH2, N_LINE									; prints registers name and value from top of the stack on specified line of frame
																			; doesn't change stack
		PrintChar 	CH1, 0, N_LINE											;
		PrintChar 	CH2, 2, N_LINE											;
		PrintChar 	":", 4, N_LINE											;
		PrintChar 	" ", 6, N_LINE											;
																			;
		mov 	di, TABLE_X*2 + 8 + TL_ADDR + VLINE_W * (N_LINE+TABLE_Y)	;
		mov 	ax, ss:[bp]													;
		inc 	bp															;
		inc		bp															;
		call 	HexPrint													;

	endm

    PrintReg "a", "x", 0	; printing each register
	PrintReg "b", "x", 1	;
	PrintReg "c", "x", 2	;
	PrintReg "d", "x", 3	;
	PrintReg "s", "i", 4	;
	PrintReg "d", "i", 5	;
	PrintReg "s", "s", 6	;
	PrintReg "s", "p", 7	;
	PrintReg "b", "p", 8	;
	PrintReg "e", "s", 9	;
	PrintReg "d", "s", 10	;
	PrintReg "c", "s", 11	; 

	pop  	ax bx cx dx si di ss sp bp es ds	; restore registers
	
	inc 	sp	; remove cs from top of the stack
	inc 	sp	;

	EndNew09h:
    	popf							; restore flags
		jmp 	dword ptr cs:old09int	; call old interrupt

NewHandler 	endp


HexPrint    proc

	;---------------------------
	; prints the hex value on desired address
	; input:
	;	ax - value
	;	di - address
	;	es - segment
	; spoils:
	;	ax, bx, si, di
	;---------------------------

	mov 	si, NDIGS_O						; si counts the number of remained digits to be print
	mov 	bx, 0							;

	add		di, (NDIGS_O - 1) * 2			; make di point to last digit position (print will start from the end)

	ConvertLoop_start:	

	    ror 	ax, 4   					; 
	    mov 	bl, ah  					; bx = ax mod 16 
	    shr 	bl, 4   					;		
	
	    and 	ax, 0FFFh  					; ax = ax div 16

	    mov 	bl, cs:DigitBuffer[bx]		; print the char on the screes
		mov		byte ptr es:[di+1],	DFLT_VA	;
		mov		byte ptr es:[di], bl		;
		
		dec 	di							; make di point to the next digit position
		dec 	di							;
		dec 	si							; decrease counter
	
	    cmp 	si, 0						; do while si>0
		je 		ConvertLoop_end				;

	jmp ConvertLoop_start

    ConvertLoop_end:
		ret

HexPrint    endp


EndProgram:
end Start