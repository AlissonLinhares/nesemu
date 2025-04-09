ORG $0000

start:
	adc #$1
	beq jump_exit
	nop
	cmp #$1
	beq jump_down
jump_up:
	nop
	beq jump_exit
jump_down:
	nop
	beq jump_up
jump_exit:
