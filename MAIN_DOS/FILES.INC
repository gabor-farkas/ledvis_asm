;  FILES.INC by Helium for xm2wav.  (Farkas G bor, 1997 summer Hungary)
;-----------------------------------------------------------------
;			-version 1.05
;			-version 1.1	(groupfile support)
;                                       1999.X.2
;	 _fileopen
;	 _fileclose
;	 _fileread
;	 _fileseek
;	 _filelongread		;corrected (don't freeze by reading 16K)
;				; 1999.XI.28.
;	 _filewrite
;	 _filecreate
;	 _filelongwrite
;	 _getDTAAddress
;	 _findfirst
;	 _findnext
;	 _DTAshadow
;	 _groupadjust		;v1.1
;	 _fileseek2		;v1.2

; for NASM version 1999.XII.23.
;
	DTAoffset	dd	0
	DTAshadow	times	64 db 0
	groupindex      db      0
	seekadjust      dd      0
	flr_buf		dd	0
	flr_size	dd	0
	flr_where	dd	0
	flr_handle	dw	0

;*************************************************
;   FILEOPEN procedure
; IN:   DX:AX ASCIIZ filename's address	(segment:offset)
;       CL:   0 read
;             1 write
;             2 read/write
; OUT:  AX:  filehandle or error code
;       CF:  set on error
;*************************************************
global _fileopen

_fileopen:

		push ES
		mov word [v86r_ax],3D00h
		mov [v86r_dx],AX
		mov [v86r_al],CL
		mov byte [v86r_cl],0
		mov [v86r_ds],DX
		mov AX,21h
		int 33h
		mov AX,[v86r_ax]
		push AX
		pop AX
		pop ES
		mov dword [seekadjust],0

		ret

;***************************************************
;   CLOSE FILE
;   IN: BX: File handle
;   OUT:  AX destroyed or error code
;         CF set on error
;***************************************************
global _fileclose

_fileclose:
		
		push ES
		mov word [v86r_ax],3E00h
		mov word [v86r_bx],BX
		mov AX,21h
		int 33h
		mov AX,word [v86r_ax]
		pop ES

		ret

;***************************************************
;   READ FROM FILE
;	IN
;		BX	file handle
;		CX 	number of bytes to read
;		DX:AX	buffer
;	OUT
;		CF	set on error
;		AX	number of bytes actually read / error code
;	     					        5 - Acces denied
;						        6 - Invalid file handle
;***************************************************
global _fileread

_fileread:

		push ES
		mov word [v86r_ax],3F00h
		mov [v86r_bx],BX
		mov [v86r_cx],CX
		mov [v86r_ds],DX
		mov [v86r_dx],AX
		mov AX,21h
		int 33h
		mov AX,[v86r_ax]
		pop ES

		ret

;*******************************************************************************
;  SET FILE POSITION
;   IN	BX=filehandle
;       EAX=new pos
;   OUT CF set on error
;	AX= error code
;				1 - function number invalid
;				6 - invalid handle
;*******************************************************************************
global _fileseek, _fileseek2

_fileseek:

		pushad
		mov word [v86r_ax],4200h
		mov [v86r_bx],BX
		add EAX,[seekadjust]
		mov ECX,EAX
		shr ECX,16
		mov [v86r_cx],CX
		mov [v86r_dx],AX
		mov AX,21h
		int 33h
		popad
		mov AX,[v86r_ax]

		ret
;IN: CL-mode
;OUT:  eax - dx:ax return
_fileseek2:

		pushad
		mov byte [v86r_ah],42h
		mov [v86r_al],cl
		mov [v86r_bx],bx
		add eax,[seekadjust]
		mov ecx,eax
		shr ecx,16
		mov [v86r_cx],cx
		mov [v86r_dx],ax
		mov ax,21h
		int 33h
		popad
		mov ax,[v86r_dx]
		shl eax,16
		mov ax,[v86r_ax]
		ret

;*******************************************************************************
;  FILELONGREAD procedure (1998.II.6)
;  IN:	ECX	size
;	BX	filehandle
;	ESI	16K reading buffer in lomem
;	EDI	where
;*******************************************************************************
;flr_size	dd	?
;flr_buf	dd	?
;flr_where	dd	?
;flr_handle	dw	?
global _filelongread
_filelongread:
			pushad

			mov [flr_size],ECX
			mov [flr_buf],ESI
			mov [flr_where],EDI
			mov [flr_handle],BX

			xor EDX,EDX
		 	mov EAX,ECX
			mov EBX,16384
			div EBX
			push EDX

			mov ECX,EAX
			mov BX,[flr_handle]
			mov EDX,ESI
			add edx,[_code32a]
			mov EAX,EDX
			shr EDX,4
			and AX,15
			cmp ECX,0
			jz .flr1
	.flrl1:		push ECX
			mov ECX,16384
			push AX
			call _fileread
			pop AX
			mov ECX,16384
			call _movesiedi
			add EDI,16384
			pop ECX
			loop .flrl1
	.flr1:
			pop ECX
			cmp ECX,0
			jz .flr2
			call _fileread
			call _movesiedi
	.flr2:

			popad
			ret

;*******************************************************************************
;  FILEWRITE procedure
;  IN: BX,CX,DX:AX
;*******************************************************************************
global _filewrite
_filewrite:
			pushad

			mov word [v86r_ax],4000h
			mov [v86r_cx],CX
			mov [v86r_bx],BX
			mov [v86r_ds],DX
			mov [v86r_dx],AX
			mov AX,21h
			int 33h

			popad
			ret

;*******************************************************************************
;  FILECREATE procedure
;  IN:	CX attributes
;	DX:AX filename
;  OUT:	AX handle
;*******************************************************************************
global _filecreate
_filecreate:

			push ES
			mov [v86r_cx],CX
			mov [v86r_ds],DX
			mov [v86r_dx],AX
			mov word [v86r_ax],3C00h
			mov AX,21h
			int 33h
			mov AX,[v86r_ax]
			pop ES

			ret

;*******************************************************************************
;  FILELONGWRITE procedure (1999.VII.8)
;  IN:	ECX	size
;	BX	filehandle
;	EDI	16K reading buffer in lomem
;	ESI	source
;*******************************************************************************
global _filelongwrite
_filelongwrite:
			pushad

			mov [flr_size],ECX
			mov [flr_buf],EDI
			mov [flr_where],ESI
			mov [flr_handle],BX
;
;			xor EDX,EDX
;		 	mov EAX,ECX
;			mov EBX,16384
;			div EBX
;			push EDX
;
;			mov ECX,EAX
;			mov BX,flr_handle
;			mov EDX,EDI
;			mov EAX,EDX
;			shr EDX,4
;			and AX,15
;			cmp ECX,0
;			jz .flw1
;	.flwl1:		push ECX
;			mov ECX,16384
;			call _movesiedi
;			add ESI,16384
;			mov CX,16384
;			push AX
;			call _filewrite
;			pop AX
;			pop ECX
;			loop .flwl1
;	.flw1:
;			pop ECX
;			call _movesiedi
; 			call _filewrite

			mov ECX,16384
			call _movesiedi
			mov EAX,EDI
			mov EDX,EAX
			shr EDX,4
			and EAX,15
			mov ECX,16384
			mov BX,[flr_handle]
			call _filewrite

			popad
			ret

;*******************************************************************************
;  GETDTAADDRESS procedure	(1999.VII.19)
;  OUT:  GS:DTAoffset		address
;*******************************************************************************
global _getDTAAddress
_getDTAAddress:
			push EBX
			push EAX

			mov byte [v86r_ah],2Fh
			mov AX,21h
			int 33h
			movzx EAX,word [v86r_es]
			movzx EBX,word [v86r_bx]
			shl EAX,4
			add EAX,EBX
			mov [DTAoffset],EAX

			pop EAX
			pop EBX
			ret

;*******************************************************************************
;  FINDFIRST procedure		(1999.VII.19)
;  IN:	DS:[EDI]	ASCIIZ path and filename
;	CX		attributes
;  OUT:	DTA		info about found file
;	AX		error code (2 = no such file, 18 = no more files)
;*******************************************************************************
global _findfirst
_findfirst:

			mov AX,code32
			mov [v86r_ds],AX
			mov [v86r_dx],DI
			mov [v86r_cx],CX
			mov byte [v86r_ah],4Eh
			mov AX,21h
			int 33h
			mov AX,[v86r_ax]

			ret

;*******************************************************************************
;  FINDNEXT procedure		(1999.VII.19)
;  OUT:	AX	error code	(18 = no more files)
;*******************************************************************************
global _findnext
_findnext:

			mov byte [v86r_ah],4Fh
			mov AX,21h
			int 33h
			mov AX,[v86r_ax]

			ret

;*******************************************************************************
;  DTASHADOW procedure		(1999.VII.19)
;*******************************************************************************
global _DTAshadow
_DTAshadow:
			pushad

			mov ECX,43
			mov ESI,[DTAoffset]
			mov EDI,DTAshadow
			add EDI,[_code32a]
			call _movesiedi

			popad
			ret

;*******************************************************************************
;  GROUPADJUST procedure
;  IN:	groupindex [0..254]
;	BX	filehandle
;*******************************************************************************
global _groupadjust
_groupadjust:

			movzx EAX,byte [groupindex]
			inc EAX
			shl EAX,4
			add EAX,12
			call _fileseek
			mov DX,code32
			mov EAX,seekadjust
			mov CX,4
			call _fileread
			mov EAX,0
			call _fileseek

			ret
