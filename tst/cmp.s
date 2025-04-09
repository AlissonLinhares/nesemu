ORG $0000

start:
	lda #$02
	cmp $F0
	nop
	cmp $F1,x
	nop
	cmp $F2,y
	nop
	cmp $FFF0
	nop
	cmp $FFF1,x
	nop
	cmp $FFF2,y

PAD $00F0,$0
	DSB 1,$01
	DSB 1,$02
	DSB 1,$03

PAD $FFF0,$0
	DSB 1,$FF
	DSB 1,$F0
