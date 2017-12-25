;  Text 80x50 video mode routines by Helium (Farkas G bor [H]), 1997 summer.
;  -with 32-bit pmode   (for the xm2wav)
;  Microsoft suXX!		ver: 1.02
;	v1.03	- 1999.VII.13.  - writehexw corrected
;
;   _inittextmode		;set 80x50x16 text mode
;   _writec			;write one char
;   _writez			;write ASCIIZ string
;   _writehexdw			;write doubleword as hex
;   _writehexb			;write byte as hex
;   _writedecw			;write word as decimal (unsigned)
;   _writemem80hex ;(out of order) write pentaword floating point as hex
;   _writehexw			;write word as hex

; for NASM version, 1999.XII.23.

%macro debugm1 2
	push edx
	mov eax,%2
	mov dx,%1
	call _writehexdw
	pop edx
%endmacro
%macro debugm2 2
	push edx
	mov eax,%2
	mov dx,%1
	call _writehexdw
	call _keywait
	pop edx
%endmacro
;*******************************************************************************
;       DATA - variables needed
;*******************************************************************************

bytec	db   02h		;color of bytes
dwordc	db   09h		;color of doublewords
worddc  db   07h		;color of words if write as decimal
wordc	db   02h		;color of words if write as hex
pworddc db   07h		;color of pwords if write as decimal
hext	db   '0123456789ABCDEF'
cbuffer	times 21 db 0

;*******************************************************************************
;  Inittextmode
;*******************************************************************************
global _inittextmode
_inittextmode:

		  push ES
		  pushad
		  mov word [v86r_ax],3
		  mov AX,10h
		  int 33h		; set the 80*25
		  mov word [v86r_ax],1112h
		  mov word [v86r_dx],0
		  mov word [v86r_bx],0800h
		  mov word [v86r_cx],256
		  mov AX,10h
		  int 33h		; set chars to 8x8

	          popad
		  pop ES
		  ret


;*******************************************************************************
;  one char write
;	GS- kell!!
;	DL- X
;	DH- Y
;	CX- char
;*******************************************************************************
global _writec
_writec:
		
		pushad
		push DX
		xor EAX,EAX	
		mov BX,160
		mov AL,DH
		mul BX
		pop DX
		and DX,00FFh
		shl DX,1
		add AX,DX
		mov word [GS:eax+0b8000h],CX

		popad

		ret

;*******************************************************************************
;  ASCIIZ write		(with colors)
;	EDI- offset (with DS)
;	GS- kell!
;	DL- X
;	DH- Y
;	CH- color       (command in string:  01,color  -set color) 
;*******************************************************************************
global _writez

_writez:

		pushad

		push DX
		xor EAX,EAX
		mov BX,160
		mov AL,DH
 		mul BX
		pop DX
		and DX,00FFh
		shl DX,1
		add AX,DX	

		mov CL,[DS:EDI]
	.loop1: mov word [GS:EAX+0b8000h],CX
		inc EAX
		inc EAX
		inc EDI
		mov CL,[DS:EDI]
		cmp CL,1
		jnz .more1
		inc EDI
		mov CH,[DS:EDI]
		inc EDI
		mov CL,[DS:EDI]
		jmp .loop1
	.more1:
		cmp CL,0
		jnz .loop1

	
		popad
		ret

;*******************************************************************************
;  Write doubleword as hex
;	IN:  EAX: number to write
;	     DL:  X
;	     DH:  Y
;*******************************************************************************
global _writehexdw

_writehexdw:

		pushad
		
		mov EDI,hext

		mov ECX,8

	.lwdw:  push ECX
		mov EBX,EAX
		and EBX,0F0000000h
		shr EBX,28
		mov CL,[DS:EDI+EBX]
		mov CH,[dwordc]			;doublewords' color
		call _writec
		shl EAX,4
		inc DL
		
		pop ECX
		loop .lwdw

		popad
		ret

;*******************************************************************************
;  Write byte as hex
;	IN:	AL	byte to write
;		DL	X
;		DH	Y
;*******************************************************************************
global _writehexb
_writehexb:

		pushad

		xor EBX,EBX
		mov EDI,hext
		mov BL,AL
		shr BL,4
		mov CL,[DS:EDI+EBX]
		mov CH,[bytec]				;bytes' color
		call _writec
		inc DL
		mov BL,AL
		and BL,0Fh
		mov CL,[DS:EDI+EBX]
		call _writec

		popad	

		ret

;*******************************************************************************
;  WRITEDECW procedure 1998.1.30
;  IN:	AX	value
;	DL	X
;	DH	Y
;*******************************************************************************
;cbuffer	 db 21 dup(0)
global _writedecw
_writedecw:
			pushad

			xor ECX,ECX
			mov dword [cbuffer],ECX
			mov dword [cbuffer+4],ECX	;clear buffer
			mov EDI,cbuffer

			push DX
			mov BX,10
			xor CX,CX
	.wdd2:		xor DX,DX
			div BX
			add DL,48
			mov [EDI],DL
			inc EDI
			inc CX
			cmp AX,0
			jnz .wdd2
			pop DX

			mov EDI,cbuffer
			add EDI,ECX
			dec EDI
			mov BH,[worddc]
	.wdd3:		push CX
			mov CH,BH
			mov CL,[EDI]
			call _writec
			dec EDI
			inc DL
			pop CX
			loop .wdd3


			popad
			ret

;*******************************************************************************
;  WRITEMEM80DEC procedure
;  IN:	EDI	offset of pentaword
;	DL	X
;	DH	Y
;*******************************************************************************
;_writemem80hex		proc
;			pushad
;
;			mov CH,pworddc
;			xor AH,AH
;			mov AL,[EDI+9]
;			bt AX,7
;			jnc .wm8h1
;			mov CL,'-'
;			call _writec
;			inc DL			;write sign
;	.wm8h1:
;			mov CL,[EDI+7]
;			shr CL,7
;			add CL,48
;			call _writec
;			inc DL			;write first digit
;
;			mov CL,'.'
;			call _writec
;			inc DL			;write '.'
;
;			mov BL,dwordc
;			mov EAX,dword ptr [EDI+4]
;			and EAX,80000000h	;clear 31st bit
;			mov dwordc,CH
;			call _writehexdw
;			add DL,8
;			mov EAX,dword ptr [EDI]
;			call _writehexdw
;			add DL,9
;			movzx EAX,word ptr [EDI+8]
;			and AX,8000h		;clear 15th bit
;			call _writehexdw
;			mov dwordc,BL
;
;			popad
;			ret
;_writemem80hex		endp

;*******************************************************************************
;  WRITEHEXW procedure (1998.II.7)
;  IN:	AX	value
;	DL	X
;	DH	Y
;*******************************************************************************
global _writehexw
_writehexw:
			pushad

			xor EBX,EBX
			mov ECX,4
	.whwl1:		push ECX
			mov BX,AX
			and BX,0F000h
			shr BX,12
			mov CL,[hext+EBX]
			mov CH,[wordc]
			call _writec
			inc DL
			shl AX,4
			pop ECX
			loop .whwl1

			popad
			ret
