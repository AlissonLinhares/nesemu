ORG $0000

start:
	lda #0
	stc
	adc #$1
	nop
	adc $4
	nop
	adc $3,x
	nop
	adc $E000
	nop
	adc $E001,x
	nop
	adc $E000,y
	nop
	adc ($04,x)
	nop
	adc ($04),y

PAD $65EA,$0
	DSB 1,$AB

PAD $E000,$0
	DSB 1,$FF
	DSB 1,$EE
