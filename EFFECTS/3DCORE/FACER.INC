;
;	face structure:
;		3 dword indices
;
;
[segment codeseg]
[bits 32]

global	_drawface,df_scrbuf,df_faces,df_vertices,df_lightdir,df_fnormals
global	df_Zbuffer,_df_clrzbuffer,df_texture,df_texcoords

%macro Ysort 4
	mov eax,[%1+4]
	cmp eax,[%2+4]
	jnge %%1
	xchg %1,%2
	mov ax,[baseUV+%3]
	mov bx,[baseUV+%4]
	mov [baseUV+%4],ax
	mov [baseUV+%3],bx
%%1:
%endmacro
%macro addeax 2
	mov eax,%2
	add %1,eax
%endmacro
%macro step 4
	mov eax,%2
	sub eax,%3
	cdq
	idiv dword %4
	mov %1,eax
%endmacro
%macro moveax 2
	mov eax,%2
	mov %1,eax
%endmacro
%macro initialT 3
	movzx edx,byte [baseUV+%2]
	xor eax,eax
	idiv dword %3
	mov %1,eax
%endmacro
%macro stepT 6
	movzx edx,byte [baseUV+%2]
	xor eax,eax
	idiv dword %5
	mov ebx,eax
	movzx edx,byte [baseUV+%3]
	xor eax,eax
	idiv dword %6
	neg eax
	add eax,ebx
	cdq
	idiv dword %4
	mov %1,eax
%endmacro
%macro initialP 2
	xor eax,eax
	lea edx,[eax+1]
	idiv dword %2
	mov %1,eax
%endmacro
%macro stepP 4
	xor eax,eax
	lea edx,[eax+1]
	idiv dword %2
	mov ebx,eax
	xor eax,eax
	lea edx,[eax+1]
	idiv dword %3
	sub ebx,eax
	mov eax,ebx
	cdq
	idiv dword %4
	mov %1,eax
%endmacro

; in:	eax		- face index
;	df_faces	- face structures
;	df_vertices	- vertexbuffer
;	df_scrbuf	- 96x96 screen buffer
;	df_lightdir	- pointer to light direction vector
;	df_fnormals	- face normals array
;	df_Zbuffer	- 96*96*4 zbuffer
;	df_texture	- 24*24 texture
;	df_texcoords	- array of (byte x, byte y) struct

_df_clrzbuffer:

			mov edi,[df_Zbuffer]
			mov ecx,96*96
			mov eax,0x7FFFFFFF
			rep stosd

			ret

_drawface:
			push eax
			;- get face vertices and texture coordinates
			mov ebx,12
			mul ebx
			add eax,[df_faces]
			mov edx,eax
			mov ebp,[df_vertices]
			mov ebx,[df_texcoords]
			mov esi,[edx]
			mov ax,[ebx+2*esi]
			mov [baseUV],ax
			shl esi,4
			add esi,ebp
			mov edi,[edx+4]
			mov ax,[ebx+2*edi]
			mov [baseUV+2],ax
			shl edi,4
			add edi,ebp
			mov eax,[edx+8]
			mov cx,[ebx+2*eax]
			mov [baseUV+4],cx
			shl eax,4
			add ebp,eax
			pop eax

			;- count face brightness
			push esi
			push ebp
			shl eax,4
			add eax,[df_fnormals]
			mov esi,eax
			mov ebp,[df_lightdir]
			call _vectordotproduct
			or eax,eax
			jge .dfblack
			xor eax,eax
	.dfblack:	mov ebx,63
			mul ebx
			mov ebx,0x10000
			div ebx
			cmp al,63
			jna .dfwhite
			mov al,63
	.dfwhite:	mov [dfscolor],al
			pop ebp
			pop esi


			; sort by Y
			Ysort esi,edi,0,2
			Ysort edi,ebp,2,4
			Ysort esi,edi,0,2

			;- compute differences and fill some initial values
			mov ebx,[esi+4]
			sar ebx,16
			mov eax,[edi+4]
			sar eax,16
			sub eax,ebx
			mov [dfABh],eax
			mov eax,[ebp+4]
			sar eax,16
			sub eax,ebx
			jz near .dfend		;triangle has 0 height
			mov [dfY],ebx
			mov [dfACh],eax
			mov eax,[ebp+4]
			sub eax,[edi+4]
			sar eax,16
			mov [dfBCh],eax
			;-
			mov ebx,[esi]
			mov [dfLX],ebx
			mov [dfRX],ebx
			mov eax,[edi]
			sub eax,ebx
			mov [dfABw],eax
			mov eax,[ebp]
			sub eax,ebx
			mov [dfACw],eax
			mov eax,[ebp]
			sub eax,[edi]
			mov [dfBCw],eax

			mov eax,[dfACw]
			cdq
			idiv dword [dfACh]
			mov [dfACs],eax

			mov eax,[esi+8]
			mov [dfLZ],eax
			mov [dfRZ],eax
			step [dfRZs],[ebp+8],[esi+8],[dfACh]
			initialP [dfpLZ],[esi+8]
			mov [dfpRZ],eax
			stepP [dfpRZs],[ebp+8],[esi+8],[dfACh]

			initialT [dfLU],0,[esi+8]
			mov [dfRU],eax
			initialT [dfLV],1,[esi+8]
			mov [dfRV],eax
			stepT [dfRUs],4,0,[dfACh],[ebp+8],[esi+8]
			stepT [dfRVs],5,1,[dfACh],[ebp+8],[esi+8]

			mov ecx,[dfABh]
			or ecx,ecx
			jz near .dfnoupper
			mov eax,[dfABw]
			cdq
			idiv ecx
			mov [dfABCs],eax
			step [dfLZs],[edi+8],[esi+8],ecx
			stepT [dfLUs],2,0,ecx,[edi+8],[esi+8]
			stepT [dfLVs],3,1,ecx,[edi+8],[esi+8]
			stepP [dfpLZs],[edi+8],[esi+8],[dfABh]
			;- B supposed to be on the left side of AC
			call _df_scanlines
			
	.dfnoupper:	;- lower part
			mov eax,[edi]
			mov [dfLX],eax
			mov eax,[edi+4]
			sar eax,16
			mov [dfY],eax
			moveax [dfLZ],[edi+8]
			initialT [dfLU],2,[edi+8]
			initialT [dfLV],3,[edi+8]
			initialP [dfpLZ],[edi+8]

			mov ecx,[dfBCh]
			or ecx,ecx
			jz near .dfnolower
			mov eax,[dfBCw]
			cdq
			idiv ecx
			mov [dfABCs],eax
			step [dfLZs],[ebp+8],[edi+8],ecx
			stepT [dfLUs],4,2,ecx,[ebp+8],[edi+8]
			stepT [dfLVs],5,3,ecx,[ebp+8],[edi+8]
			stepP [dfpLZs],[ebp+8],[edi+8],[dfBCh]

			call _df_scanlines

	.dfnolower:

	.dfend:
			ret

_df_scanlines:
			pushad

	.dfsll1:	;- draw line
			push ecx
			mov ebx,[dfRX]
			mov eax,[dfLX]
			xor ebp,ebp
			cmp eax,ebx
			jng .dfsl2
			xchg eax,ebx
			inc ebp
	.dfsl2:		mov ecx,ebx
			sar ecx,16
			adc ecx,0
			sar eax,16
			adc eax,0
			sub ecx,eax
			cmp ecx,0
			jnge near .dfsl1
			inc ecx
			mov [dfX],eax
			step [dfZs],[dfRZ],[dfLZ],ecx
			step [dfUs],[dfRU],[dfLU],ecx
			step [dfVs],[dfRV],[dfLV],ecx
			step [dfpZs],[dfpRZ],[dfpLZ],ecx
			moveax [dfZ],[dfLZ]
			moveax [dfU],[dfLU]
			moveax [dfV],[dfLV]
			moveax [dfpZ],[dfpLZ]
			or ebp,ebp
			jz .dfsl3
			neg dword [dfZs]
			moveax [dfZ],[dfRZ]
			moveax [dfU],[dfRU]
			moveax [dfV],[dfRV]
			moveax [dfpZ],[dfpRZ]
			neg dword [dfUs]
			neg dword [dfVs]
			neg dword [dfpZs]
	.dfsl3:
	.dfsll2:	call _df_gpixel		;highly optimized stuff
			inc dword [dfX]
			addeax [dfZ],[dfZs]
			addeax [dfU],[dfUs]
			addeax [dfV],[dfVs]
			addeax [dfpZ],[dfpZs]
			loop .dfsll2
	.dfsl1:
			pop ecx
			;- step
			addeax [dfLX],[dfABCs]
			addeax [dfRX],[dfACs]
			addeax [dfLZ],[dfLZs]
			addeax [dfRZ],[dfRZs]
			addeax [dfLU],[dfLUs]
			addeax [dfLV],[dfLVs]
			addeax [dfRU],[dfRUs]
			addeax [dfRV],[dfRVs]
			addeax [dfpLZ],[dfpLZs]
			addeax [dfpRZ],[dfpRZs]
			inc dword [dfY]
			dec ecx
			jnz near .dfsll1

			popad
			ret

_df_gpixel:

			mov eax,96
			mov ebx,[dfY]
			or ebx,ebx
			jnge .dfscend
			cmp ebx,eax
			jge .dfscend
			mov edi,[dfX]
			or edi,edi
			jnge .dfscend
			cmp edi,eax
			jnc .dfscend
			imul ebx
			add edi,eax
			lea ebp,[4*edi]
			add ebp,[df_Zbuffer]
			mov eax,[dfZ]
			cmp eax,[ebp]
			jge .dfscend
			mov [ebp],eax
			xor eax,eax
			lea edx,[eax+1]
			idiv dword [dfpZ]
			mov ebx,eax
			add edi,[df_scrbuf]
			mov eax,[dfU]
			imul ebx
			mov [dfUp],edx
			mov eax,[dfV]
			imul ebx
			mov [dfVp],edx
			call [df_pixel]
	.dfscend:

			ret

_df_scpixel:

			mov al,[dfVp]
			or al,[dfUp]
;			and al,0xE8
;			shl al,1
;			mov al,24
;			mul byte [dfVp]
;			add ax,[dfUp]
;			movzx eax,ax
;			mov ebx,[df_texture]
;			mov al,[ebx+eax]
;			mov bl,63
;			mul byte [dfscolor]
;			div bl
			mov [edi],al

			ret

[segment dataseg]
df_scrbuf		dd	0
df_faces		dd	0
df_vertices		dd	0
df_lightdir		dd	0
df_fnormals		dd	0
df_Zbuffer		dd	0
df_texture		dd	0
df_texcoords		dd	0
dfABh			dd	0	;height of upper part	32.0
dfBCh			dd	0	;height of lower part
dfACh			dd	0	;height of triangle
dfABw			dd	0	;16.16
dfBCw			dd	0
dfACw			dd	0
dfLX			dd	0	;16.16 scanline one end X coord
dfRX			dd	0	;other end
dfY			dd	0	;32.0 scanline Y
dfABCs			dd	0	;dX/dY
dfACs			dd	0
dfX			dd	0	;32.0 pixel X
dfLZ			dd	0	;Z coord on scanline left
dfRZ			dd	0	;right
dfLZs			dd	0	;dZ/dY on left
dfRZs			dd	0	;right
dfZ			dd	0	;actual Z on pixel
dfZs			dd	0	;dZ/dX in scanline
dfLU			dd	0	;the same stuff for U,V tex coords
dfLV			dd	0
dfRU			dd	0
dfRV			dd	0
dfLUs			dd	0
dfLVs			dd	0
dfRUs			dd	0
dfRVs			dd	0
dfU			dd	0
dfV			dd	0
dfUs			dd	0
dfVs			dd	0
dfUp			dd	0	;perspective corrected stuff
dfVp			dd	0
dfpLZ			dd	0
dfpRZ			dd	0
dfpLZs			dd	0
dfpRZs			dd	0
dfpZ			dd	0
dfpZs			dd	0
baseUV			times	6 db(0)
df_pixel		dd	_df_scpixel
dfscolor		db	63