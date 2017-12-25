%include "../../segments.inc"
[segment codeseg]
[bits 32]

global _film_initialize,_film_destroy,_film_step,_film_render
global film_animname,film_string,film_numframes
extern	matrix
extern	ffX,ffY,_ff_writez,_font_selectfont,_fx1_putchar,_ff_putchar
extern	_strlen
extern	_allocmem,_freemem
extern	_giveresource
extern	tl_data

_film_initialize:

			mov esi,[panimname]
			call _giveresource
			mov [animation],esi

			mov esi,[pstring]
			call _strlen
			mov [strlen],ecx
			mov eax,24
			mul ecx
			mov [viewport+8],eax
			mov ebx,24
			mul ebx
			call _allocmem
			mov [letters],eax
			mov [viewport],eax

			;render text
			mov al,1
			call _font_selectfont
			mov dword [ffX],3
			mov dword [ffY],-4
			mov ecx,[strlen]
			mov esi,[pstring]
	.fil1:		lodsb
			push esi
			push ecx
			mov ebp,viewport
			call _fx1_putchar
			add dword [ffY],byte 24
			pop ecx
			pop esi
			loop .fil1

			xor eax,eax
			mov [position],eax
			mov [speed],eax
			call _font_selectfont
			mov dword [acceleration],8

			mov ebx,[animation]
			mov eax,[ebx+13]
			add eax,[tl_data]
			mov [animdata],eax
			imul eax,[ebx],byte 24
			mov [animheight],eax
			imul eax,byte 24
			add eax,[animdata]
			mov [animend],eax
			mov eax,[ebx]
			shl eax,2
			mov [numframes],eax

			ret

_film_destroy:

			mov eax,[letters]
			call _freemem

			ret

_film_step:

			mov eax,[speed]
			add [position0],eax
			add eax,[acceleration]
			cmp eax,256*24
			jc .fs1
			mov eax,[position]
			cdq
			mov ebx,24
			div ebx
			mov eax,256*24
			or edx,edx
			jz .fs1
			mov eax,256*25
	.fs1:		mov [speed],eax

			ret

_film_render:

			mov esi,[letters]
			mov edi,[matrix]
			mov eax,[viewport+8]
			sub eax,[position]
			cmp eax,24
			jng .fr1
			;- the whole stuff is from the text
			mov eax,24
			mul dword [position]
			add esi,eax
			mov ecx,24*24/4
			rep movsd
			ret
.fr1:			or eax,eax
			jng .fr2
			;- some from the text
			push eax
			shl eax,1
			lea ecx,[eax+2*eax]
			mov eax,24
			mul dword [position]
			add esi,eax
			rep movsd
			pop eax
			;- and some from the animation
			mov ecx,24
			sub ecx,eax
			shl ecx,1
			lea ecx,[ecx+2*ecx]
			mov esi,[animdata]
			rep movsd
			ret
.fr2:			;- whole stuff from the animation
			mov dword [acceleration],32
			neg eax
			cdq
			mov ebx,[animheight]
			div ebx
			imul eax,edx,byte 24
			mov esi,[animdata]
			add esi,eax
			mov ecx,24
	.frl1:		push ecx
			mov cl,6
			rep movsd
			pop ecx
			cmp esi,[animend]
			jc .fr3
			mov esi,[animdata]
		.fr3:	loop .frl1

			ret

[bits 32]
[segment dataseg]
panimname		dd	0
pstring			dd	0
numframes		dd	1000
strlen			dd	0
letters			dd	0
film_animname		equ	panimname
film_string		equ	pstring
film_numframes		equ	numframes
viewport		dd	0,24,0
			db	0,255,63
position0		db	0
position		dd	0
speed			dd	0
acceleration		dd	8
animation		dd	0
animheight		dd	0
animend			dd	0
animdata		dd	0