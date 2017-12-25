pgroup	group	realseg

realseg segment para public use16
	assume cs:pgroup, ds:pgroup

jump32_16 macro sel,off
	db 66h
	db 0eah
	dw off
	dw sel
endm

start:

		

		ret

realseg		ends
		end