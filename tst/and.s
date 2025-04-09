ORG $0000

start:
	lda #$FF
	and $F0
	nop
	and $F1,x
	nop
	and $F2,y
	nop
	lda #$FF
	and $FFF0
	nop
	and $FFF1,x
	nop
	and $FFF2,y

PAD $00F0,$0
	DSB 1,$FF
	DSB 1,$0F
	DSB 1,$F0

PAD $FFF0,$0
	DSB 1,$FF
	DSB 1,$0F
	DSB 1,$F0
