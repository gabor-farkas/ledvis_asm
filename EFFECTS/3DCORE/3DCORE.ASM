%include "..\..\segments.inc"
%include "sinetab.inc"
%include "trans.inc"
%include "facer.inc"
[segment codeseg]
[bits 32]

extern	_allocmem,_freemem
global	_core3d_init,_core3d_dest
global	_bufferconvert

_core3d_init:

			call _initsines

			ret

_core3d_dest:

			call _destsines

			ret

; in:	esi	-	96x96 input buffer
;	edi	-	24x24 output buffer
;	colors range from 0 to 63
_bufferconvert:

			mov ecx,24
	.bcl1:		push ecx
			mov cl,24
	.bcl2:		push ecx
			mov cl,4
			xor bl,bl
	.bcl3:		lodsw
			add ah,al
			lodsb
			add ah,al
			lodsb
			add ah,al
			shr ah,2
			add bl,ah
			add esi,96-4
			loop .bcl3
			sub esi,4*96-4
			mov al,bl
			shr al,2
			stosb
			pop ecx
			loop .bcl2
			add esi,96*3
			pop ecx
			loop .bcl1

			ret


[segment dataseg]