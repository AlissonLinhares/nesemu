ORG $0000

start:
	nop
	jmp jump_down
	nop
jump_up:
	nop
	jmp ($00F0)
	nop
jump_down:
	nop
	jmp jump_up
	nop
jump_exit:

PAD $00F0,$0
	DSW 1, jump_exit
