%include "..\..\segments.inc"
[segment codeseg]
[bits 32]

global	_falling_initialize,_falling_destroy,_falling_step,_falling_render
extern	_randomize, _random
extern	matrix
extern	_allocmem,_freemem

_falling_initialize:

			mov eax,blocks_size
			call _allocmem
			mov [blocks],eax

			mov ecx,numblocks
	.fil1:		mov eax,ecx
			dec eax
			call _newblock
			loop .fil1

			mov dword [mcounter],mc_start
			mov byte [part],0

			call _fall_start

			ret

_falling_destroy:

			mov eax,[blocks]
			call _freemem

			ret

_falling_step:

			dec dword [mcounter]
			jnz .fs3
			cmp byte [part],0
			jnz .fs30
			inc byte [part]
			mov byte [mcounter],mc_part1
			jmp .fs3
	.fs30:		cmp byte [part],1
			jnz .fs3
			inc byte [part]
	.fs3:

			cmp byte [part],1
			jnz .fs4
			inc byte [speed]
	.fs4:

			mov ecx,numblocks
			mov esi,blockdata
			xor bl,bl
	.fsl1:		mov ax,[speed]
			add ax,[esi+1]
			cmp ax,24*16
			jnge .fs1
			mov ax,24*16
			cmp byte [part],2
			jz .fs1
			movzx eax,bl
			push ebx
			call _newblock
			pop ebx
			jmp .fs2
		.fs1:	mov [esi+1],ax
		.fs2:	add esi,byte blockd_size
			inc bl
			loop .fsl1


			cmp byte [part],2
			jnz .fs5
			call _fall_step
	.fs5:

			ret

_falling_render:

			mov edi,[matrix]
			mov ecx,24*24/4
			xor eax,eax
			rep stosd

			mov ecx,numblocks
			mov esi,[blocks]
			mov ebp,blockdata
	.frl1:		push ecx
			mov al,6
			mul byte [ebp]
			movzx edi,al
			add edi,[matrix]
			movsx ebx,word [ebp+1]
			sar ebx,4
			neg ebx
			movzx ecx,byte [ebp+4]
			cmp ebx,ecx
			jnl .frskipall
			push esi
			cmp ebx,0
			jz .frstart
			jg .frskipfromsrc
			;- skip data from destination
			mov ecx,24
			add ecx,ebx
			neg ebx
			mov eax,24
			mul ebx
			add edi,eax
			jmp .frstart
	.frskipfromsrc:	sub ecx,ebx
			lea ebx,[ebx+4*ebx]
			add esi,ebx
	.frstart:	cmp ecx,0
			jng .frl2end
			cmp ecx,24
			jna .frl2
			mov ecx,24
	.frl2:		movsd
			movsb
			add edi,byte 24-5
			loop .frl2
	.frl2end	pop esi
	.frskipall:	add esi,blockheight*5
			add ebp,byte blockd_size
			pop ecx
			loop .frl1

			call _fall_render

			ret

_newblock:
			push ecx
			push esi

			push eax
			shl eax,blockd_s2
			add eax,blockdata
			mov edi,eax
			mov al,[lastrow]
			stosb
			inc al
			and al,3
			mov [lastrow],al
			mov eax,11
			call _random
			add al,5
			push eax
			shl eax,1
			lea ebx,[eax+2*eax]
			mov [edi+3],bl
			shl ebx,4
			neg ebx
			mov eax,5*16
			call _random
			add eax,ebx
			stosw
			mov eax,24
			call _random
			add al,8
			mov al,16
			stosb
			pop ecx
			pop ebx
			mov eax,blockheight*5
			mul ebx
			add eax,[blocks]
			mov edi,eax
			mov byte [pccolor],48
	.nbl1:		push ecx
			cmp cl,1
			jnz .nb1
			mov byte [pccolor],63
	.nb1:		mov eax,numchars
			call _random
			inc al
			call _putchar
			add edi,byte 5
			pop ecx
			loop .nbl1

			pop esi
			pop ecx
			ret

;-------------------------
; in:	al - charindex
;	edi - offset of buffer of width of 5 bytes
_putchar:

			mov bl,5
			mul bl
			movzx eax,ax
			add eax,matrixchars
			mov esi,eax
			mov ecx,5
			mov dl,[pccolor]
	.pcl1:		push ecx
			lodsb
			mov ah,al
			mov ecx,5
	.pcl2:		mov al,ah
			and al,16
			shr al,4
			jz .pc0
			mov al,dl
		.pc0:	stosb
			shl ah,1
			loop .pcl2
			pop ecx
			loop .pcl1

			ret

_putchar2:

			mov bl,5
			mul bl
			movzx eax,ax
			add eax,matrixchars
			mov esi,eax
			mov eax,24
			imul dword [pcY]
			add eax,[pcX]
			mov edx,[matrix]
			add eax,edx
			mov edi,eax
			mov ebp,[pcbufsize]
			add ebp,edx
			mov ecx,5
			mov bl,[pccolor]
	.pcl1:		cmp edi,ebp
			jnc .pcend
			cmp edi,edx
			jc .pcskipline
			push ecx
			lodsb
			mov ah,al
			mov ecx,5
	.pcl2:		mov al,ah
			and al,16
			shr al,4
			jz .pc0
			mov [edi],bl
		.pc0:	inc edi
			shl ah,1
			loop .pcl2
			sub edi,byte 5
			dec esi
			pop ecx
	.pcskipline:	inc esi
			add edi,byte 24
			loop .pcl1
	.pcend:

			ret

[segment dataseg]
pccolor			db	63
pcY			dd	0
pcX			dd	0
pcbufsize		dd	24*24
offset			db	0
blocks			dd	0
blockheight		equ	16*6
numblocks		equ	6
blocks_size		equ	blockheight*6*numblocks
blockdata		times	numblocks*8 db(0)
			;row,{pos,pos}(s12.4), speed(4.4), height, 0,0,0
blockd_s2		equ	3
blockd_size		equ	8
lastrow			db	0
mc_start		equ	24*4
mc_part1		equ	12
mcounter		dd	mc_start
part			db	0
speed			dw	16
%include	"chars.inc"
%include	"fall.inc"