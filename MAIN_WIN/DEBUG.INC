[segment text]
[bits 32]

extern _MessageBoxA@16

global	_debugeax

_debugeax:
			pushad

			mov edx,eax
			mov ecx,8
			mov edi,ddstring+2
			mov ebx,hextab
	.dal1:		mov eax,edx
			shr eax,28
			xlatb
			stosb
			shl edx,4
			loop .dal1

			invoke _MessageBoxA@16,byte 0,dword ddstring,dword ddstring,byte 0

			popad
			ret

[segment .data]
hextab		db	'0123456789ABCDEF',0
ddstring	db	'0x00000000',0