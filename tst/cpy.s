ORG $0000

start:
	ldy #$02
	cpy #$02
	nop
	cpy #$01
	nop
	cpy #$03
	nop
	cpy $F0
	nop
	cpy $FFF0

PAD $00F0,$0
	DSB 1,$01
	DSB 1,$02
	DSB 1,$03

PAD $FFF0,$0
	DSB 1,$FF
	DSB 1,$F0
