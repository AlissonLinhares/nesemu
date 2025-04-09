ORG $0000

start:
	lda #$FF
	sta $F0
	sta $F1,x
	sta $FFF0
	sta $FFF1,x
	sta $FFF2,y
	sta ($F1,x)
	sta ($00),y
