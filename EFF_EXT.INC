FPS		equ	18
extern	_font_initialize,_font_destroy
extern	_animplay_initialize,_animplay_destroy,_animplay_step,_animplay_render
extern	_falling_initialize,_falling_destroy,_falling_step,_falling_render
extern	_scroll_initialize,_scroll_destroy,_scroll_step,_scroll_render
extern	scroll_numframes
externeffect nwp
extern	nwp_numframes,nwp_effect,nwp_stringofs,nwp_effects,nwp_numeffects
extern	nwp_params
extern	_vertbars,_horlines,_fadebl,_fadecr,_horzscrl,_nwpzoom,_ptab
extern	sracok_desc,pdn0
sd_size		equ	12
extern	scroll_szovegaddr
extern	animname,animframes
;externeffect d3t
externeffect film
extern	film_animname,film_string,film_numframes
externeffect fade
externeffect se
extern	se_pef,se_imf,peftab,imftab,se_num_effects
extern	_random
externeffect	kukac
externeffect	pacfade
extern	pf_numframes
