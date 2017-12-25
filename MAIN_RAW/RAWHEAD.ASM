;%define DBOUT			; debugging output in bochs
	; this means that video mode is set to 0x13, normal dosmode
	; matrix output is enabled
[segment code32]
;[org 0x100]
[bits 16]

		times	256 db(0x90)	;eliminated by linker

start:

%ifdef DBOUT
		mov ax,0x13
		int 10h
%endif
		cli
		;-- first of all, i copy myself to 0x10000, to be able
		;-- to modify my data
		push cs
		pop ds
		mov ax,0x1000
		mov es,ax
		xor di,di
		xor si,si
		mov cx,16384
		cld
		rep movsw
		;-- then i jump there
		jmp 0x1000:.start2
.start2:
		push cs
		pop ds
		mov ax,0x2000
		mov ss,ax
		mov sp,0xFFF0
		mov ax,cs
		mov [code16r],ax
		mov [jump2+2],ax	;TODO: check these whether they are needed or not
		call _initGDT
		call _setirqs
		mov eax,CR0
		or al,1     ;PE    ; caches and protecion enable
		and eax,0x9FFFFFFF ; CD, NW - normal chache mode
		mov CR0,eax
		jmp code16:clear_label
clear_label:

		mov ax,data32
		mov ds,ax
		mov es,ax
		mov fs,ax
		mov gs,ax
		mov ax,stackselector
		mov ss,ax
		mov sp,0xFFF0
		db 66h
		[bits 32]
		jmp code32sel:start32
		[bits 16]

real_back:

		mov ax,data16
		mov ds,ax
		mov es,ax
		mov fs,ax
		mov gs,ax

		mov eax,CR0
		and al,0xFE
		mov CR0,eax
		jmp far [jump2]
label2:

		mov ax,[code16r]
		mov ds,ax
		mov es,ax
		mov fs,ax
		mov gs,ax

.halt:		jmp .halt

%macro descriptor 4
	%%m:
	%1 equ %%m-GDT
	dw	%3 & 0xFFFF
	dw	%2 & 0xFFFF
	db	(%2 >> 16) & 0xFF
	dw	%4 | ((%3 >> 8) & 0xF00)
	db	%2 >> 24
%endmacro
%macro int_gate 2	;sel,ofs
	dw	%2
	dw	%1
	db	0
	db	0x8E
	dw	0
%endmacro
%macro exception_handlers 0
	%rep 32
	int_gate code32sel,exceptions
	%endrep
%endmacro
align 16
GDT:

descriptor	nullsel,0,0,0
descriptor	code32sel,0,0xFFFF,0x409A
descriptor	code16,0,0xFFFF,0x9A
descriptor	data32,0,1024*1024-1,0xC092	;set G bit, get max limit
descriptor	data16,0,0xFFFF,0x92
descriptor	textvideo,0xB8000,0xFFFF,0x92
descriptor	vgavideo,0xA0000,0xFFFF,0x92
descriptor	stackselector,0x20000,0xFFFF,0x92
GDTlen		equ	$-GDT
GDTnum		equ	4
GDTlimit	dw	GDTlen
GDTbase		dd	0
GDTbase0	dd	GDT
jump1		dw	clear_label
code16r		dw	0
jump2		dw	label2
		dw	0
align 16
IDT:
exception_handlers		;0..1F
int_gate code32sel,_irq0		;20
int_gate code32sel,_keyboard_irq_handler	;21
IDTlen		equ	$-IDT
IDTlimit	dw	IDTlen
IDTbase		dd	IDT

_initGDT:

			mov eax,ds
			shl eax,4
			add [GDTbase0],eax
			mov [_code32a],eax
			mov cx,GDTnum
			mov bx,GDT+8
			mov dx,ax
			shr eax,16
	.igl1:		mov [bx+2],dx
			mov [bx+4],al
			mov [bx+7],ah
			add bx,8
			loop .igl1

			mov eax,ds
			shl eax,4
			add eax,GDT
			mov [GDTbase],eax

			lgdt [GDTlimit]

			mov eax,cs
			shl eax,4
			add [IDTbase],eax
			lidt [IDTlimit]

			ret

_setirqs:

			;redirecting IRQ0..F -> int20..2F
			mov al,0x11
			out 0x20,al
			mov al,0x20
			out 0x21,al
			mov al,4
			out 0x21,al
			mov al,1
			out 0x21,al

			mov al,0x11
			out 0xA0,al
			mov al,0x28
			out 0xA1,al
			mov al,2
			out 0xA1,al
			mov al,1
			out 0xA1,al

			; enable irq0 and irq1
			mov al,0xFC
			out 0x21,al
			mov al,0xFF
			out 0xA1,al

			; "PIT setup"	- timer
			mov al,0x34
			out 0x43,al
			xor al,al
			out 0x40,al
			out 0x40,al

			ret


[segment code32]
[bits 32]

start32:

			mov esi,szoveg0
			call _writeasciiz
			mov eax,0xC1CAFA52
			call _writehexdw

			call _keyboard_init

			sti

;			mov dx,0x60
;			mov al,0xED
;			out dx,al
;			mov al,0x07
;			out dx,al

			call _demomain
.phalt:			jmp .phalt

;			db 66h
;			[bits 16]
;			jmp code16:real_back

exceptions:

			mov ax,data32
			mov ds,ax
			mov esi,szoveg1
			call _writeasciiz

			cli
			hlt	;an enemy might wake me up, but who cares

_irq0:
			push eax
			push ds

			mov ax,data32
			mov ds,ax
			inc dword [tickcount]

;			mov ax,textvideo
;			mov ds,ax
;			inc byte [ds:0]
;			mov ax,vgavideo
;			mov ds,ax
;			inc byte [ds:0]

			mov al,0x20
			out 0x20,al

			pop ds
			pop eax
			iretd


global	_code32a
szoveg0			db 'This should be in protected mode: ',0
szoveg1			db 'system halted - exception ',0
tickcount		dd	0
_code32a		dd	0
%include	"txt.inc"
%include	"keyboard.inc"
%include	"tloader.inc"
%include	"memory.inc"
%include	"screen.inc"
%include	"demomain.inc"
times     5*1024+800 db(0)
