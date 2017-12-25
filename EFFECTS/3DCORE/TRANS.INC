;
;	data formats:
;	vertex:		4 doublewords 16.16 signed fixpoint
;	matrix:		4 vertices representing columns
;	angle:		doubleword, 16.16 signed fixpoint, 2pi is 65536
;
;	ss = ds assumed !!!
;
[segment codeseg]
[bits 32]

global	_vectordotproduct,_transformvertices,_nullmatrix,_identitymatrix
global	_rotationXmatrix,_matrixtransp,_matrixmultiply,_translationmatrix
global	_projectionmatrix,_rotationYmatrix,_rotationZmatrix
global	_normalizevertices

; in:	esi,ebp - op1,op2
; out:	eax	- result
_vectordotproduct:
			push ebx
			push ecx
			push ebp
			push esi
			push edx

			xor ebx,ebx
			mov ecx,4
	.vdpl1:		lodsd
			imul dword [ebp]
			shrd eax,edx,16
			add ebx,eax
			add ebp,byte 4
			loop .vdpl1
			mov eax,ebx

			pop edx
			pop esi
			pop ebp
			pop ecx
			pop ebx

			ret

; in:	esi,ecx,edi,ebp - source,count,dest,matrix
_transformvertices:
			pushad

	.tfvl1:		push ecx
			push ebp
			mov ecx,4
	.tfvl2:		call _vectordotproduct
			stosd
			add ebp,byte 16
			loop .tfvl2
			pop ebp
			pop ecx
			add esi,byte 16
			loop .tfvl1

			popad
			ret

; in:	ebp	- dest
_nullmatrix:
			pushad

			mov edi,ebp
			mov ecx,4*4
			xor eax,eax
			rep stosd

			popad
			ret

; in:	ebp	- dest
_identitymatrix:
			call _nullmatrix
			push eax
			mov eax,0x00010000
			mov [ebp],eax
			mov [ebp+(4+1)*4],eax
			mov [ebp+(8+2)*4],eax
			mov [ebp+(12+3)*4],eax
			pop eax

			ret

; in:	eax	- angle
;	ebp	- dest
%macro rotationmatrixproc 4
			call _identitymatrix
			push eax
			push ebx
			push edx
			movzx edx,ax
			mov ebx,[sinetab]
			mov eax,[ebx+edx*4]
			sar eax,15
			mov [ebp+%1*4],eax	;+sin
			mov [ebp+%2*4],eax	;+sin
			mov eax,[ebx+edx*4+16384*4]
			sar eax,15
			mov [ebp+%3*4],eax	;+cos
			neg eax
			mov [ebp+%4*4],eax	;-cos
			pop edx
			pop ebx
			pop eax
			ret
%endmacro
_rotationXmatrix:
	rotationmatrixproc (4+1),(8+2),(4+2),(8+1)
_rotationYmatrix:
	rotationmatrixproc 0,(8+2),2,8 
_rotationZmatrix
	rotationmatrixproc 0,(4+1),1,4

; in:	esi		source
;	edi		destination
_matrixtransp:
			pushad

			mov ecx,4
	.mtl1:		push ecx
			mov cl,4
	.mtl2:		movsd
			add esi,byte 12
			loop .mtl2
			sub esi,16*4-4
			pop ecx
			loop .mtl1

			popad
			ret

; in:	esi,ebp		left operand, up operand
;	edi		result
; left operand and the result matrix can be at the same memory location,
; but edi=ebp results in incorrect result matrix
_matrixmultiply:
			pushad

			push edi
			mov edi,tempmatrix
			call _matrixtransp
			pop edi

			mov ecx,4
	.mml1:		push ecx
			mov cl,4
			mov esi,tempmatrix
	.mml2:		call _vectordotproduct
			stosd
			add esi,byte 16
			loop .mml2
			add ebp,byte 16
			pop ecx
			loop .mml1

			popad
			ret

; in:	ebp		- dest
;	eax,ebx,ecx	- X,Y,Z displacement
_translationmatrix:
			call _identitymatrix
			mov [ebp+12],eax
			mov [ebp+16+12],ebx
			mov [ebp+32+12],ecx
			ret

; in:	ebp		- dest
;	eax		- x & y scale
;	ebx		- z weight
; basic projection matrix
_projectionmatrix:
			call _nullmatrix
			push eax
			mov [ebp],eax
			mov [ebp+16+4],eax
			neg dword [ebp+16+4]
			mov [ebp+48+8],ebx
;			mov eax,0x10000
			mov [ebp+32+8],eax
			pop eax
			ret

; in:	esi,edi,ecx	- src,dest,count
;	ebx		- screen center vector (2 dd)
;	esi=edi allowed
_normalizevertices:
			pushad

	.nvl1:		push ecx
			mov ebp,[esi+12]
			mov ecx,2
	.nvl2:		lodsd
			cdq
			shld edx,eax,16
			idiv ebp
			add eax,[ebx]
			stosd
			add ebx,byte 4
			loop .nvl2
			add esi,byte 8
			add edi,byte 8
			sub ebx,byte 8
			pop ecx
			loop .nvl1

			popad
			ret

[segment dataseg]
tempmatrix		times	16 dd(0)