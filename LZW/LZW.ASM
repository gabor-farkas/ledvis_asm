%include "../segments.inc"
%include "lzw.inc"
[segment codeseg]
[bits 32]

global _lzw_show

extern	_allocmem,_freemem
;extern	_keywait,_debugeax
;extern	screenbuf,_putscreen

_lzw_show:

;			mov eax,[data]
;			mov [lzw_source],eax
;			mov eax,[datalen]
;			mov [lzw_slen],eax
;			mov eax,65536
;			call _allocmem
;			mov [dest],eax
;			mov [lzw_destination],eax


			;---
			call _lzw_uncompress
;			mov esi,[lzw_destination]
;			mov edi,[screenbuf]
;			mov ecx,64000/4
;			rep movsd
;			call _putscreen
;			call _keywait

			mov eax,[dest]
			call _freemem

			ret

[segment dataseg]
;data		dd	sdata
;datalen		dd	sdatalen
dest		dd	0