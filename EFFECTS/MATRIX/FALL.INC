[segment codeseg]
[bits 32]

_fall_start:

			xor al,al
			mov [counter],al
			mov [counter2],al
			mov [charcode],al

			movzx eax,byte [chars]
			call _fall_startchar

			ret

_fall_startchar:

			mov [charcode],al
			mov [xpos],ah

			xor bl,bl
			mov bh,mblurphases
			mov cl,63
			mov edi,mblur
	.fsl1:		mov al,-10
			stosb
			mov al,bl
			mul cl
			div bh
			stosb
			inc bl
			cmp bl,bh
			jc .fsl1

			ret

_fall_step:

			mov al,[counter2]
			inc al
			cmp al,8
			jc .fs2
			mov al,[counter]
			inc al
			cmp al,4
			jnc .fs3
			mov [counter],al
			movzx eax,al
			mov ax,[chars+2*eax]
			call _fall_startchar
			jmp .fs4
	.fs3:		mov byte [counter],4
	.fs4:		xor al,al
	.fs2:		mov [counter2],al

			cmp byte [counter],4
			jz .fsnemstep

			mov ecx,mblurphases-1
			mov edi,mblur
			lea esi,[edi+2]
	.fsl1:		movsb
			inc esi
			inc edi
			loop .fsl1
			rep movsb

			mov al,[mblurlast]
			add al,3
			cmp al,9
			jnge .fs1
			mov al,9
		.fs1:	mov [mblurlast],al

	.fsnemstep:

			ret

_fall_render:

			movzx ecx,byte [counter]
			cmp cl,0
			jz .frnem
			mov byte [pccolor],63
			mov esi,chars
			mov dword [pcX],0
			mov dword [pcY],9
	.frl2:		push ecx
			lodsw
			push esi
			call _putchar2
			pop esi
			add byte [pcX],6
			pop ecx
			loop .frl2

	.frnem:

			cmp byte [counter],4
			jz .frnem2

			mov al,[xpos]
			mov [pcX],al
			mov ecx,mblurphases
			mov esi,mblur
	.frl1:		push ecx
			lodsb
			movsx eax,al
			mov [pcY],eax
			lodsb
			mov [pccolor],al
			push esi
			mov al,[charcode]
 			call _putchar2
			pop esi
			pop ecx
			loop .frl1

	.frnem2:

			ret

[segment dataseg]
align 16
charcode	db	0
xpos		db	0
mblurphases	equ	8
mblur		times	mblurphases*2 db(0) ; posY, color
mblurlast	equ	mblur+(mblurphases-1)*2
chars		db	code_one,0, code_three,6, code_dot,12, code_h,18
counter		db	0
counter2	db	0
tempszar	db	0