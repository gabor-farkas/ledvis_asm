[segment code32]
[bits 32]
MEMSIZE		equ	30*1024*1024
MEMABSBASE	equ	1024*1024+64*1024	;64k for stack

global	_allocmem,_freemem
global	_code32a

_allocmem:

	;some memory management would be needed here

			mov [blocklen],eax
			pushad
			mov esi,[mainmemory]
			; find the end of the block that can be
			; allocated here
	.am0		mov ecx,maxblocks
			mov ebx,[memblocks]
			mov ebp,ebx
			xor edx,edx
			dec edx
	.aml1:		cmp esi,[ebx]
			ja .am1
			mov eax,[ebx]	;a block found above esi
			sub eax,esi
			cmp eax,edx
			jnc .am1	;a nearer block already found
			or eax,eax
			jnz .am2	;a block found exactly at esi
			xor edx,edx
			mov ebp,ebx
			jmp .aml1end
	.am2:		mov edx,eax	;store this as the nearest block
			mov ebp,ebx
	.am1:		add ebx,byte 8
			loop .aml1	; and go on
			cmp edx,-1
			jnz .aml1end
			mov edx,[memoryend]	;no block found at all
			sub edx,esi
			xor ebp,ebp
	.aml1end:	cmp edx,[blocklen]
			jnc .am3
			; this block is not large enough
			or ebp,ebp
			jnz .am4
			;- panic - out of memory
			cli
			hlt
	.am4:		mov esi,[ebp]
			add esi,[ebp+4]	;jump to the end of the nearest block
			jmp .am0
	.am3:		; a fitting block found
			; maybe we should check all free block to find
			; the most fitting one
			; - now we store this block in our table
			mov ebx,[memblocks]
			xor eax,eax
			mov ecx,maxblocks
	.aml2:		cmp [ebx],eax
			jz .aml2end
			add ebx,byte 8
			loop .aml2
			; - panic - ran out of block entries
			cli
			hlt
	.aml2end:	mov [ebx],esi
			mov [blockofs],esi
			mov eax,[blocklen]
			mov [ebx+4],eax
			; -- fill it up with zeroes
			mov edi,esi
			mov ecx,[blocklen]
			xor eax,eax
			cld
			rep stosb

			popad
			mov eax,[blockofs]
			ret

_freemem:
			pushad
			mov esi,[memblocks]
			mov ecx,maxblocks
	.fml1:		cmp eax,[esi]
			jz .fm1
			add esi,byte 8
			loop .fml1
			;-- panic - block not found
			jmp .fmend
	.fm1:		xor eax,eax
			mov [esi],eax
			mov [esi+4],eax

	.fmend:
			popad
			ret

_initmem:

			mov dword [memoryend],MEMSIZE
			mov eax,MEMABSBASE
			sub eax,[_code32a]
			mov [memblocks],eax
			add [memoryend],eax
			add eax,maxblocks*8
			mov [mainmemory],eax
			mov edi,[memblocks]
			xor eax,eax
			mov ecx,maxblocks*4
			rep stosd

			ret

mainmemory		dd	0
memoryend		dd	0
blocklen		dd	0
blockofs		dd	0
maxblocks		equ	256
memblocks		dd	0	;offset, length |
					; ofs is 0 for empty blocks