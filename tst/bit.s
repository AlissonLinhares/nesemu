ORG $0000

start:
	lda #$aa
	bit $F0
	nop
	bit $F1
	nop
	bit $F2
	nop
	bit $FFF0
	nop
	bit $FFF1

PAD $00F0,$0
	DSB 1,$EE
	DSB 1,$AA
	DSB 1,$CC

PAD $FFF0,$0
	DSB 1,$0F
	DSB 1,$00
