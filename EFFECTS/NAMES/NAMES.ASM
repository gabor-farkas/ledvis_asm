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

pdesc   0,"Akó Károly","AKÓ KÁROLY","Karcsi"
pdesc   1,"Albert Viktor","ALBERT VIKTOR","Veek"
pdesc   2,"Bíró János Kálmán","BÍRÓ JÁNOS","BJ"
pdesc   3,"Bíró Zoltán László","BÍRÓ ZOLTÁN","BZ"
pdesc   4,"Cservák Ádám","CSERVÁK ÁDÁM","Llamma"
pdesc   5,"Csizmadia Péter","CSIZMADIA PÉTER","Csizma"
pdesc   6,"Farkas Gábor","FARKAS GÁBOR","Ordas"
pdesc   7,"Földi István","FÖLDI ISTVÁN","Mazsy"
pdesc   8,"Gébert Csaba Tamás","GÉBERT CSABA","Csuba"
pdesc	9,"Gubán Péter","GUBÁN PÉTER","Guban"
pdesc  10,"Juhász Péter","JUHÁSZ PÉTER","Juhasz"
pdesc  11,"Kiss Balázs","KISS BALÁZS","Balu"
pdesc  12,"Kollár László","KOLLÁR LÁSZLÓ","Laci"
pdesc  13,"Kollár Tamás","KOLLÁR TAMÁS","Mutz"
pdesc  14,"Kuklis Zoltán","KUKLIS ZOLTÁN","Kukli"
pdesc  15,"Kovács György","KOVÁCS GYÖRGY","Gyuri"
pdesc  16,"Kõvári Tamás","KÕVÁRI TAMÁS","Kovari"
pdesc  17,"Kürti Árpád István","KÜRTI ÁRPÁD","Arpi"
pdesc  18,"Mahunka Lajos Zsolt","MAHUNKA LAJOS","Lajos"
pdesc  19,"Molnár József Gábor","MOLNÁR JÓZSEF","Joe"
pdesc  20,"Papp Zoltán","PAPP ZOLTÁN","Papp"
pdesc  21,"Tamási Ferenc","TAMÁSI FERENC","Feri"
pdesc  22,"Tanai Tamás","TANAI TAMÁS","Tanai"
pdesc  23,"Ujfaludi András","UJFALUDI ANDRÁS","Umbi"
pdesc  24,"Váradi Attila","VÁRADI ATTILA","Ati"
pdesc  25,"Vásárhelyi Tamás","VÁSÁRHELYI TAMÁS","Gonzi"
pdesc  26,'Fekete Katalin',"FEKETE KATALIN","Oszi"
sracok_desc:
ptable	27
