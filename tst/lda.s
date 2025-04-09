ORG $0000

start:
	lda #0
	nop
	lda #$80
	nop
	lda #1
	nop
	lda $4
	nop
	lda $3,x
	nop
	lda $E000
	nop
	lda $E001,x
	nop
	lda $E000,y
	nop
	lda ($04,x)
        nop
	lda ($04),y

PAD $E000,$0
	DSB 1,$FF
	DSB 1,$EE

PAD $EA80,$0
	DSB 1,$AB
