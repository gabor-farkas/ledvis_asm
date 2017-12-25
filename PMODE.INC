%ifndef WIN32
;*** for NASM version 1999.XII.23. (HHC's work :)
extern   v86r_eax, v86r_ebx, v86r_ecx, v86r_edx
extern   v86r_esi, v86r_edi, v86r_ebp
extern   v86r_ah, v86r_al, v86r_bh, v86r_bl
extern   v86r_ch, v86r_cl, v86r_dh, v86r_dl
extern   v86r_ax, v86r_bx, v86r_cx, v86r_dx
extern   v86r_si, v86r_di, v86r_bp
extern	v86r_ds, v86r_es, v86r_fs, v86r_gs
extern	_selcode, _seldata, _selzero, _lomembase
extern	_lomemtop, _himembase, _himemtop, _pspa
extern   _code16a, _code32a, _getirqvect, _setirqvect
extern   _sysbyte0, _setselector, _exitcode

extern   _getmem, _getlomem, _gethimem, _lomemsize
extern   _himemsize, _ret, _exit, _getirqmask
extern   _setirqmask, _rmpmirqset, _rmpmirqfree
extern   _getselector, _freeselector

; Convert physical pointer to realative pointer
%macro @rlp 2
        mov %1,%2
        sub %1,dword [_code32a]
%endmacro

; Convert realative pointer to physical pointer
%macro @php 2
        mov %1,%2
	add %1,[_code32a]
%endmacro

%endif