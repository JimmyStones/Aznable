;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 3.8.0 #10562 (Linux)
;--------------------------------------------------------
	.module ui
	.optsdcc -mz80
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _strlen
	.globl _sprintf
	.globl _asc_1
	.globl _asc_0
	.globl _musicram
	.globl _sndram
	.globl _tilemapram
	.globl _tilemapctl
	.globl _spritecollisionram
	.globl _spriteram
	.globl _bgcolram
	.globl _fgcolram
	.globl _chram
	.globl _system_pause
	.globl _starfield
	.globl _timer
	.globl _timestamp
	.globl _ps2_mouse
	.globl _ps2_key
	.globl _spinner
	.globl _paddle
	.globl _analog_r
	.globl _analog_l
	.globl _joystick
	.globl _input0
	.globl _clear_chars
	.globl _clear_char_area
	.globl _clear_bgcolor
	.globl _write_string
	.globl _write_stringfs
	.globl _write_stringf
	.globl _write_stringf_ushort
	.globl _write_stringf_short
	.globl _write_stringf_ulong
	.globl _write_char
	.globl _write_char_row
	.globl _set_fgcolour
	.globl _set_bgcolour
	.globl _write_bgcol_row
	.globl _write_bits
	.globl _box
	.globl _panel
	.globl _panel_shaded
	.globl _fill
	.globl _fill_bgcol
	.globl _page_border
	.globl _draw_charactermap
;--------------------------------------------------------
; special function registers
;--------------------------------------------------------
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area _DATA
_input0	=	0x8000
_joystick	=	0x8100
_analog_l	=	0x8200
_analog_r	=	0x8300
_paddle	=	0x8400
_spinner	=	0x8500
_ps2_key	=	0x8600
_ps2_mouse	=	0x8700
_timestamp	=	0x8800
_timer	=	0x8900
_starfield	=	0x8a00
_system_pause	=	0x8a30
_chram	=	0x9800
_fgcolram	=	0xa000
_bgcolram	=	0xa800
_spriteram	=	0xb000
_spritecollisionram	=	0xb400
_tilemapctl	=	0x8c00
_tilemapram	=	0x8c10
_sndram	=	0x8b00
_musicram	=	0x8b10
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area _INITIALIZED
_asc_0::
	.ds 1
_asc_1::
	.ds 1
;--------------------------------------------------------
; absolute external ram data
;--------------------------------------------------------
	.area _DABS (ABS)
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area _HOME
	.area _GSINIT
	.area _GSFINAL
	.area _GSINIT
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area _HOME
	.area _HOME
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area _CODE
;ui.c:29: void clear_chars(char c)
;	---------------------------------
; Function clear_chars
; ---------------------------------
_clear_chars::
	push	ix
	ld	ix,#0
	add	ix,sp
;ui.c:31: for (unsigned short p = 0; p < chram_size; p++)
	ld	bc, #0x0000
00103$:
	ld	hl, #_chram_size
	ld	a, c
	sub	a, (hl)
	ld	a, b
	inc	hl
	sbc	a, (hl)
	jr	NC,00105$
;ui.c:33: chram[p] = c;
	ld	hl, #_chram+0
	add	hl, bc
	ld	a, 4 (ix)
	ld	(hl), a
;ui.c:34: fgcolram[p] = c;
	ld	hl, #_fgcolram
	add	hl, bc
	ld	a, 4 (ix)
	ld	(hl), a
;ui.c:35: bgcolram[p] = c;
	ld	hl, #_bgcolram
	add	hl, bc
	ld	a, 4 (ix)
	ld	(hl), a
;ui.c:31: for (unsigned short p = 0; p < chram_size; p++)
	inc	bc
	jr	00103$
00105$:
;ui.c:37: }
	pop	ix
	ret
;ui.c:39: void clear_char_area(char c, unsigned char tx, unsigned char ty, unsigned char bx, unsigned char by)
;	---------------------------------
; Function clear_char_area
; ---------------------------------
_clear_char_area::
	push	ix
	ld	ix,#0
	add	ix,sp
;ui.c:41: for (char y = ty; y <= by; y++)
	ld	b, 6 (ix)
	ld	a, 7 (ix)
	sub	a, 5 (ix)
	ld	c, a
00103$:
	ld	a, 8 (ix)
	sub	a, b
	jr	C,00105$
;ui.c:43: write_char_row(c, 0, tx, y, (bx - tx) + 1);
	ld	d, c
	inc	d
	push	bc
	ld	e, b
	push	de
	ld	a, 5 (ix)
	push	af
	inc	sp
	xor	a, a
	push	af
	inc	sp
	ld	a, 4 (ix)
	push	af
	inc	sp
	call	_write_char_row
	pop	af
	pop	af
	inc	sp
	pop	bc
;ui.c:41: for (char y = ty; y <= by; y++)
	inc	b
	jr	00103$
00105$:
;ui.c:45: }
	pop	ix
	ret
;ui.c:48: void clear_bgcolor(char color)
;	---------------------------------
; Function clear_bgcolor
; ---------------------------------
_clear_bgcolor::
	push	ix
	ld	ix,#0
	add	ix,sp
;ui.c:50: for (unsigned short p = 0; p < chram_size; p++)
	ld	bc, #0x0000
00103$:
	ld	hl, #_chram_size
	ld	a, c
	sub	a, (hl)
	ld	a, b
	inc	hl
	sbc	a, (hl)
	jr	NC,00105$
;ui.c:52: bgcolram[p] = color;
	ld	hl, #_bgcolram+0
	add	hl, bc
	ld	a, 4 (ix)
	ld	(hl), a
;ui.c:50: for (unsigned short p = 0; p < chram_size; p++)
	inc	bc
	jr	00103$
00105$:
;ui.c:54: }
	pop	ix
	ret
;ui.c:57: void write_string(const char *string, char color, unsigned char x, unsigned char y)
;	---------------------------------
; Function write_string
; ---------------------------------
_write_string::
	push	ix
	ld	ix,#0
	add	ix,sp
	push	af
;ui.c:59: unsigned short p = (y * chram_cols) + x;
	ld	hl,#_chram_cols + 0
	ld	e, (hl)
	ld	h, 8 (ix)
	ld	l, #0x00
	ld	d, l
	ld	b, #0x08
00118$:
	add	hl, hl
	jr	NC,00119$
	add	hl, de
00119$:
	djnz	00118$
	ld	c, 7 (ix)
	ld	b, #0x00
	add	hl, bc
	ex	de, hl
;ui.c:60: unsigned char l = strlen(string);
	ld	l, 4 (ix)
	ld	h, 5 (ix)
	push	hl
	call	_strlen
	pop	af
	ld	c, l
;ui.c:61: for (char c = 0; c < l; c++)
	ld	b, #0x00
00103$:
	ld	a, b
	sub	a, c
	jr	NC,00105$
;ui.c:63: chram[p] = string[c];
	ld	hl, #_chram
	add	hl, de
	ex	(sp), hl
	ld	a, 4 (ix)
	add	a, b
	ld	l, a
	ld	a, 5 (ix)
	adc	a, #0x00
	ld	h, a
	ld	a, (hl)
	pop	hl
	push	hl
	ld	(hl), a
;ui.c:64: fgcolram[p] = color;
	ld	hl, #_fgcolram
	add	hl, de
	ld	a, 6 (ix)
	ld	(hl), a
;ui.c:65: p++;
	inc	de
;ui.c:61: for (char c = 0; c < l; c++)
	inc	b
	jr	00103$
00105$:
;ui.c:67: }
	ld	sp, ix
	pop	ix
	ret
;ui.c:70: void write_stringfs(const char *format, char color, unsigned char x, unsigned char y, signed char data)
;	---------------------------------
; Function write_stringfs
; ---------------------------------
_write_stringfs::
	push	ix
	ld	ix,#0
	add	ix,sp
	ld	hl, #-33
	add	hl, sp
	ld	sp, hl
;ui.c:72: unsigned short p = (y * chram_cols) + x;
	ld	hl,#_chram_cols + 0
	ld	e, (hl)
	ld	h, 8 (ix)
	ld	l, #0x00
	ld	d, l
	ld	b, #0x08
00124$:
	add	hl, hl
	jr	NC,00125$
	add	hl, de
00125$:
	djnz	00124$
	ld	c, 7 (ix)
	ld	b, #0x00
	add	hl, bc
	ex	de, hl
;ui.c:74: sprintf(temp, format, data);
	ld	a, 9 (ix)
	ld	c, a
	rla
	sbc	a, a
	ld	b, a
	ld	hl, #0x0000
	add	hl, sp
	ld	-3 (ix), l
	ld	-2 (ix), h
	push	de
	push	bc
	ld	c, 4 (ix)
	ld	b, 5 (ix)
	push	bc
	push	hl
	call	_sprintf
	ld	hl, #6
	add	hl, sp
	ld	sp, hl
	pop	de
;ui.c:75: unsigned char l = strlen(temp);
	ld	c, -3 (ix)
	ld	b, -2 (ix)
	push	bc
	call	_strlen
	pop	af
	ld	-1 (ix), l
;ui.c:76: for (char c = 0; c < l; c++)
	ld	c, #0x00
00105$:
	ld	a, c
	sub	a, -1 (ix)
	jr	NC,00107$
;ui.c:78: if (temp[c] == 0)
	ld	l, -3 (ix)
	ld	h, -2 (ix)
	ld	b, #0x00
	add	hl, bc
	ld	b, (hl)
	ld	a, b
	or	a, a
;ui.c:80: return;
	jr	Z,00107$
;ui.c:82: chram[p] = temp[c];
	ld	hl, #_chram
	add	hl, de
	ld	(hl), b
;ui.c:83: fgcolram[p] = color;
	ld	hl, #_fgcolram
	add	hl, de
	ld	a, 6 (ix)
	ld	(hl), a
;ui.c:84: p++;
	inc	de
;ui.c:76: for (char c = 0; c < l; c++)
	inc	c
	jr	00105$
00107$:
;ui.c:86: }
	ld	sp, ix
	pop	ix
	ret
;ui.c:89: void write_stringf(const char *format, char color, unsigned char x, unsigned char y, unsigned char data)
;	---------------------------------
; Function write_stringf
; ---------------------------------
_write_stringf::
	push	ix
	ld	ix,#0
	add	ix,sp
	ld	hl, #-33
	add	hl, sp
	ld	sp, hl
;ui.c:91: unsigned short p = (y * chram_cols) + x;
	ld	hl,#_chram_cols + 0
	ld	e, (hl)
	ld	h, 8 (ix)
	ld	l, #0x00
	ld	d, l
	ld	b, #0x08
00124$:
	add	hl, hl
	jr	NC,00125$
	add	hl, de
00125$:
	djnz	00124$
	ld	c, 7 (ix)
	ld	b, #0x00
	add	hl, bc
	ex	de, hl
;ui.c:93: sprintf(temp, format, data);
	ld	c, 9 (ix)
	ld	b, #0x00
	ld	hl, #0x0000
	add	hl, sp
	ld	-2 (ix), l
	ld	-1 (ix), h
	push	de
	push	bc
	ld	c, 4 (ix)
	ld	b, 5 (ix)
	push	bc
	push	hl
	call	_sprintf
	ld	hl, #6
	add	hl, sp
	ld	sp, hl
	pop	de
;ui.c:94: unsigned char l = strlen(temp);
	ld	c, -2 (ix)
	ld	b, -1 (ix)
	push	bc
	call	_strlen
	pop	af
	ld	-3 (ix), l
;ui.c:95: for (char c = 0; c < l; c++)
	ld	c, #0x00
00105$:
	ld	a, c
	sub	a, -3 (ix)
	jr	NC,00107$
;ui.c:97: if (temp[c] == 0)
	ld	l, -2 (ix)
	ld	h, -1 (ix)
	ld	b, #0x00
	add	hl, bc
	ld	b, (hl)
	ld	a, b
	or	a, a
;ui.c:99: return;
	jr	Z,00107$
;ui.c:101: chram[p] = temp[c];
	ld	hl, #_chram
	add	hl, de
	ld	(hl), b
;ui.c:102: fgcolram[p] = color;
	ld	hl, #_fgcolram
	add	hl, de
	ld	a, 6 (ix)
	ld	(hl), a
;ui.c:103: p++;
	inc	de
;ui.c:95: for (char c = 0; c < l; c++)
	inc	c
	jr	00105$
00107$:
;ui.c:105: }
	ld	sp, ix
	pop	ix
	ret
;ui.c:108: void write_stringf_ushort(const char *format, char color, unsigned char x, unsigned char y, unsigned short data)
;	---------------------------------
; Function write_stringf_ushort
; ---------------------------------
_write_stringf_ushort::
	push	ix
	ld	ix,#0
	add	ix,sp
	ld	hl, #-43
	add	hl, sp
	ld	sp, hl
;ui.c:110: unsigned short p = (y * chram_cols) + x;
	ld	hl,#_chram_cols + 0
	ld	e, (hl)
	ld	h, 8 (ix)
	ld	l, #0x00
	ld	d, l
	ld	b, #0x08
00124$:
	add	hl, hl
	jr	NC,00125$
	add	hl, de
00125$:
	djnz	00124$
	ld	c, 7 (ix)
	ld	b, #0x00
	add	hl, bc
	ex	de, hl
;ui.c:112: sprintf(temp, format, data);
	ld	hl, #0x0000
	add	hl, sp
	ld	-3 (ix), l
	ld	-2 (ix), h
	ld	c, l
	ld	b, h
	push	de
	ld	l, 9 (ix)
	ld	h, 10 (ix)
	push	hl
	ld	l, 4 (ix)
	ld	h, 5 (ix)
	push	hl
	push	bc
	call	_sprintf
	ld	hl, #6
	add	hl, sp
	ld	sp, hl
	pop	de
;ui.c:113: unsigned char l = strlen(temp);
	ld	c, -3 (ix)
	ld	b, -2 (ix)
	push	bc
	call	_strlen
	pop	af
	ld	-1 (ix), l
;ui.c:114: for (char c = 0; c < l; c++)
	ld	c, #0x00
00105$:
	ld	a, c
	sub	a, -1 (ix)
	jr	NC,00107$
;ui.c:116: if (temp[c] == 0)
	ld	l, -3 (ix)
	ld	h, -2 (ix)
	ld	b, #0x00
	add	hl, bc
	ld	b, (hl)
	ld	a, b
	or	a, a
;ui.c:118: return;
	jr	Z,00107$
;ui.c:120: chram[p] = temp[c];
	ld	hl, #_chram
	add	hl, de
	ld	(hl), b
;ui.c:121: fgcolram[p] = color;
	ld	hl, #_fgcolram
	add	hl, de
	ld	a, 6 (ix)
	ld	(hl), a
;ui.c:122: p++;
	inc	de
;ui.c:114: for (char c = 0; c < l; c++)
	inc	c
	jr	00105$
00107$:
;ui.c:124: }
	ld	sp, ix
	pop	ix
	ret
;ui.c:127: void write_stringf_short(const char *format, char color, unsigned char x, unsigned char y, signed short data)
;	---------------------------------
; Function write_stringf_short
; ---------------------------------
_write_stringf_short::
	push	ix
	ld	ix,#0
	add	ix,sp
	ld	hl, #-43
	add	hl, sp
	ld	sp, hl
;ui.c:129: unsigned short p = (y * chram_cols) + x;
	ld	hl,#_chram_cols + 0
	ld	e, (hl)
	ld	h, 8 (ix)
	ld	l, #0x00
	ld	d, l
	ld	b, #0x08
00124$:
	add	hl, hl
	jr	NC,00125$
	add	hl, de
00125$:
	djnz	00124$
	ld	c, 7 (ix)
	ld	b, #0x00
	add	hl, bc
	ex	de, hl
;ui.c:131: sprintf(temp, format, data);
	ld	hl, #0x0000
	add	hl, sp
	ld	-2 (ix), l
	ld	-1 (ix), h
	ld	c, l
	ld	b, h
	push	de
	ld	l, 9 (ix)
	ld	h, 10 (ix)
	push	hl
	ld	l, 4 (ix)
	ld	h, 5 (ix)
	push	hl
	push	bc
	call	_sprintf
	ld	hl, #6
	add	hl, sp
	ld	sp, hl
	pop	de
;ui.c:132: unsigned char l = strlen(temp);
	ld	c, -2 (ix)
	ld	b, -1 (ix)
	push	bc
	call	_strlen
	pop	af
	ld	-3 (ix), l
;ui.c:133: for (char c = 0; c < l; c++)
	ld	c, #0x00
00105$:
	ld	a, c
	sub	a, -3 (ix)
	jr	NC,00107$
;ui.c:135: if (temp[c] == 0)
	ld	l, -2 (ix)
	ld	h, -1 (ix)
	ld	b, #0x00
	add	hl, bc
	ld	b, (hl)
	ld	a, b
	or	a, a
;ui.c:137: return;
	jr	Z,00107$
;ui.c:139: chram[p] = temp[c];
	ld	hl, #_chram
	add	hl, de
	ld	(hl), b
;ui.c:140: fgcolram[p] = color;
	ld	hl, #_fgcolram
	add	hl, de
	ld	a, 6 (ix)
	ld	(hl), a
;ui.c:141: p++;
	inc	de
;ui.c:133: for (char c = 0; c < l; c++)
	inc	c
	jr	00105$
00107$:
;ui.c:143: }
	ld	sp, ix
	pop	ix
	ret
;ui.c:146: void write_stringf_ulong(const char *format, char color, unsigned char x, unsigned char y, unsigned long data)
;	---------------------------------
; Function write_stringf_ulong
; ---------------------------------
_write_stringf_ulong::
	push	ix
	ld	ix,#0
	add	ix,sp
	ld	hl, #-43
	add	hl, sp
	ld	sp, hl
;ui.c:148: unsigned short p = (y * chram_cols) + x;
	ld	hl,#_chram_cols + 0
	ld	e, (hl)
	ld	h, 8 (ix)
	ld	l, #0x00
	ld	d, l
	ld	b, #0x08
00124$:
	add	hl, hl
	jr	NC,00125$
	add	hl, de
00125$:
	djnz	00124$
	ld	c, 7 (ix)
	ld	b, #0x00
	add	hl, bc
	ex	de, hl
;ui.c:150: sprintf(temp, format, data);
	ld	hl, #0x0000
	add	hl, sp
	ld	-3 (ix), l
	ld	-2 (ix), h
	ld	c, l
	ld	b, h
	push	de
	ld	l, 11 (ix)
	ld	h, 12 (ix)
	push	hl
	ld	l, 9 (ix)
	ld	h, 10 (ix)
	push	hl
	ld	l, 4 (ix)
	ld	h, 5 (ix)
	push	hl
	push	bc
	call	_sprintf
	ld	hl, #8
	add	hl, sp
	ld	sp, hl
	pop	de
;ui.c:151: unsigned char l = strlen(temp);
	ld	c, -3 (ix)
	ld	b, -2 (ix)
	push	bc
	call	_strlen
	pop	af
	ld	-1 (ix), l
;ui.c:152: for (char c = 0; c < l; c++)
	ld	c, #0x00
00105$:
	ld	a, c
	sub	a, -1 (ix)
	jr	NC,00107$
;ui.c:154: if (temp[c] == 0)
	ld	l, -3 (ix)
	ld	h, -2 (ix)
	ld	b, #0x00
	add	hl, bc
	ld	b, (hl)
	ld	a, b
	or	a, a
;ui.c:156: return;
	jr	Z,00107$
;ui.c:158: chram[p] = temp[c];
	ld	hl, #_chram
	add	hl, de
	ld	(hl), b
;ui.c:159: fgcolram[p] = color;
	ld	hl, #_fgcolram
	add	hl, de
	ld	a, 6 (ix)
	ld	(hl), a
;ui.c:160: p++;
	inc	de
;ui.c:152: for (char c = 0; c < l; c++)
	inc	c
	jr	00105$
00107$:
;ui.c:162: }
	ld	sp, ix
	pop	ix
	ret
;ui.c:165: void write_char(unsigned char c, char color, unsigned char x, unsigned char y)
;	---------------------------------
; Function write_char
; ---------------------------------
_write_char::
	push	ix
	ld	ix,#0
	add	ix,sp
;ui.c:167: unsigned short p = (y * chram_cols) + x;
	ld	hl,#_chram_cols + 0
	ld	e, (hl)
	ld	h, 7 (ix)
	ld	l, #0x00
	ld	d, l
	ld	b, #0x08
00103$:
	add	hl, hl
	jr	NC,00104$
	add	hl, de
00104$:
	djnz	00103$
	ld	c, 6 (ix)
	ld	b, #0x00
	add	hl, bc
	ex	de, hl
;ui.c:168: chram[p] = c;
	ld	hl, #_chram+0
	add	hl, de
	ld	a, 4 (ix)
	ld	(hl), a
;ui.c:169: fgcolram[p] = color;
	ld	hl, #_fgcolram+0
	add	hl, de
	ld	a, 5 (ix)
	ld	(hl), a
;ui.c:170: }
	pop	ix
	ret
;ui.c:173: void write_char_row(unsigned char c, char color, unsigned char x, unsigned char y, unsigned char count)
;	---------------------------------
; Function write_char_row
; ---------------------------------
_write_char_row::
	push	ix
	ld	ix,#0
	add	ix,sp
;ui.c:175: unsigned short p = (y * chram_cols) + x;
	ld	hl,#_chram_cols + 0
	ld	e, (hl)
	ld	h, 7 (ix)
	ld	l, #0x00
	ld	d, l
	ld	b, #0x08
00118$:
	add	hl, hl
	jr	NC,00119$
	add	hl, de
00119$:
	djnz	00118$
	ld	c, 6 (ix)
	ld	b, #0x00
	add	hl, bc
	ex	de, hl
;ui.c:176: for (char b = 0; b < count; b++)
	ld	c, #0x00
00103$:
	ld	a, c
	sub	a, 8 (ix)
	jr	NC,00105$
;ui.c:178: chram[p] = c;
	ld	hl, #_chram+0
	add	hl, de
	ld	a, 4 (ix)
	ld	(hl), a
;ui.c:179: fgcolram[p] = color;
	ld	hl, #_fgcolram
	add	hl, de
	ld	a, 5 (ix)
	ld	(hl), a
;ui.c:180: p++;
	inc	de
;ui.c:176: for (char b = 0; b < count; b++)
	inc	c
	jr	00103$
00105$:
;ui.c:182: }
	pop	ix
	ret
;ui.c:185: void set_fgcolour(char color, char x, char y)
;	---------------------------------
; Function set_fgcolour
; ---------------------------------
_set_fgcolour::
	push	ix
	ld	ix,#0
	add	ix,sp
;ui.c:187: unsigned short p = (y * chram_cols) + x;
	ld	hl,#_chram_cols + 0
	ld	e, (hl)
	ld	h, 6 (ix)
	ld	l, #0x00
	ld	d, l
	ld	b, #0x08
00103$:
	add	hl, hl
	jr	NC,00104$
	add	hl, de
00104$:
	djnz	00103$
	ld	c, 5 (ix)
	ld	b, #0x00
	add	hl, bc
;ui.c:188: fgcolram[p] = color;
	ld	de, #_fgcolram+0
	add	hl, de
	ld	a, 4 (ix)
	ld	(hl), a
;ui.c:189: }
	pop	ix
	ret
;ui.c:192: void set_bgcolour(char color, char x, char y)
;	---------------------------------
; Function set_bgcolour
; ---------------------------------
_set_bgcolour::
	push	ix
	ld	ix,#0
	add	ix,sp
;ui.c:194: unsigned short p = (y * chram_cols) + x;
	ld	hl,#_chram_cols + 0
	ld	e, (hl)
	ld	h, 6 (ix)
	ld	l, #0x00
	ld	d, l
	ld	b, #0x08
00103$:
	add	hl, hl
	jr	NC,00104$
	add	hl, de
00104$:
	djnz	00103$
	ld	c, 5 (ix)
	ld	b, #0x00
	add	hl, bc
;ui.c:195: bgcolram[p] = color;
	ld	de, #_bgcolram+0
	add	hl, de
	ld	a, 4 (ix)
	ld	(hl), a
;ui.c:196: }
	pop	ix
	ret
;ui.c:199: void write_bgcol_row(char color, unsigned char x, unsigned char y, unsigned char count)
;	---------------------------------
; Function write_bgcol_row
; ---------------------------------
_write_bgcol_row::
	push	ix
	ld	ix,#0
	add	ix,sp
;ui.c:201: unsigned short p = (y * chram_cols) + x;
	ld	hl,#_chram_cols + 0
	ld	e, (hl)
	ld	h, 6 (ix)
	ld	l, #0x00
	ld	d, l
	ld	b, #0x08
00118$:
	add	hl, hl
	jr	NC,00119$
	add	hl, de
00119$:
	djnz	00118$
	ld	c, 5 (ix)
	ld	b, #0x00
	add	hl, bc
	ex	de, hl
;ui.c:202: for (char b = 0; b < count; b++)
	ld	c, #0x00
00103$:
	ld	a, c
	sub	a, 7 (ix)
	jr	NC,00105$
;ui.c:204: bgcolram[p] = color;
	ld	hl, #_bgcolram+0
	add	hl, de
	ld	a, 4 (ix)
	ld	(hl), a
;ui.c:205: p++;
	inc	de
;ui.c:202: for (char b = 0; b < count; b++)
	inc	c
	jr	00103$
00105$:
;ui.c:207: }
	pop	ix
	ret
;ui.c:210: void write_bits(char bits[], char multi, unsigned char first, unsigned char length, char color, unsigned char x, unsigned char y)
;	---------------------------------
; Function write_bits
; ---------------------------------
_write_bits::
	push	ix
	ld	ix,#0
	add	ix,sp
	push	af
	push	af
	dec	sp
;ui.c:212: for (char b = first; b < first + length; b++)
	ld	c, 7 (ix)
00107$:
	ld	e, 7 (ix)
	ld	d, #0x00
	ld	l, 8 (ix)
	ld	h, #0x00
	add	hl, de
	ld	a, c
	ld	e, #0x00
	sub	a, l
	ld	a, e
	sbc	a, h
	jp	PO, 00147$
	xor	a, #0x80
00147$:
	jp	P, 00109$
;ui.c:214: write_char((b) ? asc_1 : asc_0, color, x, y - 1);
	ld	d, 11 (ix)
	dec	d
	ld	a, c
	or	a, a
	jr	Z,00111$
	ld	hl,#_asc_1 + 0
	ld	b, (hl)
	jr	00112$
00111$:
	ld	hl,#_asc_0 + 0
	ld	b, (hl)
00112$:
	push	bc
	push	de
	inc	sp
	ld	h, 10 (ix)
	ld	l, 9 (ix)
	push	hl
	push	bc
	inc	sp
	call	_write_char
	pop	af
	pop	af
	pop	bc
;ui.c:215: char m = 0b00000001;
	ld	-1 (ix), #0x01
;ui.c:216: for (char i = 0; i < 8; i++)
	ld	e, 6 (ix)
	ld	h, c
	ld	l, #0x00
	ld	d, l
	ld	b, #0x08
00148$:
	add	hl, hl
	jr	NC,00149$
	add	hl, de
00149$:
	djnz	00148$
	ex	de, hl
	ld	a, 4 (ix)
	add	a, e
	ld	-4 (ix), a
	ld	a, 5 (ix)
	adc	a, d
	ld	-3 (ix), a
	ld	e, 10 (ix)
	ld	-2 (ix), #0x00
00104$:
;ui.c:219: x++;
	ld	a, e
	inc	a
	ld	-5 (ix), a
;ui.c:216: for (char i = 0; i < 8; i++)
	ld	a, -2 (ix)
	sub	a, #0x08
	jr	NC,00101$
;ui.c:218: write_char((bits[b * multi] & m) ? asc_1 : asc_0, color, x, y);
	ld	l, -4 (ix)
	ld	h, -3 (ix)
	ld	a, (hl)
	and	a, -1 (ix)
	or	a, a
	jr	Z,00113$
	ld	hl,#_asc_1 + 0
	ld	b, (hl)
	jr	00114$
00113$:
	ld	hl,#_asc_0 + 0
	ld	b, (hl)
00114$:
	push	bc
	ld	d, 11 (ix)
	push	de
	ld	a, 9 (ix)
	push	af
	inc	sp
	push	bc
	inc	sp
	call	_write_char
	pop	af
	pop	af
	pop	bc
;ui.c:219: x++;
	ld	e, -5 (ix)
;ui.c:220: m <<= 1;
	ld	a, -1 (ix)
	add	a, a
	ld	-1 (ix), a
;ui.c:216: for (char i = 0; i < 8; i++)
	inc	-2 (ix)
	jr	00104$
00101$:
;ui.c:222: x++;
	ld	a, -5 (ix)
	ld	10 (ix), a
;ui.c:212: for (char b = first; b < first + length; b++)
	inc	c
	jp	00107$
00109$:
;ui.c:224: }
	ld	sp, ix
	pop	ix
	ret
;ui.c:227: void box(unsigned char tx, unsigned char ty, unsigned char bx, unsigned char by, char c, char color)
;	---------------------------------
; Function box
; ---------------------------------
_box::
	push	ix
	ld	ix,#0
	add	ix,sp
;ui.c:229: for (unsigned char x = tx; x <= bx; x++)
	ld	b, 4 (ix)
00104$:
	ld	a, 6 (ix)
	sub	a, b
	jr	C,00101$
;ui.c:231: write_char(c, color, x, ty);
	push	bc
	ld	a, 5 (ix)
	push	af
	inc	sp
	push	bc
	inc	sp
	ld	h, 9 (ix)
	ld	l, 8 (ix)
	push	hl
	call	_write_char
	pop	af
	pop	af
	pop	bc
;ui.c:232: write_char(c, color, x, by);
	push	bc
	ld	a, 7 (ix)
	push	af
	inc	sp
	push	bc
	inc	sp
	ld	h, 9 (ix)
	ld	l, 8 (ix)
	push	hl
	call	_write_char
	pop	af
	pop	af
	pop	bc
;ui.c:229: for (unsigned char x = tx; x <= bx; x++)
	inc	b
	jr	00104$
00101$:
;ui.c:234: for (unsigned char y = ty + 1; y < by; y++)
	ld	b, 5 (ix)
	inc	b
00107$:
	ld	a, b
	sub	a, 7 (ix)
	jr	NC,00109$
;ui.c:236: write_char(c, color, tx, y);
	push	bc
	push	bc
	inc	sp
	ld	h, 4 (ix)
	ld	l, 9 (ix)
	push	hl
	ld	a, 8 (ix)
	push	af
	inc	sp
	call	_write_char
	pop	af
	pop	af
	pop	bc
;ui.c:237: write_char(c, color, bx, y);
	push	bc
	push	bc
	inc	sp
	ld	h, 6 (ix)
	ld	l, 9 (ix)
	push	hl
	ld	a, 8 (ix)
	push	af
	inc	sp
	call	_write_char
	pop	af
	pop	af
	pop	bc
;ui.c:234: for (unsigned char y = ty + 1; y < by; y++)
	inc	b
	jr	00107$
00109$:
;ui.c:239: }
	pop	ix
	ret
;ui.c:242: void panel(char tx, char ty, char bx, char by, char color)
;	---------------------------------
; Function panel
; ---------------------------------
_panel::
	push	ix
	ld	ix,#0
	add	ix,sp
;ui.c:244: write_char(char_corner_round_tl, color, tx, ty);
	ld	h, 5 (ix)
	ld	l, 4 (ix)
	push	hl
	ld	d, 8 (ix)
	ld	e,#0x95
	push	de
	call	_write_char
	pop	af
	pop	af
;ui.c:245: write_char(char_corner_round_tr, color, bx, ty);
	ld	h, 5 (ix)
	ld	l, 6 (ix)
	push	hl
	ld	d, 8 (ix)
	ld	e,#0x89
	push	de
	call	_write_char
	pop	af
	pop	af
;ui.c:246: write_char(char_corner_round_bl, color, tx, by);
	ld	h, 7 (ix)
	ld	l, 4 (ix)
	push	hl
	ld	d, 8 (ix)
	ld	e,#0x8a
	push	de
	call	_write_char
	pop	af
	pop	af
;ui.c:247: write_char(char_corner_round_br, color, bx, by);
	ld	h, 7 (ix)
	ld	l, 6 (ix)
	push	hl
	ld	d, 8 (ix)
	ld	e,#0x8b
	push	de
	call	_write_char
	pop	af
	pop	af
;ui.c:248: for (char x = tx + 1; x < bx; x++)
	ld	b, 4 (ix)
	inc	b
00104$:
	ld	a, b
	sub	a, 6 (ix)
	jr	NC,00101$
;ui.c:250: write_char(char_line_h, color, x, ty);
	push	bc
	ld	a, 5 (ix)
	push	af
	inc	sp
	push	bc
	inc	sp
	ld	d, 8 (ix)
	ld	e,#0x83
	push	de
	call	_write_char
	pop	af
	pop	af
	pop	bc
;ui.c:251: write_char(char_line_h, color, x, by);
	push	bc
	ld	a, 7 (ix)
	push	af
	inc	sp
	push	bc
	inc	sp
	ld	d, 8 (ix)
	ld	e,#0x83
	push	de
	call	_write_char
	pop	af
	pop	af
	pop	bc
;ui.c:248: for (char x = tx + 1; x < bx; x++)
	inc	b
	jr	00104$
00101$:
;ui.c:253: for (char y = ty + 1; y < by; y++)
	ld	b, 5 (ix)
	inc	b
00107$:
	ld	a, b
	sub	a, 7 (ix)
	jr	NC,00109$
;ui.c:255: write_char(char_line_v, color, tx, y);
	push	bc
	push	bc
	inc	sp
	ld	a, 4 (ix)
	push	af
	inc	sp
	ld	d, 8 (ix)
	ld	e,#0x82
	push	de
	call	_write_char
	pop	af
	pop	af
	pop	bc
;ui.c:256: write_char(char_line_v, color, bx, y);
	push	bc
	push	bc
	inc	sp
	ld	a, 6 (ix)
	push	af
	inc	sp
	ld	d, 8 (ix)
	ld	e,#0x82
	push	de
	call	_write_char
	pop	af
	pop	af
	pop	bc
;ui.c:253: for (char y = ty + 1; y < by; y++)
	inc	b
	jr	00107$
00109$:
;ui.c:258: }
	pop	ix
	ret
;ui.c:261: void panel_shaded(char tx, char ty, char bx, char by, char color_high, char color1, char color2)
;	---------------------------------
; Function panel_shaded
; ---------------------------------
_panel_shaded::
	push	ix
	ld	ix,#0
	add	ix,sp
;ui.c:263: write_char(char_corner_round_tl, color_high, tx, ty);
	ld	h, 5 (ix)
	ld	l, 4 (ix)
	push	hl
	ld	d, 8 (ix)
	ld	e,#0x95
	push	de
	call	_write_char
	pop	af
	pop	af
;ui.c:264: write_char(char_corner_round_tr, color1, bx, ty);
	ld	h, 5 (ix)
	ld	l, 6 (ix)
	push	hl
	ld	d, 9 (ix)
	ld	e,#0x89
	push	de
	call	_write_char
	pop	af
	pop	af
;ui.c:265: write_char(char_corner_round_bl, color1, tx, by);
	ld	h, 7 (ix)
	ld	l, 4 (ix)
	push	hl
	ld	d, 9 (ix)
	ld	e,#0x8a
	push	de
	call	_write_char
	pop	af
	pop	af
;ui.c:266: write_char(char_corner_round_br, color2, bx, by);
	ld	h, 7 (ix)
	ld	l, 6 (ix)
	push	hl
	ld	d, 10 (ix)
	ld	e,#0x8b
	push	de
	call	_write_char
	pop	af
	pop	af
;ui.c:267: for (char x = tx + 1; x < bx; x++)
	ld	b, 4 (ix)
	inc	b
00104$:
	ld	a, b
	sub	a, 6 (ix)
	jr	NC,00101$
;ui.c:269: write_char(char_line_h, color1, x, ty);
	push	bc
	ld	a, 5 (ix)
	push	af
	inc	sp
	push	bc
	inc	sp
	ld	d, 9 (ix)
	ld	e,#0x83
	push	de
	call	_write_char
	pop	af
	pop	af
	pop	bc
;ui.c:270: write_char(char_line_h, color2, x, by);
	push	bc
	ld	a, 7 (ix)
	push	af
	inc	sp
	push	bc
	inc	sp
	ld	d, 10 (ix)
	ld	e,#0x83
	push	de
	call	_write_char
	pop	af
	pop	af
	pop	bc
;ui.c:267: for (char x = tx + 1; x < bx; x++)
	inc	b
	jr	00104$
00101$:
;ui.c:272: for (char y = ty + 1; y < by; y++)
	ld	b, 5 (ix)
	inc	b
00107$:
	ld	a, b
	sub	a, 7 (ix)
	jr	NC,00109$
;ui.c:274: write_char(char_line_v, color1, tx, y);
	push	bc
	push	bc
	inc	sp
	ld	a, 4 (ix)
	push	af
	inc	sp
	ld	d, 9 (ix)
	ld	e,#0x82
	push	de
	call	_write_char
	pop	af
	pop	af
	pop	bc
;ui.c:275: write_char(char_line_v, color2, bx, y);
	push	bc
	push	bc
	inc	sp
	ld	a, 6 (ix)
	push	af
	inc	sp
	ld	d, 10 (ix)
	ld	e,#0x82
	push	de
	call	_write_char
	pop	af
	pop	af
	pop	bc
;ui.c:272: for (char y = ty + 1; y < by; y++)
	inc	b
	jr	00107$
00109$:
;ui.c:277: }
	pop	ix
	ret
;ui.c:279: void fill(char tx, char ty, char bx, char by, char c, char color)
;	---------------------------------
; Function fill
; ---------------------------------
_fill::
	push	ix
	ld	ix,#0
	add	ix,sp
;ui.c:281: for (char y = ty; y <= by; y++)
	ld	b, 5 (ix)
	ld	a, 6 (ix)
	sub	a, 4 (ix)
	ld	c, a
00103$:
	ld	a, 7 (ix)
	sub	a, b
	jr	C,00105$
;ui.c:283: write_char_row(c, color, tx, y, (bx - tx) + 1);
	ld	d, c
	inc	d
	push	bc
	ld	e, b
	push	de
	ld	h, 4 (ix)
	ld	l, 9 (ix)
	push	hl
	ld	a, 8 (ix)
	push	af
	inc	sp
	call	_write_char_row
	pop	af
	pop	af
	inc	sp
	pop	bc
;ui.c:281: for (char y = ty; y <= by; y++)
	inc	b
	jr	00103$
00105$:
;ui.c:285: }
	pop	ix
	ret
;ui.c:286: void fill_bgcol(char tx, char ty, char bx, char by, char color)
;	---------------------------------
; Function fill_bgcol
; ---------------------------------
_fill_bgcol::
	push	ix
	ld	ix,#0
	add	ix,sp
;ui.c:288: for (char y = ty; y <= by; y++)
	ld	c, 5 (ix)
	ld	a, 6 (ix)
	sub	a, 4 (ix)
	ld	b, a
00103$:
	ld	a, 7 (ix)
	sub	a, c
	jr	C,00105$
;ui.c:290: write_bgcol_row(color, tx, y, bx - tx);
	push	bc
	push	bc
	inc	sp
	ld	a, c
	push	af
	inc	sp
	ld	h, 4 (ix)
	ld	l, 8 (ix)
	push	hl
	call	_write_bgcol_row
	pop	af
	pop	af
	pop	bc
;ui.c:288: for (char y = ty; y <= by; y++)
	inc	c
	jr	00103$
00105$:
;ui.c:292: }
	pop	ix
	ret
;ui.c:295: void page_border(char color)
;	---------------------------------
; Function page_border
; ---------------------------------
_page_border::
;ui.c:297: panel(0, 0, 39, 29, color);
	ld	hl, #2+0
	add	hl, sp
	ld	d, (hl)
	ld	e,#0x1d
	push	de
	ld	a, #0x27
	push	af
	inc	sp
	xor	a, a
	push	af
	inc	sp
	xor	a, a
	push	af
	inc	sp
	call	_panel
	pop	af
	pop	af
	inc	sp
;ui.c:298: }
	ret
;ui.c:300: void draw_charactermap()
;	---------------------------------
; Function draw_charactermap
; ---------------------------------
_draw_charactermap::
;ui.c:302: char c = 94;
;ui.c:303: for (char y = 0; y < 29; y += 2)
	ld	bc, #0x005e
00107$:
	ld	a, b
	sub	a, #0x1d
	ret	NC
;ui.c:305: for (char x = 0; x < 39; x += 4)
	ld	d, #0x00
00104$:
;ui.c:308: write_char(c, 0, x, y + 1);
	ld	e, b
;ui.c:305: for (char x = 0; x < 39; x += 4)
	ld	a, d
	sub	a, #0x27
	jr	NC,00115$
;ui.c:307: write_stringf("%d", 0xFF, x, y, c);
	push	bc
	push	de
	ld	a, c
	push	af
	inc	sp
	ld	c, d
	push	bc
	ld	a, #0xff
	push	af
	inc	sp
	ld	hl, #___str_0
	push	hl
	call	_write_stringf
	ld	hl, #6
	add	hl, sp
	ld	sp, hl
	pop	de
	pop	bc
;ui.c:308: write_char(c, 0, x, y + 1);
	ld	h, e
	inc	h
	push	bc
	push	de
	ld	l, d
	push	hl
	xor	a, a
	ld	b, a
	push	bc
	call	_write_char
	pop	af
	pop	af
	pop	de
	pop	bc
;ui.c:309: c++;
	inc	c
;ui.c:305: for (char x = 0; x < 39; x += 4)
	inc	d
	inc	d
	inc	d
	inc	d
	jr	00104$
00115$:
;ui.c:303: for (char y = 0; y < 29; y += 2)
	ld	b, e
	inc	b
	inc	b
;ui.c:312: }
	jr	00107$
___str_0:
	.ascii "%d"
	.db 0x00
	.area _CODE
	.area _INITIALIZER
__xinit__asc_0:
	.db #0x30	; 48	'0'
__xinit__asc_1:
	.db #0x31	; 49	'1'
	.area _CABS (ABS)