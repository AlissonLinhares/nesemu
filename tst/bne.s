ORG $0000

start:
	adc #$0
	bne jump_exit
	nop
	cmp #$1
	bne jump_down
jump_up:
	nop
	bne jump_exit
jump_down:
	nop
	bne jump_up
jump_exit:
