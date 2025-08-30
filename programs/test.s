; Simple test program for the 6502 assembler
; Tests various instruction types and addressing modes

.org $2000

start:
    ; Test immediate loading
    LDA #$42        ; Load accumulator with $42
    LDX #$10        ; Load X with $10
    LDY #$20        ; Load Y with $20

    ; Test zero page operations
    STA $80         ; Store A to zero page
    STX $81         ; Store X to zero page
    STY $82         ; Store Y to zero page

    ; Test absolute addressing
    STA $1000       ; Store A to absolute address
    STX $1001       ; Store X to absolute address
    STY $1002       ; Store Y to absolute address

    ; Test indexed addressing
    LDA $80,X       ; Load from zero page + X
    STA $1000,Y     ; Store to absolute + Y

    ; Test arithmetic
    ADC #$01        ; Add 1 to accumulator
    SBC #$01        ; Subtract 1 from accumulator

    ; Test logical operations
    AND #$0F        ; AND with $0F
    ORA #$F0        ; OR with $F0
    EOR #$FF        ; XOR with $FF

    ; Test comparisons
    CMP #$42        ; Compare A with $42
    CPX #$10        ; Compare X with $10
    CPY #$20        ; Compare Y with $20

    ; Test increment/decrement
    INX             ; Increment X
    INY             ; Increment Y
    DEX             ; Decrement X
    DEY             ; Decrement Y

    ; Test register transfers
    TAX             ; Transfer A to X
    TAY             ; Transfer A to Y
    TXA             ; Transfer X to A
    TYA             ; Transfer Y to A

    ; Test stack operations
    PHA             ; Push A to stack
    PHP             ; Push processor status
    PLA             ; Pull A from stack
    PLP             ; Pull processor status

    ; Test branch (forward)
    BEQ end         ; Branch if zero
    BNE continue    ; Branch if not zero

continue:
    ; Test subroutine
    JSR subroutine  ; Call subroutine

    ; Test jumps
    JMP end         ; Jump to end

subroutine:
    ; Simple subroutine
    LDA #$FF        ; Load $FF
    RTS             ; Return from subroutine

end:
    ; Test status flags
    CLC             ; Clear carry
    SEC             ; Set carry
    CLI             ; Clear interrupt
    SEI             ; Set interrupt

    ; Test shifts
    ASL A           ; Arithmetic shift left
    LSR A           ; Logical shift right
    ROL A           ; Rotate left
    ROR A           ; Rotate right

    ; Infinite loop
loop:
    NOP             ; No operation
    JMP loop        ; Jump to self

; Data section
.org $1100
data:
    .byte $01       ; Single byte
    .byte $02       ; Another byte
    .word $1234     ; 16-bit word

; Reset vector
.org $FFFC
    .word start     ; Point to start of program
