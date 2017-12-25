%include "..\segments.inc"
%include "util.inc"
[segment codeseg]
[bits 32]

extern	tl_template,tl_templatelen,tl_data

global	_giveresource

;in:	esi	- offset of identifier string (pathname)
;out:	esi	- offset of resource
;	carry flag set if resource found
_giveresource:

			mov [identifier],esi
			mov [nexttoken],esi

			mov eax,[tl_template]
			mov [chunk],eax
			mov eax,[tl_templatelen]
			mov [chunklen],eax

	.grl:		mov esi,[nexttoken]
			call _gettoken
			mov [tokentype],dl
			call _findchunk
			jnc .grnotfound
			cmp byte [tokentype],0
			jz .grl
			mov esi,[chunk]
			stc
			ret

	.grnotfound:	clc
			ret

;in:	esi	- string offset
;out:	dl	- token type:  0 - directory ; 1 - resource
_gettoken:
	
			xor dl,dl
			mov [token],esi
			; find '\' before #0
			mov bh,'\'
			xor bl,bl
	.gtl0:		lodsb
			cmp al,bh
			jz .gtfound
			cmp al,bl
			jnz .gtl0
			; directory not found - it should be a resource
			mov dl,1
	.gtfound:	mov [nexttoken],esi
			ret

; finds the child chunk of the chunk described by chunk and chunklen
;  having the name of string from token to nexttoken-1
; out: 	carry flag set if chunk found
_findchunk:

			mov edx,[chunk]
			mov ebp,[chunklen]
			add ebp,edx
	.fcl0:		mov edi,[token]
			lea esi,[edx+5]
	.fcl1:		mov al,[esi]
			cmp al,[edi]
			jnz .fcnotfound
			cmp al,0
			jz .fcfound
			inc esi
			inc edi
			jmp .fcl1
	.fcfound:	mov al,[edi]
			cmp al,0
			jz .fcfok
			cmp al,'\'
			jnz .fcnotfound2
	.fcfok:		inc esi
			mov [chunk],esi
			mov eax,[edx+1]
			sub eax,esi
			add eax,edx
			mov [chunklen],eax
			stc
			ret
	.fcnotfound:	cmp al,0
			jz .fcfound
			;- jump to the next child chunk
	.fcnotfound2:
			add edx,[edx+1]
			cmp edx,ebp
			jc .fcl0
			;- chunk not found
			clc
			ret

[segment dataseg]
identifier		dd	0
token			dd	0
nexttoken		dd	0
chunk			dd	0
chunklen		dd	0
tokentype		db	0
headerend		dd	0
text0			db	'next chunk',0
text1			db	'end reached',0