rem @echo off
d:\gabi\nasm\nasmw -fwin32 main_win.asm
d:\98ddk\bin\link @link.lnk
copy main_win.dll d:\projects\ledvis\main_win.dll
copy ..\data\images.fls d:\projects\ledvis\data\images.fls
