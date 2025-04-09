org $0000

jsr init
jsr loop
jsr end
nop

init:
	ldx #$00
	nop
	rts

loop:
	inx
	nop
	cpx #$03
	bne loop
	rts

end:
  brk
