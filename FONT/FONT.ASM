%include "..\segments.inc"
[segment codeseg]
[bits 32]

extern	_allocmem,_freemem
global	_font_initialize,_font_destroy,_font_selectfont
global	_ff_putchar,_ff_writez, ffwidth, pccharwidth, ffX,ffY

_font_initialize:

			mov byte [currentfont],0

			mov ecx,numfonts
	.fil1:		push ecx

			mov eax,256*4
			call _allocmem
			mov [asciitab],eax

			movzx eax,byte [currentfont]
			mov esi,[fontsources+4*eax]
			mov edi,[asciitab]
			mov ebp,[fontlengths+4*eax]
			call _parsefont

			movzx eax,byte [currentfont]
			inc al
			mov bl,numfonts
			div bl
			mov al,ah
			call _font_selectfont

			pop ecx
			loop .fil1

			ret

_font_selectfont:

			movzx ebx,byte [currentfont]
			shl ebx,2
			mov edx,[asciitab]
			mov [asciitabs+ebx],edx
			mov edx,[fontheight]
			mov [fontheights+ebx],edx
			movzx ebx,al
			mov [currentfont],al
			shl ebx,2
			mov eax,[asciitabs+ebx]
			mov [asciitab],eax
			mov eax,[fontheights+ebx]
			mov [fontheight],eax

			ret

_font_destroy:

			mov eax,[asciitab]
			call _freemem

			ret

_parsefont:
			push edi	;clear ascii -> item offset tab
			mov ecx,256
			xor eax,eax
			rep stosd
			pop edi

			mov ecx,esi
			lea edx,[esi+ebp]
			lea ebp,[esi+256+4]
			xor bl,bl	;index
			mov eax,[esi+256]
			mov [fontheight],eax
	.pfl1:		;- find ascii value for this index
			mov esi,ecx	;maps ascii to item index
			mov bh,-1
	.pfl2:		inc bh
			lodsb
			cmp al,bl
			jnz .pfl2
			movzx eax,bh
			mov [edi+4*eax],ebp
			mov al,[fontheight]
			mul byte [ebp]
			movzx eax,ax
			shr eax,3
			inc eax
			lea ebp,[ebp+eax+4]
			inc bl
			cmp ebp,edx
			jc .pfl1

			ret

;-----------------------
;  _ff_putchar, _ff_writez
; in:	al - char / esi - asciiz string
;	ffX, ffY
;	[ds:ebp] - output buffer descriptor:
;			0	4	offset
;			4	4	width
;			8	4	height
;			12	1	flags:
;					  0 - get width only
;			13	1	bgcolor (255 - transparent)
;			14	1	color
; out:	pccharwidth / ffwidth
;
_ff_putchar:
			pushad

			movzx eax,al
			mov ebx,[asciitab]
			mov esi,[ebx+4*eax]
			or esi,esi
			jnz .pc0
			popad
			ret	;character not present
	.pc0:

			mov eax,[ffY]
			mov [pcY],eax

			lodsd
			mov [pccharwidth],eax
			bt dword [ds:ebp+12],0
			jnc .pcdraw
	 .ne		popad
			ret
	.pcdraw:	cmp dword [ffX],-24
	                jnge .ne
	                mov eax,[ebp+4]
	                cmp dword [ffX],eax
	                jg .ne
                        xor eax,eax
			mov [bitpos],eax
			mov ecx,[fontheight]
	.pcl1:		mov edx,[pcY]
			cmp edx,0
			jnge .pc2
			cmp edx,[ds:ebp+8]
			jge .pc2
			mov eax,[ds:ebp+4]
			mul edx
			add eax,[ds:ebp]
			mov edi,eax
			mov eax,[ffX]
			mov [pcX],eax
			push ecx
			mov ecx,[pccharwidth]
	.pcl2:		mov edx,[pcX]
			cmp edx,0
			jnge .pc1
			cmp edx,[ds:ebp+4]
			jge .pc1
			call _get_bit
			lea ebx,[ebp+13]
			xlatb
			cmp al,255
			jz .pc1
			mov [edi+edx],al
		.pc1:	inc dword [pcX]
			inc dword [bitpos]
			loop .pcl2
			pop ecx
			jmp .pc3
		.pc2:	mov eax,[pccharwidth]
			add [bitpos],eax
		.pc3:
			inc dword [pcY]
			loop .pcl1

			popad
			ret

_ff_writez:

			push esi
			push dword [ffX]

	.ffwl1:		lodsb
			cmp al,0
			jz .ffwend
			push esi
			call _ff_putchar
			pop esi
			mov eax,[pccharwidth]
			add [ffX],eax
			jmp .ffwl1
	.ffwend:

			mov eax,[ffX]
			pop dword [ffX]
			sub eax,[ffX]
			mov [ffwidth],eax

			pop esi

			ret

_get_bit:
			push ecx
			mov eax,[bitpos]
			mov ecx,eax
			shr eax,3
			mov al,[esi+eax]
			not cl
			and ecx,7
			bt eax,ecx
			jc .nz
			xor al,al
			jz .z
		.nz:	mov al,1
		.z:
			pop ecx
			ret

[segment dataseg]
asciitab	dd	0
fontheight	dd	0
numfonts	equ	2
asciitabs	times	numfonts dd(0)
fontheights	times	numfonts dd(0)
fontsources	dd	font1,font2
fontlengths	dd	font1len,font2len
currentfont	db	0
pcX		dd	0
pcY		dd	0
ffX		dd	0
ffY		dd	0
ffwidth		dd	0
pccharwidth	dd	0
bitpos		dd	0
%include	"font1.inc"
%include	"font2.inc"
%include	"fx.inc"
