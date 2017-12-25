global	_read_pos,dmapos
global  _dma_restart

_dma_init:

		mov eax,2*65536
		call _allocmem
		mov [memory],eax
;		call _debugeax
		add eax,[_code32a]
		mov ebx,eax
		shr ebx,16
		mov [dmapage],bx
		mov bx,0x4000
		mov [dmaoffset],bx
		shr eax,4
		and ax,4095
		cmp ax,2048
		jc .bla
		xor ax,ax
		mov [dmaoffset],ax
		inc word [dmapage]
.bla:		mov al,[dmapage]
		and al,1
		jz .bla2
		inc byte [dmapage]	;only even pages
.bla2:
_dma_restart:
		mov al,5
		out 0xD4,al
		mov al,[dmapage]
		out 0x8B,al
		xor al,al
		out 0xD8,al
		mov al,[dmalen]
		out 0xC6,al
		mov al,[dmalen+1]
		out 0xC6,al
		mov al,[dmaoffset]
		out 0xC4,al
		mov al,[dmaoffset+1]
		out 0xC4,al
		mov al,0x59
		out 0xD6,al
		mov al,1
		out 0xD4,al
		movzx eax,word [dmaoffset]
		shl eax,1
		mov [dmapos],ax
		mov [dmaold],ax
		movzx ebx,word [dmapage]
		shl ebx,16
		add eax,ebx
		sub eax,[_code32a]
		mov [dmamem],eax

;		movzx eax,word [dmapage]
;		call _debugeax
;		movzx eax,word [dmaoffset]
;		call _debugeax
;		mov eax,[dmamem]
;		add eax,[_code32a]
;		call _debugeax

		ret

_read_pos:

		xor al,al
		out 0xD8,al
		in al,0xC4
		mov [dmapos],al
		in al,0xC4
		mov [dmapos+1],al

		ret

memory		dd	0
dmamem		dd	0
dmapage		dw	0
dmaoffset	dw	0
dmalen		dw	12*16*3-1
dmaseg		dw	0
dmapos		dw	0
dmaold		dw	0
dmacount	dw	0,0
