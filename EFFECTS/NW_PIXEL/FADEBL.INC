[segment codeseg]
[bits 32]

global _fadebl

_fadebl:

			mov al,[counter]
			cmp al,12
			jc .fb1
			neg al
			add al,23
			add esi,ebp
	.fb1:		mov bl,255
			mul bl
			mov bl,12
			div bl
			mov bl,al
			not bl
			mov ecx,24*24
	.fbl1:		lodsb
			mul bl
			mov al,ah
			stosb
			loop .fbl1

			ret

[segment dataseg]