; kukac effect for project ledmatrix, 2004.05.02
%include "../../segments.inc"
%include "../../macros.inc"
[segment codeseg]
[bits 32]

globaleffect	kukac
extern	matrix
extern	_random,_randomize

_kukac_initialize:

			call _randomize
			call _restartgame

			ret

_kukac_destroy:

			ret

_kukac_step:

			mov al,[cnt2]
			inc al
			cmp al,cnt2max
			jc .nothin

			mov ebx,[headpos]
			mov dx,[pixels+2*ebx]
			dec ebx
			and ebx,ml_mask
			mov [headpos],ebx
			add dl,[direction]
			add dh,[direction+1]
			mov [pixels+2*ebx],dx
			cmp dx,[foodpos]
			jnz .nofood
			;- eat food
			call _placefood
			mov eax,[length]
			inc eax
			cmp eax,maxlen
			jc .lok
			call _restartgame
			jmp .restarted
		.lok:	mov [length],eax
	.nofood:
			call _finddirection
	.restarted:
			xor al,al
	.nothin:	mov [cnt2],al

			ret

_kukac_render:

			mov edi,[matrix]
			xor eax,eax
			mov ecx,24*24/4
			rep stosd

			mov ecx,[length]
			mov edx,[headpos]
			mov esi,pixels
			mov edi,[matrix]
	.krl1:		movzx eax,byte [esi+2*edx+1]
			imul eax,byte 24
			movzx ebx,byte [esi+2*edx]
			add eax,ebx
			mov byte [edi+eax],kukaccolor
			inc edx
			and edx,ml_mask
			loop .krl1

			movzx eax,byte [foodpos+1]
			imul eax,byte 24
			movzx ebx,byte [foodpos]
			add eax,ebx
			mov byte [edi+eax],foodcolor

			ret

;---------------------------------------------------
_restartgame:

			;--- restart kukac
			mov byte [length],initiallen
			mov byte [headpos],0
			mov edi,pixels
			mov al,12
			mov ah,al
			stosw
			mov ecx,initiallen-1
	.rkl1:		inc ah			;tail hangs down
			stosw
			loop .rkl1

			call _placefood

			call _finddirection

			ret

_placefood:

	.pfagain:	mov eax,24
			call _random
			mov [foodpos],al
			mov eax,24
			call _random
			mov [foodpos+1],al
			mov esi,foodpos
			call _check_collision
			jc .pfagain

			ret

; in:	esi	- points to a word containing coordinates to check
; out:	carry flag set on collision
_check_collision:
			pushad

			cmp byte [esi],0
			jnge .coll
			cmp byte [esi],23
			jg .coll
			cmp byte [esi+1],0
			jnge .coll
			cmp byte [esi+1],23
			jg .coll

			mov edi,pixels
			mov eax,[headpos]
			mov bx,[esi]
			mov ecx,[length]
	.ccl1:		cmp bx,[edi+2*eax]
			jz .coll
			inc eax
			and eax,ml_mask
			loop .ccl1

			popad
			clc
			ret

	.coll:		popad
			stc
			ret

; finds the direction to go
;	wants to reach food first vertically then horizontally,
;	chooses another possible direction if blocked,
;	dies if blocked in all directions
_finddirection:

			xor bx,bx
			mov edx,[headpos]
			mov dx,[pixels+2*edx]
			cmp dh,[foodpos+1]
			jc .felette
			ja .alatta
			;- the head is in the same raster as the food
			cmp dl,[foodpos]
			jc .jobbra
			;- headpos cannot be equal to foodpos
			;- balra
			dec bl
			jmp .fdcheck
	.felette:	inc bh
			jmp .fdcheck
	.alatta:	dec bh
			jmp .fdcheck
	.jobbra:	inc bl
			jmp .fdcheck

	.fdcheck:
			mov esi,temppos
			mov [esi],dx
			add [esi],bl
			add [esi+1],bh
			call _check_collision
			jnc .fdok
			;- direction not free, find another
			mov ebp,directions
			mov ecx,4
	.fdl:		mov [esi],dx
			mov bx,[ebp]
			add [esi],bl
			add [esi+1],bh
			call _check_collision
			jnc .fdok
			add ebp,byte 2
			loop .fdl
			;- died
			call _restartgame

	.fdok:
			mov [direction],bx
			ret

[segment dataseg]
maxlen		equ	64
ml_mask		equ	63
kukaccolor	equ	32
foodcolor	equ	63
initiallen	equ	4
length		dd	initiallen
pixels		times	2*maxlen db(0)
headpos		dd	0
directions	db	0,-1,1,0,0,1,-1,0	;up,right,down,left
foodpos		dd	0
temppos		dd	0
direction	dd	0
cnt2		dd	0
cnt2max		equ	1