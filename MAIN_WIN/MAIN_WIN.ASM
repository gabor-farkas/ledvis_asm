%include "..\macros.inc"
%include "mem32.inc"
%include "tloader.inc"
%include "debug.inc"
[segment text]
[bits 32]
%DEFINE WIN32

extern	_GetProcessHeap@0

global _main

global	_main_initialize,_main_destroy,_main_step,_main_render
global	_matrixaddr
global	matrix
extern	_font_initialize,_font_destroy
externeffect scrplay
extern	_decode_animations

_main_initialize:
		pushad

		invoke _GetProcessHeap@0
		mov dword [hHeap],eax

		call _font_initialize

		call _tl_loadtemplate
		call _decode_animations

		call _scrplay_initialize

		popad
		ret

_main_destroy:
		pushad

		call _scrplay_destroy

		call _font_destroy

		popad
		ret

_main_step:
		pushad

		call _scrplay_step

		popad
		ret

_main_render:
		pushad

		call _scrplay_render

		popad
		ret

_matrixaddr:

		mov eax,[matrix]

		ret

[segment .data]
matrix		dd	_matrix
_matrix 	times	24*24 db(0)
