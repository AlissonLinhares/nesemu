ORG $0000

start:
	ldx #$02
	cpx #$02
	nop
	cpx #$01
	nop
	cpx #$03
	nop
	cpx $F0
	nop
	cpx $FFF0

PAD $00F0,$0
	DSB 1,$01
	DSB 1,$02
	DSB 1,$03

PAD $FFF0,$0
	DSB 1,$FF
	DSB 1,$F0
