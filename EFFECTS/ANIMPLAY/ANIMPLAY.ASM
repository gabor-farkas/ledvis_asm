%include "..\..\segments.inc"
%include "decoder.inc"
[segment codeseg]
[bits 32]

extern	matrix
extern	_giveresource,tl_data

global	_animplay_initialize,_animplay_destroy,_animplay_step,_animplay_render
global	animname,animframes

_animplay_initialize:

			mov esi,[animname]
			call _giveresource
			jnc .notfound
			mov [anim0],esi
			xor eax,eax
			mov [currentframe],eax

			ret
	.notfound:	mov byte [notfound],1
			ret

_animplay_destroy:

			ret

_animplay_step:

			cmp byte [notfound],1
			jnz .as2
			ret
	.as2:

			mov esi,[anim0]
			mov eax,[currentframe]
			inc eax
;			inc eax
			cmp eax,[esi]
			jc .as1
			xor eax,eax
	.as1:		mov [currentframe],eax

			ret

_animplay_render:

			cmp byte [notfound],1
			jnz .ar1
			ret
	.ar1:

			mov eax,24*24
			mul dword [currentframe]
			mov ebx,[anim0]
			mov ebx,[ebx+13]
			add ebx,[tl_data]
			add eax,ebx
			mov esi,eax
			mov edi,[matrix]
			mov ecx,24*24/4
			rep movsd


			ret

[segment dataseg]
animname0	db	'Adam2',0
animname	dd	animname0
anim0		dd	0
animframes	equ	anim0
currentframe	dd	0
notfound	db	0