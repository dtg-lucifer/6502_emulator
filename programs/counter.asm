.org $8000
start:
    LDX #$00
    STX $0200

loop:
    INX
    STX $0200
    JMP loop

.org $FFFC
    .word start
