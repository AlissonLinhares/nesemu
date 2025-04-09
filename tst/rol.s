org $0000

start:
	lda #$7F
	rol a
	nop
	rol a
	nop
	rol $0
	nop
	rol $1,x
	nop
	rol $E000
	nop
	rol $E001,x

PAD $E000,$0
	DSB 1,$F0
	DSB 1,$EE
