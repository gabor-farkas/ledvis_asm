[segment code32]
[bits 32]

global _output_initialize,_output_destroy,_output_render
extern	screenbuf
extern	matrix

_output_initialize:

			mov dx,0x3C8
			xor ah,ah
			mov ecx,64
	.oil1:		mov al,ah
			add al,32
			out dx,al
			inc dl
			mov al,ah
			out dx,al
			xor al,al
			out dx,al
			out dx,al
			inc ah
			dec dl
			loop .oil1
			;-greyscale
			mov cl,64
			xor ah,ah
	.oil2:		mov al,ah
			add al,96
			out dx,al
			inc dl
			mov al,ah
			out dx,al
			out dx,al
			out dx,al
			inc ah
			dec dl
			loop .oil2

			ret

_output_destroy:

			ret

_output_render:

			; ‚s itt majd nagy l¢faszt nyomunk a segg‚be !

			mov esi,[matrix]
			mov edi,[screenbuf]
			mov ecx,24
	.orl1:		push ecx
			mov ecx,24
	.orl2:		push ecx
			lodsb
			add al,32
			mov ah,al
			xchg esi,ebp
			mov esi,kor
			push edi
			mov ecx,8
	.orl3:		push ecx
			mov ecx,8
	.orl4:		lodsb
			or al,al
			jz .or1
			mov al,ah
		.or1:	stosb
			loop .orl4
			add edi,320-8
			pop ecx
			loop .orl3
			pop edi
			add edi,8
			xchg esi,ebp
			pop ecx
			loop .orl2
			add edi,320-24*8+7*320
			pop ecx
			loop .orl1

			mov esi,[matrix]
			mov edi,[screenbuf]
			add edi,256
			mov ecx,24
			mov ebx,0x60606060
	.orl5:		push ecx
			mov cl,6
	.orl6:		lodsd
			add eax,ebx
			stosd
			loop .orl6
			add edi,320-24
			pop ecx
			loop .orl5

			mov esi,[matrix]
			mov edi,[screenbuf]
			add edi,256-12+64*320
			mov ecx,24
			mov ah,96
	.orl7:		push ecx
			mov cl,24
	.orl8:		lodsb
			add al,ah
			stosb
			inc edi
			loop .orl8
			add edi,640-48
			pop ecx
			loop .orl7

			ret

[segment code32]
kor			db	0,0,0,0,0,0,0,0
			db	0,0,1,1,1,1,0,0
			db	0,1,1,1,1,1,1,0
			db	0,1,1,1,1,1,1,0
			db	0,1,1,1,1,1,1,0
			db	0,1,1,1,1,1,1,0
			db	0,0,1,1,1,1,0,0
			db	0,0,0,0,0,0,0,0
