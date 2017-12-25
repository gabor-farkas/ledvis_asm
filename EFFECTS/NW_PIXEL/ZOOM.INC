[segment codeseg]
[bits 32]

global	_nwpzoom

_nwpzoom:

			movzx eax,byte [counter2]
			mov bl,24
			div bl
			mov al,255
			mul ah
			mov bl,24
			div bl
			not al
			mov [z_t],al
			movzx ebx,byte [counter2]
			cmp bl,24
			jc .nz1
			mov bl,24
;			sub bl,24
;			add esi,ebp
	.nz1:
			mov eax,256*256
			xor edx,edx
			shl ebx,6
			add ebx,256
			div ebx
			mov [z_step],eax
			mov ebx,12
			mul ebx
			mov ebx,12*256
			sub ebx,eax
			mov [z_i],ebx
			mov [z_y],ebx
			mov ecx,24
	.l1:		push ecx
			mov eax,[z_i]
			mov [z_x],eax
			mov al,24
			mul byte [z_y+1]
			movzx ebp,ax
			mov cl,24
			mov edx,[z_step]
			add esi,ebp
	.l2:		movzx ebx,byte [z_x+1]
			mov al,[esi+ebx]
			mul byte [z_t]
			mov al,ah
			and al,63
			stosb
			add [z_x],edx
			loop .l2
			sub esi,ebp
			add [z_y],edx
			pop ecx
			loop .l1

			ret

[segment dataseg]
z_step			dd	0
z_x			dd	0
z_y			dd	0
z_i			dd	0
z_t			dd	0