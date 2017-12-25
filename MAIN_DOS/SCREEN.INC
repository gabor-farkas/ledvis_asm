;*******************************************************************************
;  SCREEN.INC by HHC/cmds 2000.III.1. Copyright Farkas G bor 2000, Hungary
;
;	_putscreen
;	_clearscreen
;	_initscreen
;	_initmcga
;
;	_restoretext
;
;	for ledmatrix project version


global screenbuf,matrix,_putscreen

[segment code32]
screenbuf	dd	0
matrix		dd	matrixf
matrixf		times	24*24 db(0)
[segment code32]
[bits 32]

;*******************************************************************************
_putscreen:

			mov ecx,64000/4
			mov esi,[screenbuf]
			@rlp edi,0A0000h
			cld
			rep movsd

			ret

_clearscreen:

			mov ecx,64000/4
			mov edi,[screenbuf]
			xor eax,eax
			cld
			rep stosd

			ret

_initscreen:

			mov eax,65536
			call _allocmem
			mov [screenbuf],eax

			ret

_initmcga:

			push es
			mov word [v86r_ax],13h
			mov ax,10h
			int 33h
			pop es

			ret

_restoretext:

			push es
			mov word [v86r_ax],3
			mov ax,10h
			int 33h
			pop es

			ret