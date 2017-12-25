@echo off
cd main_dos
call main_dos.bat
cd ..
cd scrplay
call scrplay.bat
cd ..
tlink.exe @led_dr.lnk
tlink.exe @led_dos.lnk
rem @d:\gabi\tlink.exe @led_raw.lnk
