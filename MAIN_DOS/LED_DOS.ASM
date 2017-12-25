[segment code32]
[bits 32]

%include	"..\pmode.inc"
%include 	"screen.inc"
%include	"timing.inc"
%include	"memory.inc"
%include	"../macros.inc"
%include	"tloader.inc"

extern	_output_initialize,_output_destroy,_output_render
extern	_font_initialize,_font_destroy
%include	"../eff_ext.inc"
externeffect	scrplay
extern	_lzw_show
extern	sinetab,_initsines
extern	_outraw_init,_outraw_render,_outraw_destroy
extern	_giveresource
extern	_decode_animations

global	_main

_main:
	sti

	call _initmem

	call _timer_initialize

	call _tl_loadtemplate
	call _decode_animations

%ifndef RAWOUT
	call _initmcga
%endif
	call _initscreen
	call _output_initialize
%ifdef RAWOUT
	call _outraw_init
%endif
	call _font_initialize

	;call _clearscreen

	call _scrplay_initialize

	xor eax,eax
	mov [counter],eax
.ml:	cmp dword [counter],0
	jz .ml
	mov ecx,[counter]
	xor eax,eax
	mov [counter],eax
.ml2:	push ecx
	call _scrplay_step
	pop ecx
	loop .ml2
	call _scrplay_render
%ifdef RAWOUT
	call _outraw_render
%endif
%ifndef RAWOUT
	call _output_render
	call _putscreen
%endif

	push es
	mov word [v86r_ax],0x0100
	mov ax,16h
	int 33h
	pop es
	jnz .mlend

	jmp .ml
.mlend:

	call _scrplay_destroy

	call _timer_destroy

	push es
	mov word [v86r_ax],0
	mov ax,16h
	int 33h
	pop es


	call _font_destroy
%ifndef RAWOUT
	call _output_destroy
%endif
%ifdef RAWOUT
	call _outraw_destroy
%endif
	call _restoretext

	mov esi,[memblocks]
	mov ecx,maxblocks
	xor ebx,ebx
	xor eax,eax
.dl1:	cmp [esi],ebx
	jz .d1
	inc eax
.d1:	add esi,byte 8
	loop .dl1
	mov edx,0x1000
        ;call _writehexdw

	jmp _exit
%ifndef RAWOUT
global _dma_restart
_dma_restart:
            ret
%endif
