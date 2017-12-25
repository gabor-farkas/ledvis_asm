; template loader for dos
%include "files.inc"
%include "txt80x50.inc"
%include "crt.inc"

global	tl_template,tl_templatelen,tl_data
global	_debugeax
extern	_lzw_uncompress,lzw_source,lzw_slen,lzw_destination,lzw_destlen

_debugeax:
			pushad
			xor edx,edx
			call _writehexdw
			call _getkey
			popad
			ret

_tl_loadtemplate:

			mov eax,32768
			call _getlomem
			mov [filebuf],eax

			mov dx,code32
			mov eax,filename
			xor ecx,ecx
			call _fileopen
			mov [fhandle],ax
			mov ebx,eax
			mov ecx,4
			mov esi,[filebuf]
			mov edi,tl_templatelen
			call _filelongread
			mov eax,[tl_templatelen]
			call _allocmem
			mov [tl_template],eax
			mov ecx,[tl_templatelen]
			mov esi,[filebuf]
			mov edi,[tl_template]
			call _filelongread
			mov ecx,4
			mov esi,[filebuf]
			mov edi,datatemplen
			call _filelongread
			mov ecx,4
			mov esi,[filebuf]
			mov edi,tl_datalen
			call _filelongread
			mov eax,[tl_datalen]
			call _allocmem
			mov [tl_data],eax
			mov eax,[datatemplen]
			call _allocmem
			mov [datatemp],eax
			mov ecx,[datatemplen]
			mov esi,[filebuf]
			mov edi,[datatemp]
			call _filelongread
			call _fileclose

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

;			mov esi,dd0
;			call _giveresource
;			mov eax,[esi+13]
;			call _debugeax

	.lterror:

			ret

_movesiedi:
			push ecx
			push esi
			push edi
			push eax
			mov eax,ecx
			shr ecx,2
			rep movsd
			mov ecx,eax
			and ecx,3
			rep movsb
			pop eax
			pop edi
			pop esi
			pop ecx

			ret

filename		db	'data\images.fls',0
fhandle			dd	0
tl_template		dd	0
tl_templatelen		dd	0
tl_data			dd	0
tl_datalen		dd	0
datatemp		dd	0
datatemplen		dd	0
filebuf			dd	0
dd0			db	'Mazsy',0
