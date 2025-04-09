ORG $0000

start:
	lda #$FF
	lsr a
	nop
	lsr a
	nop
	lsr $0
	nop
	lsr $1,x
	nop
	lsr $E000
	nop
	lsr $E001,x

PAD $E000,$0
	DSB 1,$0C
	DSB 1,$01
