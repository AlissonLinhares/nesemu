ORG $0000

start:
	lda #$00
	ora $F0
	nop
	ora $F1,x
	nop
	ora $F2,y
	nop
	lda #$00
	ora $FFF0
	nop
	ora $FFF1,x
	nop
	ora $FFF2,y

PAD $00F0,$0
	DSB 1,$00
	DSB 1,$0F
	DSB 1,$F0

PAD $FFF0,$0
	DSB 1,$00
	DSB 1,$0F
	DSB 1,$F0
