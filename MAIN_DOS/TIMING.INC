irq_stub_buf	times	21 db(0)
counter		dd	0
irq_edx		dd	0
irq_eax		dd	0

_timer_initialize:

		mov bl,0
		call [_getirqvect]
		mov [irq_edx],edx
		mov edx,_timer_interrupt
		call [_setirqvect]
		mov edi,irq_stub_buf
		call _rmpmirqset
		mov [irq_eax],eax

		ret

_timer_destroy:

		xor ebx,ebx
		mov eax,[irq_eax]
		call _rmpmirqfree
		mov edx,[irq_edx]
		call [_setirqvect]

		ret

_timer_interrupt:
		push eax
		push ds
		mov ds,[cs:_seldata]
		inc dword [counter]
		pop ds
		mov al,0x20
		out 0x20,al
		pop eax
;		sti
		iretd
