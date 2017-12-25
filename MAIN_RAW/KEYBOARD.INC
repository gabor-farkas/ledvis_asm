; following bochs's rombios code
[bits 32]

kwait		equ	0x3FFFF

%macro keymaxloop 4	;count, read mask, out byte, jz or jnz
	mov ecx,%1
%%1:	in al,0x64
	and al,%2
	%4 %%2
	mov al,%3
	out 0x80,al
	loop %%1
%%2:
%endmacro

%macro waitfordata 1	;0x80 output base
	keymaxloop kwait,2,%1,jz
	or ecx,ecx
	keypanic0 %1
	keymaxloop kwait,1,%1+1,jnz
	or ecx,ecx
	keypanic0 %1+1
%endmacro

%macro keypanic0 1
	jnz %%nem
	mov eax,%1
	jmp _keyboard_panic
%%nem
%endmacro
%macro keypanic 1
	jz %%nem
	mov eax,%1
	jmp _keyboard_panic
%%nem:
%endmacro

_keyboard_init:

			;---- flush buffers
			keymaxloop kwait,2,0,jz

			mov ecx,0x2000
	.kil2:		xor al,al
			out 0x80,al
			in al,0x64
			and al,1
			jz .ki1
			in al,0x60
			mov cx,0x2000
	.ki1:		loop .kil2

			mov al,0xAA
			out 0x64,al

			waitfordata 0

			in al,0x60
			cmp al,0x55
			keypanic 0x991

			mov al,0xAB
			out 0x64,al

			waitfordata 0x10

			in al,0x60
			or al,al
			keypanic 0x992

			mov al,0xAE
			out 0x64,al
			mov al,0xA8
			out 0x64,al

			;---
			mov al,0xFF
			out 0x60,al
			waitfordata 0x20
			in al,0x60
			cmp al,0xFA
			keypanic 0x993
			keymaxloop kwait,1,0x31,jnz
			or cx,cx
			keypanic0 0x31
			in al,0x60
			cmp al,0xAA
			keypanic 0x994

			mov al,0xF5
			out 0x60,al
			waitfordata 0x40
			in al,0x60
			cmp al,0xFA
			keypanic 0x995

			mov al,0x60
			out 0x64,al
			keymaxloop kwait,2,0x50,jz
			or cx,cx
			keypanic0 0x50
			mov al,0x61
			out 0x60,al
			keymaxloop kwait,2,0x60,jz
			or cx,cx
			keypanic0 0x60
			mov al,0xF4
			out 0x60,al
			waitfordata 0x70
			in al,0x60
			cmp al,0xFA
			keypanic 0x996

			mov al,0x77
			out 0x80,al

			ret

_keyboard_panic:

			push eax
			mov esi,keypanicstring
			call _writeasciiz
			pop eax
			call _writehexdw

			ret

;-----------------------------
_keyboard_irq_handler:
			push eax

			mov al,0xAD
			out 0x64,al
			mov al,0x0B
			out 0x20,al
			in al,0x20
			and al,0x02
			jz .kihend

			in al,0x60
			push ds
			pushad
			call _keyboard_handler
			popad
			pop ds

			mov al,0x20
			out 0x20,al
	.kihend:
			mov al,0xAE
			out 0x64,al

			pop eax
			iretd

_keyboard_handler:

			mov bx,textvideo
			mov ds,bx
			mov [ds:2],al

			ret

keypanicstring		db	'Keyboard seems not be present . . .',0