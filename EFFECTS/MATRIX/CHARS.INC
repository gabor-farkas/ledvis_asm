[segment dataseg]
numchars	equ	10
code_h		equ	numchars+1
code_dot	equ	numchars+2
code_one	equ	numchars+3
code_three	equ	numchars+4
matrixchars
	db	0,0,0,0,0

	db	01110b
	db	10001b
	db	10001b
	db	10001b
	db	01110b

	db	00100b
	db	01100b
	db	00100b
	db	00100b
	db	00100b

	db	01110b
	db	10001b
	db	00010b
	db	01100b
	db	11111b

	db	11110b
	db	00001b
	db	00010b
	db	00001b
	db	11110b

	db	01000b
	db	01000b
	db	01111b
	db	00010b
	db	00010b

	db	11111b
	db	10000b
	db	11110b
	db	00001b
	db	11110b

	db	01110b
	db	10000b
	db	11110b
	db	10001b
	db	01110b

	db	11111b
	db	00010b
	db	00100b
	db	01000b
	db	10000b

	db	01110b
	db	10001b
	db	01110b
	db	10001b
	db	01110b

	db	01110b
	db	10001b
	db	01111b
	db	00001b
	db	01110b

	;-- extra chars
	db	11011b
	db	11011b
	db	11111b
	db	11011b
	db	11011b

	db	00000b
	db	00000b
	db	00000b
	db	00110b
	db	00110b

	db	00110b
	db	01110b
	db	00110b
	db	00110b
	db	00110b

	db	11110b
	db	00011b
	db	00110b
	db	00011b
	db	11110b