ORG $0000

start:
	ldy #0
	nop
	ldy #$80
	nop
	ldy #1
	nop
	ldy $4
	nop
	ldy $3,x
	nop
	ldy $E000
	nop
	ldy $E000,x

PAD $E000,$0
	DSB 1,$FF
	DSB 1,$EE
