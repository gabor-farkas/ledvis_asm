; template loader for raw IA32

global	tl_template,tl_templatelen,tl_data
global	_debugeax
extern	_lzw_uncompress,lzw_source,lzw_slen,lzw_destination,lzw_destlen

_tl_loadtemplate:

			call _flashreset

			mov ebx,eax
			mov ecx,4
			mov edi,tl_templatelen
			call _flashread
			mov eax,[tl_templatelen]
			call _allocmem
			mov [tl_template],eax
			mov ecx,[tl_templatelen]
			mov edi,[tl_template]
			call _flashread
			mov ecx,4
			mov edi,datatemplen
			call _flashread
			mov ecx,4
			mov edi,tl_datalen
			call _flashread
			mov eax,[tl_datalen]
			call _allocmem
			mov [tl_data],eax
			mov eax,[datatemplen]
			call _allocmem
			mov [datatemp],eax
			mov ecx,[datatemplen]
			mov edi,[datatemp]
			call _flashread

			;- uncompress
			mov eax,[datatemp]
			mov [lzw_source],eax
			mov eax,[datatemplen]
			mov [lzw_slen],eax
			mov eax,[tl_data]
			mov [lzw_destination],eax
			call _lzw_uncompress

			mov eax,[datatemp]
			call _freemem

	.lterror:

			ret

_flashread:

			mov dx,0x1FF
	.frl1:		in al,dx
                        stosb
			mov al,8
			out dx,al
			xor al,al
			out dx,al
			loop .frl1

			ret

_flashreset:

			mov dx,0x1FF
			mov al,16
			out dx,al
			xor al,al
			out dx,al
			
			mov ecx,32768
	.frl1:      mov al,8
	            out dx,al
	            xor al,al
	            out dx,al
	           loop .frl1

			ret


tl_template		dd	0
tl_templatelen		dd	0
tl_data			dd	0
tl_datalen		dd	0
datatemp		dd	0
datatemplen		dd	0
