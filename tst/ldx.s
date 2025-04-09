ORG $0000

start:
	ldx #0
	nop
	ldx #$80
	nop
	ldx #1
	nop
	ldx $4
	nop
	ldx $3,y
	nop
	ldx $E000
	nop
	ldx $E000,y

PAD $E000,$0
	DSB 1,$FF
	DSB 1,$EE
