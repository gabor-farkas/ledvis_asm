%include "../../segments.inc"
%include "../../macros.inc"
[segment codeseg]
[bits 32]

;
;	contained scroll background effects:
;		0	- 2 level outlined
;		1	- pure text
;		2	- simple vertical gradient (?)
;
;

globaleffect se
extern	matrix
extern	_allocmem,_freemem
extern	scroll_outbuf
extern	_debugeax

%macro 	olbuf	2
	mov esi,[%2]
	mov edi,[%1]
	call _outline
%endmacro
%macro	orbuf	2
	mov esi,[%2]
	mov edi,[%1]
	call _orbuffers
%endmacro
%macro	clrbuf 1
	mov edi,[%1]
	call _clrbuf
%endmacro

_se_initialize:

			mov eax,48*26*2
			call _allocmem
			mov [outline],eax
			add eax,48*26
			mov [outline2],eax

			ret

_se_destroy:

			mov eax,[outline]
			call _freemem

			ret

_se_step:

			ret

_se_render:

			mov esi,[scroll_outbuf]
			mov edi,[outline]
			mov ecx,48*26*2/4
			xor eax,eax
			rep stosd
			mov edi,[outline]
			add edi,byte 48
			mov ecx,24*48/4
			rep movsd

;			mov edi,[outline]
;			add edi,13*48
;			mov ecx,48/4
;			mov eax,0x3F3F3F3F
;			rep stosd

			call [imf]

			mov esi,[outline]
			add esi,byte 48+12
			mov ebp,[outline2]
			add ebp,byte 48+12
			mov edx,[scroll_outbuf]
			add edx,byte 12
			mov edi,[matrix]
			mov ecx,24
	.srl2:		push ecx
			mov cl,24
	.srl3:		call [pef]
	                ;movsb
			loop .srl3
			pop ecx
			add esi,byte 24
			add ebp,byte 24
			add edx,byte 24
			loop .srl2

			ret

; in:	esi,edi	- input/output buffers
_outline:

			push edi
			mov ecx,48*26/4
			xor eax,eax
			rep stosd
			pop edi
			add esi,byte 48
			add edi,byte 48
			mov ecx,24*48
	.ol1:		lodsb
			or al,al
			jnz .on
			or al,[esi-2]
			or al,[esi]
			or al,[esi-49]
			or al,[esi+47]
			mov [edi],al
		.on	inc edi
			loop .ol1

			ret

_orbuffers:

			mov ecx,48*26/4
	.obl1:		lodsd
			or [edi],eax
			add edi,byte 4
			loop .obl1

			ret

_pef_0:
			lodsb
			shr al,2
			mov ah,[ebp]
			shr ah,3
			or al,ah
			or al,[edx]
			stosb
			inc ebp
			inc edx

			ret

_im_0:
			olbuf outline2,outline
			orbuf outline2,outline
			olbuf outline,outline2
			orbuf outline2,outline
			olbuf outline,outline2
;			orbuf outline2,outline
;			olbuf outline,outline2

			ret

_clrbuf:

			mov ecx,48*26/4
			xor eax,eax
			rep stosd

			ret


[segment dataseg]
global	se_pef,se_imf,peftab,imftab,se_num_effects
outline			dd	0
outline2		dd	0
pef			dd	_pef_0	;pixeleffect of final output
imf			dd	_im_0
peftab			dd	_pef_0,_pef_1,_pef_2
imftab			dd	_im_0,_im_1,_im_2
se_pef			equ	pef
se_imf			equ	imf
se_num_effects		dd	3
%include	"e1.inc"
%include	"e2.inc"
