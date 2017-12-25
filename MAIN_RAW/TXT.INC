[segment code32]
[bits 32]
; clear protected mode text screen handling routines
;	works console-like
;	needs a selector called textvideo to be initialized
;   started 2004.01.15, instead of preparing for the physics exam

spointer	dd	0
scolor		db	7
suplimit	dd	80*25*2
shextab		db	'0123456789ABCDEF'

global	_writehexdw,_debugeax	;somebody need them symbols

_debugeax:

; in: al, es:edi
_putchar:

			mov ah,[scolor]
			stosw
			cmp edi,[suplimit]
			jnc _scrollup
			ret

_scrollup:
			pushad
			mov ax,es
			mov ds,ax
			xor edi,edi
			mov esi,160
			mov ecx,24*80*2/4
			rep movsd
			xor eax,eax
			mov ecx,80*2/4
			rep stosd
			popad
			mov edi,80*24*2
			ret

; in: ds:esi
_writeasciiz:
			pushad
			cld
			mov ax,textvideo
			mov es,ax
			mov edi,[spointer]
	.waloop:	lodsb
			call _putchar
			or al,al
			jnz .waloop
			mov [spointer],edi
			popad

			ret

; in: eax
_writehexdw:
			pushad
			mov edx,eax
			mov ax,textvideo
			mov es,ax
			mov edi,[spointer]
			mov ecx,8
			mov ebx,shextab
	.whdl1:		mov eax,edx
			shr eax,28
			xlatb
			call _putchar
			shl edx,4
			loop .whdl1
			mov [spointer],edi
			popad

			ret
