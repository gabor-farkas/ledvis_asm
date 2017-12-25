%include "../../segments.inc"
%include "3deng.inc"
[segment codeseg]
[bits 32]

global	_d3t_initialize,_d3t_destroy,_d3t_step,_d3t_render
extern	matrix
extern	_allocmem,_freemem
extern	_giveresource

_d3t_initialize:

			call _core3d_init
			mov ebp,pmatrix
			mov eax,0x500000
			mov ebx,0x10000
			call _projectionmatrix

			mov eax,96*96
			call _allocmem
			mov [d3scrbuf],eax

			mov eax,96*96*4
			call _allocmem
			mov [df_Zbuffer],eax

			mov eax,24*24
			call _allocmem
			mov [texture],eax
			mov [df_texture],eax
			mov edi,eax
			mov ch,0x18
	.tl1:		mov cl,0x18
	.tl2:		mov al,cl
			or al,ch
;			add al,ch
;			mov bl,63
;			mul bl
;			mov bl,0x30
;			div bl
			stosb
			dec cl
			jnz .tl2
			dec ch
			jnz .tl1

			mov esi,animname
			call _giveresource
			mov [anim],esi
			xor eax,eax
			mov [currentframe],eax

			ret

_d3t_destroy:

			mov eax,[df_Zbuffer]
			call _freemem

			mov eax,[d3scrbuf]
			call _freemem

			call _core3d_dest

			ret

_d3t_step:

			add dword [angle],0x300
;			add dword [dist],0x400
			mov esi,[anim]
			mov eax,[currentframe]
;			inc eax
			cmp eax,[esi]
			jc .ds1
			xor eax,eax
		.ds1:	mov [currentframe],eax

			ret
_d3t_render:

			mov edi,[d3scrbuf]
			mov ecx,96*96/4
			xor eax,eax
			rep stosd
			call _df_clrzbuffer

			mov eax,[angle]
			mov ebp,tmatrix
			call _rotationYmatrix
			mov ebp,t2matrix
			xor eax,eax
			xor ebx,ebx
			mov ecx,[dist]
			call _translationmatrix
			mov esi,tmatrix
			mov ebp,t2matrix
			mov edi,t3matrix
			call _matrixmultiply
			mov esi,t3matrix
			mov ebp,pmatrix
			mov edi,t2matrix
			call _matrixmultiply
			mov esi,d3points
			mov edi,d3transformed
			mov ebp,t2matrix
			mov ecx,[numvertices]
			call _transformvertices
			mov esi,facenormals
			mov edi,transformedfn
			mov ebp,tmatrix
			mov ecx,[numfaces]
			call _transformvertices
			mov esi,d3transformed
			mov edi,esi
			mov ecx,[numvertices]
			mov ebx,center
			call _normalizevertices

			mov eax,[d3scrbuf]
			mov [df_scrbuf],eax
			mov dword [df_vertices],d3transformed
			mov dword [df_faces],faces
			mov dword [df_lightdir],lightdir
			mov dword [df_fnormals],transformedfn
			mov dword [df_texcoords],texcoords
			mov eax,24*24
			mul dword [currentframe]
			add eax,[anim]
			add eax,byte 13
			mov [df_texture],eax
			mov ecx,[numfaces]
			xor eax,eax
	.rl1:		push eax
			push ecx
			call _drawface
			pop ecx
			pop eax
			inc eax
			loop .rl1

			mov esi,[d3scrbuf]
			mov edi,[matrix]
			call _bufferconvert

			ret

[segment dataseg]
%macro imx	0
times	16 dd(0)
%endmacro
NUMVERTICES		equ	8
d3transformed		times	NUMVERTICES*4 dd(0)
angle			dd	0
tmatrix			imx
t2matrix		imx
t3matrix		imx
pmatrix			imx
dist			dd	0x30000
d3scrbuf		dd	0
numvertices		dd	NUMVERTICES
pp			equ	0x10000
pm			equ	-0x10000
pn			equ	0
d3points		dd	pp,pm,pm,pp
			dd	pp,pm,pp,pp
			dd	pm,pm,pp,pp
			dd	pm,pm,pm,pp
			dd	pp,pp,pm,pp
			dd	pp,pp,pp,pp
			dd	pm,pp,pp,pp
			dd	pm,pp,pm,pp
NUMFACES		equ	12
numfaces		dd	NUMFACES
faces			dd	3,0,4
			dd	3,4,7
			dd	0,1,5
			dd	0,5,4
			dd	1,5,6
			dd	1,6,2
			dd	2,6,7
			dd	2,7,3
			dd	4,5,6
			dd	4,6,7
			dd	0,3,2
			dd	0,2,1
facenormals		dd	pn,pn,pm,pn
			dd	pn,pn,pm,pn
			dd	pp,pn,pn,pn
			dd	pp,pn,pn,pn
			dd	pn,pn,pp,pn
			dd	pn,pn,pp,pn
			dd	pm,pn,pn,pn
			dd	pm,pn,pn,pn
			dd	pn,pp,pn,pn
			dd	pn,pp,pn,pn
			dd	pn,pm,pn,pn
			dd	pn,pm,pn,pn
transformedfn		times	NUMFACES*4 dd(0)
lightdir		dd	pn,pn,pm,pn
center			dd	0x300000,0x300000
texture			dd	0
texcoords		db	23,23
			db	0,23
			db	23,23
			db	0,23
			db	23,0
			db	0,0
			db	23,0
			db	0,0
animname		db	'Umbi0',0
anim			dd	0
currentframe		dd	0