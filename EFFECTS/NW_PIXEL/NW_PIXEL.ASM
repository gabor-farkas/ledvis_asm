%include "../../segments.inc"
%include "vertbars.inc"
%include "horlines.inc"
%include "fadebl.inc"
%include "fadecr.inc"
%include "horzscrl.inc"
%include "zoom.inc"
%include "ptab.inc"
[segment codeseg]
[bits 32]

global	_nwp_initialize,_nwp_destroy,_nwp_step,_nwp_render,_nwp_startname
; _nwp_startname counts the number of frames showing the name will take
global	nwp_numframes
global	nwp_effect
global	nwp_stringofs
global	nwp_effects,nwp_numeffects,nwp_params
extern	_allocmem,_freemem
extern	ffX,ffY,_ff_putchar,pccharwidth,_font_selectfont,_fx1_putchar
extern	_strlen
extern	matrix

_nwp_initialize:

			mov [letter_increment],al

			mov ecx,24*24/4
			xor eax,eax
			mov [image],eax
			mov [counter],al
			mov [counter2],al
			mov edi,[matrix]
			rep stosd
			call _nwp_startname

			ret

_nwp_destroy:

			ret

_nwp_step:

			mov al,[counter2]
			add al,[letter_increment]
			cmp al,letter_countmax
			jc .s0
			mov ah,[image]
			inc ah
			mov bl,[numimages]
			dec bl
			cmp ah,bl
			jc .s1
			xor ah,ah
	.s1:		mov [image],ah
			xor al,al
	.s0:		mov [counter2],al
			cmp al,24
			jc .s2
			mov al,23
	.s2:		mov [counter],al

			ret

_nwp_render:

			mov edi,[matrix]	;edi - matrix
			mov eax,24*24
			mul dword [image]
			add eax,[images]
			mov esi,eax		;esi - image leaving
			mov ebp,24*24		;ebp - 24*24
			;- [counter] goes 0..23 - not including stayon time
			call [effect]

			ret

_nwp_startname:

			mov al,1
			call _font_selectfont

			cmp dword [images],0
			jz .sn1
			mov eax,[images]
			call _freemem
	.sn1:

			mov esi,[stringofs]
			call _strlen
			mov eax,ecx
			mov [numimages],eax
			mov ebx,24*24
			mul ebx
			call _allocmem
			mov [images],eax
			mov eax,letter_countmax
			mov ebx,[numimages]
			dec ebx
			mul ebx
			div dword [letter_increment]
			dec eax
			mov [nwp_numframes],eax

			mov dword [ffX],3
			mov dword [ffY],-4
			mov ecx,[numimages]
			mov esi,[stringofs]
			mov ebx,[images]
	.snl1:		push ecx
			lodsb
			push esi
			push ebx
			mov ebp,viewport
			mov [ds:ebp],ebx
			call _fx1_putchar
			pop ebx
			add ebx,24*24
			pop esi
			pop ecx
			loop .snl1

			xor al,al
			call _font_selectfont

			ret

[segment dataseg]
numimages		dd	0
images			dd	0
counter			db	0
counter2		db	0
image			dd	0
effect			dd	_horlines
nwp_effect		equ	effect
teststring		db	' Újfaludi András ',0
nwp_numframes		dd	0
stringofs		dd	teststring
nwp_stringofs		equ	stringofs
viewport		dd	0,24,24
			db	0,255,63
letter_countmax		equ	48
letter_increment	dd	3
nwp_effects		dd	_fadebl,_fadecr,_horlines,_horzscrl,_vertbars
			dd	_ptab
nwp_params		db	5,5,3,5,4,2
nwp_numeffects		dd	6