ORG $0000

start:
	clc
	bcs jump_exit
	nop
	sec
	bcs jump_down
jump_up:
	nop
	bcs jump_exit
jump_down:
	nop
	bcs jump_up
jump_exit:
