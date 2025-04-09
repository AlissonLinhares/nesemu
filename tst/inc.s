ORG $0000

start:
	ldx #$FF
	nop
	inx
	nop
	inx
	nop
	ldy #$1
	nop
	iny
	nop
	iny
	nop
	inc $F0
	nop
	inc $F0,x
	nop
	inc $E000
	nop
	inc $E000,x

PAD $00F0,$0
	DSB 1,$01

PAD $E000,$0
	DSB 1,$FF
