;
;	computes sine/cosine table with 65536 elements each
;	uses Taylor method, as
;	 sin x = sum[k=0]((-1)^k * x^(2k+1) / (2k+1)!)
;	 cos x = sum[k=0]((-1)^k * x^(2k) / (2k)!)
;		(0! = 1)
;	computes sine and cosine to 45deg (8192 values) and then
;	fills up the remaining area using these values
;	computes values are 1.31 fixed point signed values, thus
;	-1 is 2^31 and 2^31-1 is almost +1, and zero is zero ofcoz
;
;
[segment codeseg]
[bits 32]

global	_initsines,_destsines,sinetab,cosinetab

_initsines:

			mov eax,16384*5*4
			call _allocmem
			mov [sinetab],eax
			add eax,16384*4
			mov [cosinetab],eax

			call _countsines

			ret

_destsines:

			mov eax,[sinetab]
			call _freemem

			ret

_countsines:

			; compute sines from 0 to 45 deg
			mov ecx,8192
			mov edi,[sinetab]
	.csl1:		push ecx
			mov eax,8192
			sub eax,ecx	;positive values only
			mov ebx,0xC90FDAA2	;pi/4*(2^32)
			mul ebx
			mov ebx,8191
			div ebx
			mov ecx,maxtermpairs
			mov ebx,fact_sine
			mov ebp,eax
			xor esi,esi
	.csl2:		add esi,eax
			mul ebp
			mov eax,edx
			mul ebp
			mov eax,edx
			xor edx,edx
			div dword [ebx]
			sub esi,eax
			mul ebp
			mov eax,edx
			mul ebp
			mov eax,edx
			xor edx,edx
			div dword [ebx+4]
			add ebx,byte 8
			loop .csl2
			mov eax,esi
			shr eax,1
			stosd
			pop ecx
			loop .csl1

			; compute cosines from 0 to 45 deg
			mov ecx,8192
			mov edi,[sinetab]
			add edi,16384*4
	.csl3:		push ecx
			mov eax,8192
			sub eax,ecx	;positive values only
			mov ebx,0xC90FDAA2	;pi/4*(2^32)
			mul ebx
			mov ebx,8191
			div ebx
			mov ecx,maxtermpairs
			mov ebx,fact_cosine
			mov ebp,eax
			xor esi,esi
			xor eax,eax
			dec eax
	.csl4:		add esi,eax
			mul ebp
			mov eax,edx
			mul ebp
			mov eax,edx
			xor edx,edx
			div dword [ebx]
			sub esi,eax
			mul ebp
			mov eax,edx
			mul ebp
			mov eax,edx
			xor edx,edx
			div dword [ebx+4]
			add ebx,byte 8
			loop .csl4
			mov eax,esi
			shr eax,1
			stosd
			pop ecx
			loop .csl3

			; copy sine [90..135] to [90..45]
			mov ecx,8192
			mov esi,[sinetab]
			add esi,16384*4
			mov edi,esi
			sub edi,byte 4
	.csl5:		cld
			lodsd
			std
			stosd
			loop .csl5
			; copy sine [45..0] to [135..180]
			mov ecx,8192
			xchg edi,esi
	.csl6:		std
			lodsd
			cld
			stosd
			loop .csl6
			cld

			; copy sine [0..90] to [360..450]
			mov ecx,16384
			mov esi,[sinetab]
			mov edi,esi
			add edi,65536*4
			rep movsd

			; copy and invert sine [0..180] to [180..360]
			mov ecx,32768
			mov esi,[sinetab]
			mov edi,esi
			add edi,32768*4
	 .csl7:		lodsd
			neg eax
			stosd
			loop .csl7

			ret

[segment dataseg]
sinetab			dd	0
cosinetab		dd	0
maxtermpairs		equ	3
fact_sine		dd	3*2,4*5,6*7,8*9,10*11,1
fact_cosine		dd	1*2,3*4,5*6,7*8,9*10,1