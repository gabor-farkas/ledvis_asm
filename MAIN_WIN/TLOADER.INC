; template loader for win32
[segment text]
[bits 32]
extern	_CreateFileA@28
extern	_SetFilePointer@16
extern	_ReadFile@20
extern	_CloseHandle@4
OF_READ		equ	0
FILE_BEGIN	equ	0
FILE_END	equ	2

global	tl_template,tl_templatelen,tl_data,tl_datalen
extern	_lzw_uncompress,lzw_source,lzw_slen,lzw_destination

_tl_loadtemplate:

			invoke _CreateFileA@28,dword filename, dword 0x100001, byte 0,byte 0,byte 3,byte 0,byte 0
			mov [fhandle],eax
			invoke _ReadFile@20,dword [fhandle],dword tl_templatelen,byte 4,dword temp,byte 0
			mov eax,[tl_templatelen]
			call _allocmem
			mov [tl_template],eax
			invoke _ReadFile@20,dword [fhandle],dword [tl_template],dword [tl_templatelen],dword temp,byte 0
			invoke _ReadFile@20,dword [fhandle],dword datatemplen,byte 4,dword temp,byte 0
			invoke _ReadFile@20,dword [fhandle],dword tl_datalen,byte 4,dword temp,byte 0
			mov eax,[datatemplen]
			call _allocmem
			mov [datatemp],eax
			mov eax,[tl_datalen]
			call _allocmem
			mov [tl_data],eax
			invoke _ReadFile@20,dword [fhandle],dword [datatemp],dword [datatemplen],dword temp,byte 0
			invoke _CloseHandle@4,dword [fhandle]

			mov eax,[datatemp]
			mov [lzw_source],eax
			mov eax,[datatemplen]
			mov [lzw_slen],eax
			mov eax,[tl_data]
			mov [lzw_destination],eax
			call _lzw_uncompress

			mov eax,[datatemp]
			call _freemem


			ret

[segment .data]
filename		db	'data\images.fls',0
fhandle			dd	0
tl_template		dd	0
tl_templatelen		dd	0
tl_data			dd	0
tl_datalen		dd	0
datatemp		dd	0
datatemplen		dd	0
temp			dd	0
