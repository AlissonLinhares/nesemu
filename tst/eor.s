ORG $0000

start:
	lda #$00
	eor $F0
	nop
	eor $F1,x
	nop
	eor $F2,y
	nop
	lda #$00
	eor $FFF0
	nop
	eor $FFF1,x
	nop
	eor $FFF2,y

PAD $00F0,$0
	DSB 1,$00
	DSB 1,$0F
	DSB 1,$F0

PAD $FFF0,$0
	DSB 1,$00
	DSB 1,$0F
	DSB 1,$F0
