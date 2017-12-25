[segment codeseg]
[bits 32]

global _fx1_putchar

_fx1_putchar:

			dec dword [ffX]
			dec dword [ffY]
			mov byte [ds:ebp+14],63
			call _ff_putchar
			add dword [ffX],byte 2
			add dword [ffY],byte 2
			mov byte [ds:ebp+14],24
			call _ff_putchar
			dec dword [ffX]
			dec dword [ffY]
			mov byte [ds:ebp+14],48
			call _ff_putchar

			ret

[segment dataseg]