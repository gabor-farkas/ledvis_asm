[segment codeseg]
[bits 32]

global	_decode_animations
extern	tl_template,tl_templatelen,tl_data
extern	_allocmem,_freemem
extern	_debugeax

; recursively iterate through all the animations and decode their content
_decode_animations:

			mov esi,[tl_template]
			mov ebp,[tl_templatelen]
			add ebp,esi

			call _processchunk

			ret

_processchunk:

.goon:			lodsb		;get chunk type
			cmp al,2
			jz .anim
			cmp al,0
			jnz .notdir
			;- this is a folder, process its contents
			lodsd
			push esi
			push eax
			push ebp
			lea ebp,[esi+eax-5]
			call _findcontentstart
			call _processchunk
			pop ebp
			pop eax
			pop esi
			jz near .endcheck
.notdir:		;- not a folder, not an animation,
			;- we just skip it
			lodsd
			jmp .endcheck
.anim:			;- animation found
			lodsd
			push esi
			push eax
			push ebp
			call _findcontentstart
			mov ebp,esi
			mov eax,[ebp+4]
			mul dword [ebp+8]	;w*h
			mov ecx,eax
			lea ebx,[eax-1]
			mul dword [ebp]		;count*w*h
			call _allocmem
			mov [tempmem],eax
			mov edi,eax
			mov esi,[ebp+13]
			add esi,[tl_data]
			mov eax,ecx
	.dl1:		push edi
			push ecx
			mov ecx,[ebp]	;count
	.dl2:		;movsb
			lodsb
			cmp al,0x3F
			jc .x
			mov al,0x3F
		.x:
			stosb
			add edi,ebx
			loop .dl2
			pop ecx
			pop edi
			inc edi
			loop .dl1
			mov edi,[ebp+13]
			add edi,[tl_data]
			mov esi,[tempmem]
			mov eax,[ebp]
			mul dword [ebp+4]
			mul dword [ebp+8]
			mov ecx,eax
			shr ecx,2
			rep movsd
			mov eax,[tempmem]
			call _freemem
			pop ebp
			pop eax
			pop esi
.endcheck:		lea esi,[esi+eax-5]
			cmp esi,ebp
			jc .goon

			ret

_findcontentstart:

	.fcsl1:		lodsb		;asciiz
			or al,al
			jnz .fcsl1

			ret

[segment dataseg]
tempmem			dd	0