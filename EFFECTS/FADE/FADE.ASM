%include "../../segments.inc"
[segment codeseg]
[bits 32]

global	_fade_initialize,_fade_destroy,_fade_step,_fade_render
global	fadespeed
extern	matrix

_fade_initialize:

			mov byte [fadecount],0

			ret

_fade_destroy:

			ret

_fade_step:

			mov al,[fadespeed]
			add [fadecount],al
			jnc .fs1
			mov byte [fadecount],255
	.fs1:

			ret

_fade_render:

			mov esi,[matrix]
			mov edi,esi
			mov ecx,24*24
			mov bl,[fadecount]
			not bl
	.frl1:		lodsb
			mul bl
			mov al,ah
			stosb
			loop .frl1

			ret

[segment dataseg]
fadecount		db	0
fadespeed		db	4