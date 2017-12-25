alink -o led_raw.c0 -oCOM @led_r2.lnk
copy /B data\bin_hd + led_raw.c0 led_raw.bin
data\checksum led_raw.bin 0D00
del led_raw.c0
