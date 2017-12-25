
_renderscreen:

;			mov edi,[dmamem]
;			mov ecx,16*3
;			mov ax,0xFFFF
;			cld
;			rep stosw
 ;
;			ret

;			mov edi,[matrix]
;			mov ecx,24
;			xor ebx,ebx
;	.l1:		push ecx
;			mov al,bh
;			mov bl,63
;			mul bl
;			mov bl,24
;			div bl
;			mov ecx,24
;			rep stosb
;			inc bh
;			pop ecx
;			loop .l1

			xor eax,eax
			mov [bufofs],eax
			mov [currow],al
			mov ecx,16
	.rsl0:		push ecx
			mov edi,16384
			add edi,[dmamem]
			mov esi,[matrix]
			mov ebp,12*24-8
			mov edx,-12*24
			mov ecx,12
	.rsl1:		push ecx
			mov cl,6
	.rsl2:		push ecx
			push edx
			xor dh,dh
			mov cl,8
	.rsl3:		shl dh,1
			lodsb
;			or al,al
;			jz .rs1
			shr al,1
			and al,31
			neg al
			add al,31
;			mov bl,al
;			mov al,[currow]
;			xor ah,ah
;			div bl
;			or ah,ah
			cmp al,[currow]
			ja .rs1
			or dh,1
		.rs1:	loop .rsl3
			mov al,dh
			stosb
			pop edx
			add esi,ebp
			xchg edx,ebp
			pop ecx
			loop .rsl2
			pop ecx
			loop .rsl1
			;- rearrange stuff
			;- exchange upper and lower part
			mov ecx,12*3
			mov esi,16384
			add esi,[dmamem]
			mov edi,esi
		.rl1:	lodsw
			xchg al,ah
			stosw
			loop .rl1
			;- reverse the bits of the new lower part
			mov ecx,12
			mov esi,16384
			add esi,[dmamem]
			mov edi,esi
		.rl2:	push cx
			mov cl,3
		.rl3:	lodsb
			call _revb
			stosb
			inc edi
			inc esi
			loop .rl3
			mov al,[edi-2]
			mov ah,[edi-6]
			mov [edi-2],ah
			mov [edi-6],al
			pop cx
			loop .rl2
			;- first line put last - others shifted up
			mov esi,16384
			add esi,[dmamem]
			mov edi,11*3*2
			add edi,[bufofs]
			add edi,[dmamem]
			movsw
			movsw
			movsw
			mov esi,16384+3*2
			add esi,[dmamem]
			mov edi,[bufofs]
			add edi,[dmamem]
			mov ecx,11*3
			rep movsw
.end:			pop ecx
			mov eax,12*3*2
			add [bufofs],eax
			inc byte [currow]
			inc byte [currow]
			dec ecx
			jnz near .rsl0


			ret

_revb:
			push ecx
			mov ah,al
			mov ecx,8
	.rbl1:		rcr ah,1
			rcl al,1
			loop .rbl1
			pop ecx
			ret

bufofs			dd	0
currow			db	0
