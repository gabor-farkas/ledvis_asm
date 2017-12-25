[segment text]
[bits 32]

extern	_HeapAlloc@12,_HeapFree@12

HEAP_ZERO_MEMORY	equ	8

global _allocmem,_freemem

_allocmem:

			pushad
			invoke _HeapAlloc@12,dword [hHeap],byte HEAP_ZERO_MEMORY,eax
			mov [mtemp],eax
			popad
			mov eax,[mtemp]

			ret

_freemem:
			pushad
			invoke _HeapFree@12,dword [hHeap],byte 0,eax
			popad

			ret

[segment .data]
hHeap			dd	0
mtemp			dd	0