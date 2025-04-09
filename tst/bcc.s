ORG $0000

start:
	sec
	bcc jump_exit
	nop
	clc
	bcc jump_down
jump_up:
	nop
	bcc jump_exit
jump_down:
	nop
	bcc jump_up
jump_exit:
