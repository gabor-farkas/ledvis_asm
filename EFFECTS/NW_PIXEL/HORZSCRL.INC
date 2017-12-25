[segment codeseg]
[bits 32]

global _horzscrl

_horzscrl:

			mov bl,[counter]
			mov bh,24
			sub bh,bl
			mov ecx,24
			add ebp,esi
	.hsl1:		push ecx
			mov cl,bl
			add esi,ecx
			mov cl,bh
			or cl,cl
			jz .hs1
			rep movsb
	.hs1:		xchg ebp,esi
			mov cl,bl
			or cl,cl
			jz .hs2
			rep movsb
	.hs2:		mov cl,bh
			add esi,ecx
			xchg ebp,esi
			pop ecx
			loop .hsl1

			ret

[segment dataseg]