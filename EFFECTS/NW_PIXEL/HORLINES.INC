[segment codeseg]
[bits 32]

global _horlines

_horlines:

			push edi

			mov ecx,12
			mov bl,[counter]
			inc bl
			mov bh,24
			sub bh,bl
			add ebp,esi
	.hll1:		push ecx
			; odd line to the left
			movzx eax,bl
			add esi,eax	;skip bl from img1
			mov cl,bh
			or cl,cl
			jz .hl0
			rep movsb	;copy bh from img1
		.hl0:	xchg ebp,esi
			mov cl,bl
			or cl,cl
			jz .hl1
			rep movsb	;copy bl from img2
		.hl1:	movzx eax,bh
			add esi,eax	;skip bh from img2
			; even line to the right
			add esi,eax	;skip bh from img2
			mov cl,bl
			or cl,cl
			jz .hl2
			rep movsb	;copy bl from img2
		.hl2:	xchg esi,ebp
			mov cl,bh
			or cl,cl
			jz .hl3
			rep movsb	;copy bh from img1
		.hl3:	movzx eax,bl
			add esi,eax	;skip bl from img1
			pop ecx
			loop .hll1

			pop edi

			call _blur

			ret

_blur:

			mov ecx,24+1
			xor al,al
			rep stosb
			mov esi,edi
			mov ecx,22
			mov bl,[counter]
			inc bl
			mov bh,24
			sub bh,bl
			mov dl,24
	.bl1:		push ecx
			mov ecx,22
	.bl2:		mov al,[esi-1]
			add al,[esi+1]
			add al,[esi-24]
			add al,[esi+24]
			mov ah,[esi-25]
			add ah,[esi-23]
			add ah,[esi+23]
			add ah,[esi+25]
			shr ah,1
			shr al,1
			add al,ah
			shr al,2
			mul bh
			div dl
			mov dh,al
			lodsb
			mul bl
			div dl
			add al,dh
			stosb
			loop .bl2
			pop ecx
			add esi,byte 2
			xor eax,eax
			stosw
			loop .bl1

			ret

[segment dataseg]