org $0000

start:
	lda #$7F
	ror a
	nop
	ror a
	nop
	ror $0
	nop
	ror $1,x
	nop
	ror $E000
	nop
	ror $E001,x

PAD $E000,$0
	DSB 1,$F0
	DSB 1,$EE
