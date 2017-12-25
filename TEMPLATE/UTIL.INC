[segment codeseg]

global _strlen

; in:	esi
; out:	ecx
_strlen:
			push esi
			push eax
			xor eax,eax
			xor ecx,ecx
			dec ecx
	.sll1:		inc ecx
			lodsb
			cmp al,ah
			jnz .sll1

			pop eax
			pop esi
			ret