ORG $0000

start:
	ldx #$FF
	stx $F0
	stx $F1,y
	stx $FFF0
