%include "..\..\segments.inc"
global	pdn0
global sracok_desc
[segment dataseg]

; number, normal name, capital name, animation name
%macro pdesc 4
pdn%1		db %2,0		;normal name
pdc%1		db ' ',%3,' ',0		;capital name
pda%1		db %4,0		;animation name
%endmacro

%macro pvalue 1
	dd	pdn%1
	dd	pdc%1
	dd	pda%1
%endmacro
%macro ptable 1
	%assign i 0
	%rep %1
		pvalue i
		%assign i i+1
	%endrep
%endmacro

pdesc   0,"Ak� K�roly","AK� K�ROLY","Karcsi"
pdesc   1,"Albert Viktor","ALBERT VIKTOR","Veek"
pdesc   2,"B�r� J�nos K�lm�n","B�R� J�NOS","BJ"
pdesc   3,"B�r� Zolt�n L�szl�","B�R� ZOLT�N","BZ"
pdesc   4,"Cserv�k �d�m","CSERV�K �D�M","Llamma"
pdesc   5,"Csizmadia P�ter","CSIZMADIA P�TER","Csizma"
pdesc   6,"Farkas G�bor","FARKAS G�BOR","Ordas"
pdesc   7,"F�ldi Istv�n","F�LDI ISTV�N","Mazsy"
pdesc   8,"G�bert Csaba Tam�s","G�BERT CSABA","Csuba"
pdesc	9,"Gub�n P�ter","GUB�N P�TER","Guban"
pdesc  10,"Juh�sz P�ter","JUH�SZ P�TER","Juhasz"
pdesc  11,"Kiss Bal�zs","KISS BAL�ZS","Balu"
pdesc  12,"Koll�r L�szl�","KOLL�R L�SZL�","Laci"
pdesc  13,"Koll�r Tam�s","KOLL�R TAM�S","Mutz"
pdesc  14,"Kuklis Zolt�n","KUKLIS ZOLT�N","Kukli"
pdesc  15,"Kov�cs Gy�rgy","KOV�CS GY�RGY","Gyuri"
pdesc  16,"K�v�ri Tam�s","K�V�RI TAM�S","Kovari"
pdesc  17,"K�rti �rp�d Istv�n","K�RTI �RP�D","Arpi"
pdesc  18,"Mahunka Lajos Zsolt","MAHUNKA LAJOS","Lajos"
pdesc  19,"Moln�r J�zsef G�bor","MOLN�R J�ZSEF","Joe"
pdesc  20,"Papp Zolt�n","PAPP ZOLT�N","Papp"
pdesc  21,"Tam�si Ferenc","TAM�SI FERENC","Feri"
pdesc  22,"Tanai Tam�s","TANAI TAM�S","Tanai"
pdesc  23,"Ujfaludi Andr�s","UJFALUDI ANDR�S","Umbi"
pdesc  24,"V�radi Attila","V�RADI ATTILA","Ati"
pdesc  25,"V�s�rhelyi Tam�s","V�S�RHELYI TAM�S","Gonzi"
pdesc  26,'Fekete Katalin',"FEKETE KATALIN","Oszi"
sracok_desc:
ptable	27
