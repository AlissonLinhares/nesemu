ORG $0000

ldy #$01
lda #$03
sta $01
lda #$07
sta $02
ldx #$0a
stx $0704
lda ($01),y
nop

lda #$03
and #$03 ;mask out lowest 2 bits
clc
adc #2
sta $01
