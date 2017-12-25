%include "..\..\segments.inc"
[segment codeseg]
[bits 32]

global	_randomize, _random

_randomize:

		MOV	word [kezdorand],32912
		MOV	word [kezdorand+2],42193
		ret

_random:
; In  :  AX
; OUT :  AX
		pushad
		MOV	cx,ax
		MOV	ax,word [kezdorand+2]
		MOV	bx,word	[kezdorand]
		MOV	si,ax
		MOV	di,bx
		MOV	dl,ah
		MOV	ah,al
		MOV	al,bh
		MOV	bh,bl
		XOR	bl,bl
		RCR	dl,1
		RCR	ax,1
		RCR	bx,1
		ADD	bx,di
		ADC	ax,si
		ADD	bx,1276h
		ADC	ax,5643h
		MOV	word [kezdorand],bx
		MOV	word [kezdorand+2],ax
		XOR	dx,dx
		DIV	cx
		MOV	[randomertek],dx
		POPAD
		movzx eax,word [randomertek]
		ret

[segment dataseg]
global	kezdorand
randomertek	dw	0
kezdorand	dd	0