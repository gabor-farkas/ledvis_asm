[segment codeseg]
[bits 32]

; --------------------------------------------------
; in:	lzw_source,lzw_slen
;	lzw_destination - allocated buffer
;	lzw_cww	- initial code word width
; out:	lzw_destlen - final byte count
; cww bit width is limited to 24, which gives a limit of 16777216 bytes
; on input files
; dictionary item length is limited to 255
; --------------------------------------------------
global	_lzw_uncompress
extern	_debugeax
_lzw_uncompress:

			xor eax,eax
			mov [lzw_sbp],eax
			mov eax,[lzw_slen]
			lea eax,[eax+2*eax]	;*3
			call _allocmem
			mov [lzw_dict],eax
			mov [lzw_dnext],eax

			mov eax,1
			mov ebx,eax
			mov ecx,[lzw_cww]
			shl ebx,cl
			dec ebx
			mov [lzw_cwwmask],ebx
			not ebx
			mov [lzw_cwwmask2],ebx
			dec ecx
			shl eax,cl
			mov [lzw_dicthidden],eax
			mov [lzw_dindex],eax

			mov edi,[lzw_destination]
	.ucl0:
			call _lzw_getnextcw
			cmp eax,[lzw_dicthidden]
			jnc .uc0
			;- cw not present in dictionary - copied directly
			stosb
			mov byte [lzw_lastcwlen],1
			jmp .uc_ad
	.uc0:		mov ebx,eax
			sub eax,[lzw_dicthidden]
			lea eax,[eax+4*eax]
			add eax,[lzw_dict]
			movzx ecx,byte [eax]
			mov [lzw_lastcwlen],ecx
			mov esi,[eax+1]
			rep movsb
	.uc_ad:		;-- add to dictionary
			mov eax,[lzw_dindex]
			mov cl,[lzw_cww]
			shr eax,cl
			cmp eax,0
			jz .uc1
			;-- inc cww
			inc byte [lzw_cww]
			mov eax,[lzw_cwwmask]
			shl eax,1
			inc eax
			mov [lzw_cwwmask],eax
			shl dword [lzw_cwwmask2],1
	.uc1:		mov esi,[lzw_dnext]
			add dword [lzw_dnext],byte 5
			mov eax,[lzw_lastcwlen]
			neg eax
			lea ebx,[edi+eax]
			neg eax
			inc al
			mov [esi],al
			mov [esi+1],ebx
			inc dword [lzw_dindex]
			mov eax,[lzw_slen]
			shl eax,3
			sub eax,[lzw_sbp]
			cmp eax,[lzw_cww]
			jge .ucl0
;			cmp dword [lzw_dindex],100000
;			jc .ucl0

			mov eax,[lzw_dict]
			call _freemem

			ret

; returns eax
_lzw_getnextcw:

			mov esi,[lzw_sbp]
			mov eax,[lzw_cww]
			add [lzw_sbp],eax
			mov ecx,esi
			shr esi,3
			add esi,[lzw_source]
			and cl,7
			mov eax,[esi]
			shr eax,cl
			and eax,[lzw_cwwmask]

			ret


[segment dataseg]
global	lzw_source,lzw_slen,lzw_destination,lzw_destlen
lzw_source		dd	0
lzw_slen		dd	0
lzw_destination		dd	0
lzw_destlen		dd	0
lzw_cww			dd	9	;word width
lzw_maxdictsize		dd	0
;-
lzw_sbp			dd	0	;source bit position
lzw_dict		dd	0	;structured list:
					; one byte length, dword pointer
lzw_dnext		dd	0
lzw_lastcwlen		dd	0
lzw_dicthidden		dd	0	;the first dict item index
					; that is present
lzw_dindex		dd	0
lzw_cwwmask		dd	0
lzw_cwwmask2		dd	0
