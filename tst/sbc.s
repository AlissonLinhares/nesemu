ORG $0000

start:
	lda #0
	stc
	sbc #$1
	nop
	clc
	sbc #$1
	nop
	sbc $4
	nop
	sbc $3,x
	nop
	sbc $E000
	nop
	sbc $E001,x
	nop
	sbc $E000,y
	nop
	sbc ($04,x)
	nop
	sbc ($04),y

PAD $65EA,$0
	DSB 1,$AB

PAD $E000,$0
	DSB 1,$FF
	DSB 1,$EE
