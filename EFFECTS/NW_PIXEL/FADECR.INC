[segment codeseg]
[bits 32]

global _fadecr

_fadecr:

			mov al,[counter]
			mov bl,255
			mul bl
			mov bl,23
			div bl
			mov bl,al
			mov bh,al
			not bl
			mov ecx,24*24
			dec ebp
	.fbl1:		lodsb
			mul bl
			mov dl,ah
			mov al,[esi+ebp]
			mul bh
			add ah,dl
			mov al,ah
			stosb
			loop .fbl1

			ret

[segment dataseg]