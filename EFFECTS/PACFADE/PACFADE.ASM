%include "../../segments.inc"
%include "../../macros.inc"
[segment codeseg]
[bits 32]

globaleffect	pacfade
global	pf_numframes
extern	matrix
extern	_giveresource,tl_data
extern	_debugeax

_pacfade_initialize:

			mov esi,pacname
			call _giveresource
			mov eax,[esi+8]
			add eax,[tl_data]
			mov [pacimage],eax

			ret

_pacfade_destroy:

			ret

_pacfade_step:

			ret

_pacfade_render:

			mov eax,3
			call _putpacimage

			ret

;-------------------------------------------------

;in:	eax	- index
;	ppX,ppY
_putpacimage:

			shl eax,3
			mov esi,[imgofstab+eax+4]
			imul esi,byte 24
			add esi,[imgofstab+eax]
			add esi,[pacimage]
			mov edi,[ppY]
			imul edi,byte 24
			add edi,[ppX]
			add edi,[matrix]

			mov ecx,12
	.ppil1:		push ecx
			mov cl,3
			rep movsd
			add esi,byte 12
			add edi,byte 12
			pop ecx
			loop .ppil1

			ret

[segment dataseg]
pf_numframes		dd	24+12+24
pacname			db	'pacman\kicsi',0
pacimage		dd	0
imgofstab		dd	0,0,12,0,0,12,12,12
ppX			dd	0
ppY			dd	0