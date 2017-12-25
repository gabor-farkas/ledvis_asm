%include	"../segments.inc"
%include	"../macros.inc"
global	_scrplay_initialize,_scrplay_destroy,_scrplay_step,_scrplay_render

%ifdef WIN32
	extern	_GetTickCount@0
%endif
extern	_debugeax
extern	_allocmem,_freemem
extern	kezdorand

%include	"../eff_ext.inc"
%include	"../script"
[segment codeseg]
[bits 32]

_scrplay_initialize:

			mov eax,SSsize
			call _allocmem
			mov [scriptstack],eax

			ret
_scrplay_destroy:

			mov eax,[scriptstack]
			call _freemem

			ret
_scrplay_step:

			cmp dword [cd_counter],0
			jnz .ss0
			mov eax,.back_address
			jmp [script_address]
	.back_address:	mov [script_address],eax
	.ss0:
			mov esi,step_proc
	.ssl1:		cmp dword [esi],0
			jz .ssend
			push esi
			call [esi]
			pop esi
			add esi,byte 4
			jmp .ssl1
	.ssend:

			dec dword [cd_counter]

			ret

_scrplay_render:

			mov esi,scr_proc
	.srl1:		cmp dword [esi],0
			jz .srend
			push esi
			call [esi]
			pop esi
			add esi,byte 4
			jmp .srl1
	.srend:

			ret

_nullhandler:
			ret

_addstep:
			mov esi,step_proc
			call _addtolist
			ret
_addscreen:
			mov esi,scr_proc
			call _addtolist
			ret
_removstep:
			mov esi,step_proc
			call _removfromlist
			ret
_removscreen:
			mov esi,scr_proc
			call _removfromlist
			ret

_addtolist:

			xor ebx,ebx
	.atl1:		cmp [esi],ebx
			jz .atlfound
			add esi,byte 4
			jmp .atl1
	.atlfound:	mov [esi],eax

			ret

_removfromlist:

	.rfl1:		cmp [esi],eax
			jz .rflfound
			add esi,byte 4
			jmp .rfl1
	.rflfound:	xor eax,eax
			mov [esi],eax

			ret

_rnd2:			
			push eax
%ifdef WIN32
			invoke _GetTickCount@0
%else
			xor al,al
			out 0xD8,al
			in al,0xC4
			mov ah,al
			in al,0xC4
%endif
			mov [kezdorand],ax
			pop eax
			call _random
			ret

%ifdef WIN32
_sync:
		ret
%else
extern _dma_restart
_sync:

             mov dx,0x1FF
             mov al,0        ;"hard" sync
             out dx,al
             call _dma_restart
             mov dx,0x1FF
             mov al,7
             out dx,al

             ret
%endif


[segment dataseg]
step_proc		times	16 dd (0)
scr_proc		times	16 dd (0)
cd_counter		dd	0
script_address		dd	_script
scriptstack		dd	0
SSsize			equ	16384
