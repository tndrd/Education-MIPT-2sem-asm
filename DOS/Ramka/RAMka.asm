.model tiny
.code
.186
org 100h

; --------------------parameters-------------------------
FRAME_X equ 20					;
FRAME_Y equ 5					; frame position and size
FRAME_W equ 40					;
FRAME_H equ 10					;
			
BGRD_CH equ 0b00h				; 
VBND_CH equ 0bBAh				;
HBND_CH equ 0bCDh				; frame chars
CRNR_TL equ 0bC9h				;
CRNR_TR equ 0bBBh				;
CRNR_BR equ 0bBCh				;
CRNR_BL equ 0bC8h				;

UPR_TXT equ "AS IT IS SAID: PHYSICS IS ", 0Bh, "300$", 0Bh	; upper text
LWR_TXT equ "(c) tndrd, 2020"								; lower text

STR_BGD equ 0bh					; string background

UTEXT_X equ 25					; upper text x
UTEXT_Y equ 9					; upper text y
DTEXT_X equ 33					; lower text x
DTEXT_Y equ 11 					; lower text y

FR_RATE equ 300					; frame rate in microseconds
BALL_CH equ 00b09h				; ball char
BALL_X  equ 10					; initial ball x
BALL_Y	equ 3					; initial ball y
BALL_D  equ D_BR				; initial ball direction

D_BR 	equ 11b 				; ball direction names
D_BL 	equ 10b					;
D_TR	equ 01b					;
D_TL	equ 00b					;

; -------------------------------------------------------


; -------------------adresation--------------------------
VLINE_W equ 160														; width of vmem in bytes

TL_ADDR equ 160 * FRAME_Y + 2 * FRAME_X								; address of top left corner of frame 
UTEXT_A equ 160 * UTEXT_Y + 2 * UTEXT_X								; address of upper text
DTEXT_A equ 160 * DTEXT_Y + 2 * DTEXT_X								; address of lower text

BALL_A 	equ 160 * (BALL_Y + FRAME_Y)  + 2 * (BALL_X + FRAME_X)		; initial address of the ball char
; -------------------------------------------------------


Start:	jmp	ParamBlockEnd

		UTEXT_S db UPR_TXT, 0
		DTEXT_S db LWR_TXT, 0

	ParamBlockEnd:

	mov 	bx, 0b800h			;	 
	mov		es, bx				; set es to vmem

	call	DrawFrame			; draw the frame
	
	mov 	di, UTEXT_A			;
	mov 	bx, offset UTEXT_S	; draw the upper text
	call	PutText				;

	mov 	di, DTEXT_A			;
	mov 	bx, offset DTEXT_S	; draw the lower text
	call	PutText				;

	call MoveBall				; start animation

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


MoveBall	proc

	;---------------------------
	; draws the ball animation
	; spoils ax, bx, cx, dx
	;---------------------------
	
	mov 	ax, BALL_X 			; init ball parameters
	mov 	cx, BALL_Y 			;
	mov 	dh, BALL_D 			; direction of the ball;
	
	mov 	dl, 0FFh   			; loop counter, is init with maximal byte value
	
	push 	es:[BALL_A]			; save the char that has been filled with the ball

	move_loop:

		cmp 	dl, 0			; while dl > 0
		je 		end_move		;

		call 	ManageCollision ; manage collision
		call	ComputeAddress	; bx = ball_address
								
		pop 	es:[bx]			; fill the ball char with the one that was before the ball has redrawn it
		call 	GetNextPosition	; bx = new_position
		push 	es:[bx]			; save the char that will be redrawn by the ball

		push 	ax				;
		mov 	ax, BALL_CH		; fill new_position
		mov 	es:[bx], ax		;
		
		push 	cx				;
		push 	dx				;
		mov 	ah, 86h			; wait
		mov 	cx, 0000h		;
		mov 	dx, 0F424h		;
		int 	15h				;

		pop 	dx				;
		pop 	cx				; restore registers
		pop 	ax				;

		dec 	dl				; dl--

	jmp 	move_loop

	end_move: 	pop es:[bx]		; pop useless value
				ret

MoveBall	endp


ComputeAddress	proc

	;---------------------------
	; computes the ball address from its coordinates
	; uses typical adresation formula
	; params:
	;	ax - ball x
	;	cx - ball y
	; saves ax, cx, dx
	; returns:
	;	bx - computed address
	;---------------------------

	push 	ax
	push 	cx
	push 	dx

	mov 	bx, ax
	add 	bx, FRAME_X
	add 	cx, FRAME_Y
	
	mov 	ax, VLINE_W / 2	
	mul 	cx				 
	add 	ax, bx			 
	mov 	bx, 2
	mul 	bx				
	mov 	bx, ax

	pop 	dx				
	pop 	cx				
	pop 	ax				

	ret

ComputeAddress	endp


GetNextPosition	proc

	;---------------------------
	; computes the next position depended on direction
	; params:
	;	ax - ball x
	;	cx - ball y
	;	dh - ball direction
	;	bx - current ball address
	; returns:
	;	ax - new x
	;	cx - new y	
	;	bx - new address
	;---------------------------

	check_dir_br:
		cmp		dh, D_BR
		jne		check_dir_bl

		inc 	ax
		inc 	cx

		add 	bx, VLINE_W
		inc 	bx
		inc 	bx

	check_dir_bl:
		cmp		dh, D_BL
		jne		check_dir_tr

		dec 	ax
		inc 	cx

		add 	bx, VLINE_W
		dec 	bx
		dec 	bx

	check_dir_tr:
		cmp 	dh, D_TR
		jne 	check_dir_tl

		dec 	cx
		inc 	ax

		sub 	bx, VLINE_W
		inc 	bx
		inc 	bx

	check_dir_tl:
		cmp 	dh, D_TL
		jne		end_check

		dec 	ax
		dec 	cx

		sub 	bx, VLINE_W
		dec 	bx
		dec 	bx

	end_check: 	ret

GetNextPosition	endp


ManageCollision	proc

	;---------------------------
	; changes the ball direction if it collides
	; params:
	;	ax - ball x
	;	cx - ball y
	;	dh - ball direction
	; returns:
	;	dh - new direction
	;---------------------------

	check_r_col:
		cmp 	ax, FRAME_W - 1
		jne		check_l_col

		add 	dh, 10b
		not 	dh

	check_l_col:
		cmp 	ax, 1
		jne		check_t_col

		add 	dh, 10b
		not 	dh

	check_t_col:
		cmp 	cx, 1
		jne		check_b_col

		add 	dh, 10b

	check_b_col:
		cmp 	cx, FRAME_H
		jne		finish_manage

		add dh, 10b

	finish_manage: 	and dh, 03h
					ret

ManageCollision	endp


PutText		proc

	;---------------------------
	; puts the text on a screen
	; params:
	;	di - destination address
	;	bx - source address
	; spoils di, dx
	;---------------------------

	mov 	si, 0						; si = 0	

	loop_start:	

		cmp 	byte ptr [bx][si], 0	; while (source[si] != 0)
		je 		endloop					;
		
		mov 	dh, STR_BGD				; background  = STR_BGD
		mov 	dl, [bx][si]			; char_number = source_char
		mov 	es:[di], dx 			; dest_char = src_char
		
		inc 	si						; si++
		inc 	di						; di+=2 due to its word size
		inc 	di						;
		
	jmp loop_start

	endloop: ret

PutText		endp


endprogram:     mov ax, 4c00h
                int 21h


end Start
