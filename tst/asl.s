org $0000

start:
	lda #$7F
	asl a
	nop
	asl a
	nop
	asl $0
	nop
	asl $1,x
	nop
	asl $E000
	nop
	asl $E001,x

PAD $E000,$0
	DSB 1,$F0
	DSB 1,$EE
