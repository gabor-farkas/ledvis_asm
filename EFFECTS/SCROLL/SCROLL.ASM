;
;	puts data to [scroll_outbuf]
;
;
%include "..\..\segments.inc"
[segment codeseg]
[bits 32]


global	_scroll_initialize,_scroll_destroy,_scroll_step,_scroll_render
global	scroll_szovegaddr,scroll_numframes,scroll_outbuf
extern	_ff_writez, ffwidth, ffX,ffY, _font_selectfont
extern	matrix
extern	_allocmem,_freemem
extern	_debugeax

_scroll_initialize:

			xor eax,eax
			mov [position],eax

			mov al,0
			call _font_selectfont

			mov ebp,viewport
			or byte [ebp+12],1
			mov esi,[scroll_szovegaddr]
			call _writez2
			mov eax,[ffwidth]
			mov [width],eax
			add eax,byte 36
			mov [scroll_numframes],eax
			and byte [ebp+12],254

			mov eax,48*24
			call _allocmem
			mov [scroll_outbuf],eax
			mov [viewport],eax

			ret

_scroll_destroy:

			mov eax,[scroll_outbuf]
			call _freemem

			ret

_scroll_step:

			mov eax,[position]
			inc eax
			mov ebx,[width]
			add ebx,byte 36
			cmp eax,ebx
			jc .ss1
			xor eax,eax
		.ss1:	mov [position],eax

			ret

_scroll_render:

			mov edi,[scroll_outbuf]
			mov ecx,48*24/4
			xor eax,eax
			rep stosd

			mov eax,[position]
			neg eax
			add eax,byte 48
			mov [ffX],eax
			mov esi,[scroll_szovegaddr]
			mov ebp,viewport
			call _writez2
;			mov eax,[ffwidth]
;			add [ffX],eax
;			mov esi,[scroll_szovegaddr]
;			call _writez2

			ret

_writez2:

;			dec dword [ffY]
			mov byte [viewport+14],63
;			call _ff_writez
;			add dword [ffY],byte 2
;			call _ff_writez
;			dec dword [ffY]
;			dec dword [ffX]
;			call _ff_writez
;			add dword [ffX],byte 2
;			call _ff_writez
;			dec dword [ffX]
;			mov byte [viewport+14],32
			mov dword [ffY],4
			call _ff_writez

			ret

[segment dataseg]
szoveg0			db	'bla',0
scroll_szovegaddr	dd	szoveg0

viewport		dd	0,48,24
			db	0,255,63
position		dd	0
width			dd	10
scroll_numframes	dd	10
scroll_outbuf		dd	0