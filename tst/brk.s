ORG $0000

start:
	nop
	brk

PAD $FF00, 0

IRQ:
	nop
	sta $FFFE
	sta $FFFF
	rti

ORG $FFFA
	dw 0
	dw 0
	dw IRQ
