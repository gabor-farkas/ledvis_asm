; modified,
;2004.01.07

;  CRT.INC by Helium ,1997 summer, Hungary.   (Farkas G bor) (v1.06)
;
;	_keywait (v1.04 - corrected , ES to stack)
;	_setnormaltextmode
;	_speakeron
;	_speakeroff
;	_beep
;	_getdosmem
;	_keypressed
;	_getkey
;	_movesiedi
;	_filledi_d
;	_clrkeybuf	(v1.05)
;	_beep2		(v1.06)

;  for NASM version 1999.XII.23.


;*******************************************************************************
;  KEYWAIT procedure
;*******************************************************************************
global _keywait
_keywait:

		push AX
	 	mov word [v86r_ax],0
		mov AL,16h
		int 33h
		pop AX

		ret

;*******************************************************************************
;  SETNORMALTEXTMODE procedure
;*******************************************************************************
global _setnormaltextmode
_setnormaltextmode:

			push ES
			push AX
			mov word [v86r_ax],3
			mov AX,10h
			int 33h
			pop AX
			pop ES

			ret

;*******************************************************************************
;    SPEAKERON procedure
;	EBX	Hertz
;*******************************************************************************
global _speakeron
_speakeron:

		pushad

		mov AL,10110110b
		out 43h,AL

		mov EAX,1193180
		xor EDX,EDX
		div EBX
		out 42h,AL
		mov AL,AH
		out 42h,AL

		in AL,61h
		or AL,00000011b
		out 61h,AL

		popad

		ret

;*******************************************************************************
;  SPEAKEROFF procedure
;*******************************************************************************
global _speakeroff
_speakeroff:

		push AX
		in AL,61h
		and AL,11111100b
		out 61h,AL
		pop AX

		ret

;*******************************************************************************
;  BEEP procedure
;*******************************************************************************
global _beep

_beep:

		push EAX
		push ECX

		mov EBX,300
		call _speakeron
		mov ECX,0fffffh
	.w1:	loop .w1
		call _speakeroff
	
		pop ECX
		pop EAX
		ret

;*******************************************************************************
;  GETDOSMEM procedure
;     IN
;	 AX	number of "paras" to allocate
;     OUT
;	 AX	segment	
;*******************************************************************************
global _getdosmem

_getdosmem:

		push ES
		mov word [v86r_ax],4800h
		mov word [v86r_bx],AX
		mov AX,21h
		int 33h
		mov AX,word [v86r_ax]
		pop ES

		ret

;*******************************************************************************
;  KEYPRESSED procedure
;  OUT	 ZF set if no keystroke available
;	 ZF clear if keystroke available
;		AH BIOS scan code
;		AL ASCII code
;*******************************************************************************
global _keypressed

_keypressed:

		push ES
		mov word [v86r_ax],100h
		mov AX,16h
		int 33h
		mov AX,[v86r_ax]
		pop ES
	
		ret

;*******************************************************************************
;  GETKEY procedure
;  OUT	AH BIOS scan code
;	AL ASCII code
;*******************************************************************************
global _getkey

_getkey:

		push ES
		mov word [v86r_ax],0
		mov AX,16h
		int 33h
		mov AX,[v86r_ax]
		pop ES

		ret

;*******************************************************************************
;  FILLEDI_D procedure (1999.VII.8)
;*******************************************************************************
global _filledi_d
_filledi_d:
			push ES
			push EBX

			mov BX,GS
			mov ES,BX
			cld
			rep stosd

			pop EBX
			pop ES
			ret


;*******************************************************************************
;  CLRKEYBUF procedure
;*******************************************************************************
global _clrkeybuf
_clrkeybuf:

	.crkbl1:	call _keypressed
			jz .crkblend
			call _keywait
			jmp .crkbl1
	.crkblend:

			ret

;*******************************************************************************
;  BEEP2 procedure
;*******************************************************************************
global _beep2

_beep2:

		push EAX
		push ECX

		mov EBX,500
		call _speakeron
		mov ECX,0fffffh
	.w1:	loop .w1
		call _speakeroff
	
		pop ECX
		pop EAX
		ret

