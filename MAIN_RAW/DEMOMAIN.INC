[segment code32]
[bits 32]
%include	"..\macros.inc"
%include	"..\eff_ext.inc"
extern	_decode_animations
extern	_output_initialize,_output_render
externeffect scrplay
extern	_outraw_init,_outraw_render

_demomain:
			; make new stack
			mov ax,data32
			mov ss,ax
			mov esp,1024*1024+64*1024-4
			sub esp,[_code32a]

			mov ax,data32
			mov es,ax

			call _initmem

			call _tl_loadtemplate
			call _decode_animations

			call _font_initialize
%ifdef DBOUT
			call _initscreen
			call _output_initialize
			call _clearscreen
%endif
			call _scrplay_initialize

			call _outraw_init

			mov dword [tickcount],0
	.dml1:		cmp dword [tickcount],0
			jz .dml1
			mov ecx,[tickcount]
			mov dword [tickcount],0
		.dml2:	push ecx
			call _scrplay_step
			pop ecx
			loop .dml2
			call _scrplay_render
%ifdef DBOUT
			call _output_render
			call _putscreen
%endif
			call _outraw_render
			jmp .dml1

			ret	; well, not needed :)
