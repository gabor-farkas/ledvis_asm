;%define BOCHS
[segment code32]
[bits 32]

PORT	equ	0x1FF

extern	matrix
extern	_allocmem
extern	_code32a
extern	_debugeax,_writehexdw

global	_outraw_init,_outraw_render,_outraw_destroy

_outraw_init:

			mov dx,PORT
			xor al,al
			out dx,al

%ifdef BOCHS
			; BOCHS output driver init
			mov al,0x80
			out dx,al
			mov eax,[matrix]
			add eax,[_code32a]
			out dx,al
			shr eax,8
			out dx,al
			shr eax,8
			out dx,al
			shr eax,8
			out dx,al
			xor al,al
			out dx,al
%endif

			call _dma_init

			mov dx,PORT
			xor al,al
			out dx,al
			mov al,7+16
			out dx,al
			mov al,3
			out dx,al
			mov al,7
			out dx,al

			ret

_outraw_render:

			call _renderscreen

%ifdef BOCHS
			mov dx,PORT
			mov al,0x40
			out dx,al	;report new screen to BOCHS device
			mov al,7
			out dx,al
%endif
			ret

_outraw_destroy:

			mov dx,PORT
			xor al,al
			out dx,al

			ret

%include	"dma.inc"
%include	"screen.inc"