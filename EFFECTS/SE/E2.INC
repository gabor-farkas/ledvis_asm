[segment codeseg]
[bits 32]

_im_2:

			clrbuf outline2
			clrbuf outline
			mov edi,[outline]
			add edi,byte 48
			mov ecx,24
			mov esi,grdtab
			;mov ebx,12
	.iml:		push ecx
	                ;push edi
			lodsb
			shl al,1
			;mov al,63*2
			;mov cl,bl
			mov cl,48
			rep stosb
			;pop edi
			;add edi,byte 48
			pop ecx
			;inc ebx
			loop .iml

			ret

_pef_2			equ	_pef_0

[segment dataseg]
grdtab		db	63,48,32,16,8,4,0,0,0,0,0,0
		db	0,0,0,0,0,0,4,8,16,32,48,63
