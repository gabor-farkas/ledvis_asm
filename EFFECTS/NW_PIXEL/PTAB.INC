[segment codeseg]
[bits 32]

global	_ptab

_ptab:

			add ebp,esi
			mov ecx,4
			mov ah,[counter]
	.ptl1:		push ecx
			mov cl,3
			xor edx,edx
	.ptl2:		push ecx
			mov cl,6
	.ptl3:		push ecx
			mov cl,8
	.ptl4:		lodsb
			mov bl,ah
			cmp ah,12
			jc .x
			xor al,al	;fade from dark in part 2
			sub bl,12
		.x:	cmp bl,[ptab+edx]
			jc .old
			mov al,[ebp]
			cmp ah,12
			jnc .old
			xor al,al	;fade to dark in part 1
		.old:	stosb
			inc ebp
			inc edx
			loop .ptl4
			pop ecx
			sub edx,byte 8
			loop .ptl3
			pop ecx
			add edx,byte 8
			loop .ptl2
			pop ecx
			loop .ptl1

			ret

[segment dataseg]
;ptab		db	0,0,1,1,2,2,3,3
;		db	4,4,5,5,6,6,7,7
;		db	8,8,9,9,10,10,11,11
ptab		db	0,0,3,3,6,6,9,9
		db	1,1,4,4,7,7,10,10
		db	2,2,5,5,8,8,11,11