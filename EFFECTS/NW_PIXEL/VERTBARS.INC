[segment codeseg]
[bits 32]

global _vertbars

_vertbars:

			xor ecx,ecx
			mov al,[counter]
			add al,2
			shr al,1
			mov bl,al
			neg al
			add al,12
			mov bh,al
			mov ebp,24*24
			add esi,ebp
			mov cl,24
	.vbl1:		push ecx
			mov cl,bl
			cmp cl,ch
			jz .vb1
			rep movsb
		.vb1:	;xor al,al
			;stosb
			;inc esi
			sub esi,ebp
			mov cl,bh
			sub cl,1
			cmp cl,ch
			jng .vb2
			rep movsw
		.vb2:	;xor al,al
			;stosb
			;inc esi
			add esi,ebp
			mov cl,bl
			cmp cl,ch
			jz .vb3
			rep movsb
		.vb3:	pop ecx
			loop .vbl1

			ret

[segment dataseg]