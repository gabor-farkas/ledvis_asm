%macro invoke 1-*
	%rep %0-1
		%rotate -1
		push %1
	%endrep
		%rotate -1
		call %1
%endmacro

%imacro interfacecall 2-*	;IFcall lpDD,80,dword [hwnd],dword flags
	mov eax,[%1]
	mov ebx,%2
  %rep %0-2
	%rotate -1
	push %1
  %endrep
	push eax
	mov eax,[eax]
	call [eax+ebx]
%endmacro
%idefine ifcall interfacecall

%define	lParam	ebp+20
%define	wParam	ebp+16
%define	uMsg	ebp+12
%define _hwnd	ebp+8

;%idefine DEBUG

%macro msgbox 1
	pushad
  	invoke _MessageBoxA@16,byte 0,dword %1,dword %1,byte 0
	popad
%endmacro

;----------------------------------
%macro externeffect 1
	extern _%1_initialize,_%1_destroy,_%1_step,_%1_render
%endmacro
%macro globaleffect 1
	global _%1_initialize,_%1_destroy,_%1_step,_%1_render
%endmacro