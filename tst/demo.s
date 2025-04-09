ORG $0000

jmp init
;  ___           _        __ ___  __ ___
; / __|_ _  __ _| |_____ / /| __|/  \_  )
; \__ \ ' \/ _` | / / -_) _ \__ \ () / /
; |___/_||_\__,_|_\_\___\___/___/\__/___|

; Change direction: W A S D

PAD $0600, 0

; Directions (each using a separate bit)
movingUp       EQU 1
movingRight    EQU 2
movingDown     EQU 4
movingLeft     EQU 8
joypad         EQU $4016

button_up      EQU #$8
button_down    EQU #$4
button_left    EQU #$2
button_right   EQU #$1

black          EQU #$0F
white          EQU #$30

appleL         EQU $00 ; screen location of apple, low byte
appleH         EQU $01 ; screen location of apple, high byte
snakeHeadL     EQU $10 ; screen location of snake head, low byte
snakeHeadH     EQU $11 ; screen location of snake head, high byte
snakeBodyStart EQU $12 ; start of snake body byte pairs
snakeDirection EQU $02 ; direction (possible values are below)
snakeLength    EQU $03 ; snake length, in bytes


init:
  jsr initSnake
  jsr generateApplePosition
loop:
  jsr readKeys
  jsr checkCollision
  jsr updateSnake
  jsr drawApple
  jsr drawSnake
  jsr spinWheels
  jmp loop


initSnake:
  lda #movingRight  ;start direction
  sta snakeDirection

  lda #4  ;start length (2 segments)
  sta snakeLength

  lda #$11
  sta snakeHeadL

  lda #$10
  sta snakeBodyStart

  lda #$0f
  sta $14 ; body segment 1

  lda #$04
  sta snakeHeadH
  sta $13 ; body segment 1
  sta $15 ; body segment 2
  rts


generateApplePosition:
  ;load a new random byte into $00
  jsr getRandom
  sta appleL

  ;load a new random number from 2 to 5 into $01
  jsr getRandom
  and #$03 ;mask out lowest 2 bits

  clc
  adc #2
  sta appleH
  rts


; https://wiki.nesdev.com/w/index.php/Controller_reading_code
; At the same time that we strobe bit 0, we initialize the ring counter
; so we're hitting two birds with one stone here
readJoy:
  lda #$01
  ; While the strobe bit is set, buttons will be continuously reloaded.
  ; This means that reading from joypad will only return the state of the
  ; first button: button A.
  sta joypad
  sta buttons
  lsr a        ; now A is 0
  ; By storing 0 into joypad, the strobe bit is cleared and the reloading stops.
  ; This allows all 8 buttons (newly reloaded) to be read from joypad.
  sta joypad
readloop:
  lda joypad
  lsr a	       ; bit 0 -> Carry
  rol buttons  ; Carry -> bit 0; bit 7 -> Carry
  bcc readloop
  rts

readKeys:
  jsr readJoy
  lda button_up
  bit buttons
  bne upKey
  lda button_right
  bit buttons
  bne rightKey
  lda button_down
  bit buttons
  bne downKey
  lda button_left
  bit buttons
  bne leftKey
  rts
upKey:
  lda #movingDown
  bit snakeDirection
  bne illegalMove

  lda #movingUp
  sta snakeDirection
  rts
rightKey:
  lda #movingLeft
  bit snakeDirection
  bne illegalMove

  lda #movingRight
  sta snakeDirection
  rts
downKey:
  lda #movingUp
  bit snakeDirection
  bne illegalMove

  lda #movingDown
  sta snakeDirection
  rts
leftKey:
  lda #movingRight
  bit snakeDirection
  bne illegalMove

  lda #movingLeft
  sta snakeDirection
  rts
illegalMove:
  rts


checkCollision:
  jsr checkAppleCollision
  jsr checkSnakeCollision
  rts


checkAppleCollision:
  lda appleL
  cmp snakeHeadL
  bne doneCheckingAppleCollision
  lda appleH
  cmp snakeHeadH
  bne doneCheckingAppleCollision

  ;eat apple
  inc snakeLength
  inc snakeLength ;increase length
  jsr generateApplePosition
doneCheckingAppleCollision:
  rts


checkSnakeCollision:
  ldx #2 ;start with second segment
snakeCollisionLoop:
  lda snakeHeadL,x
  cmp snakeHeadL
  bne continueCollisionLoop

maybeCollided:
  lda snakeHeadH,x
  cmp snakeHeadH
  beq didCollide

continueCollisionLoop:
  inx
  inx
  cpx snakeLength          ;got to last section with no collision
  beq didntCollide
  jmp snakeCollisionLoop

didCollide:
  jmp gameOver
didntCollide:
  rts


updateSnake:
  ldx snakeLength
  dex
  txa
updateloop:
  lda snakeHeadL,x
  sta snakeBodyStart,x
  dex
  bpl updateloop

  lda snakeDirection
  lsr
  bcs up
  lsr
  bcs right
  lsr
  bcs down
  lsr
  bcs left
up:
  lda snakeHeadL
  sec
  sbc #$20
  sta snakeHeadL
  bcc upup
  rts
upup:
  dec snakeHeadH
  lda #$1
  cmp snakeHeadH
  beq collision
  rts
right:
  inc snakeHeadL
  lda #$1f
  bit snakeHeadL
  beq collision
  rts
down:
  lda snakeHeadL
  clc
  adc #$20
  sta snakeHeadL
  bcs downdown
  rts
downdown:
  inc snakeHeadH
  lda #$6
  cmp snakeHeadH
  beq collision
  rts
left:
  dec snakeHeadL
  lda snakeHeadL
  and #$1f
  cmp #$1f
  beq collision
  rts
collision:
  jmp gameOver


drawApple:
  ldy #0
  jsr getRandom
  sta (appleL),y
  rts

drawSnake:
  ldx snakeLength
  lda #black
  sta (snakeHeadL,x) ; erase end of tail

  ldx #0
  lda white
  sta (snakeHeadL,x) ; paint head
  rts


spinWheels:
  ldx #0
spinloop:
  dex
  bne spinloop
  rts

getRandom:
	lda sysRandom
	rol sysRandom
	eor sysRandom
	rts

gameOver:

; System variables
sysRandom      DB #3
buttons        DB 0
