ORG $0000

start:
	ldx #$FF
	nop
	dex
	nop
	dex
	nop
	ldy #$1
	nop
	dey
	nop
	dey
	nop
	dec $F0
	nop
	dec $F0,x
	nop
	dec $E000
	nop
	dec $E000,x

PAD $00F0,$0
	DSB 1,$01

PAD $E000,$0
	DSB 1,$FF
