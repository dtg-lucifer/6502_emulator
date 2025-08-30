; Comprehensive 6502 Opcode Test Program
; Tests all opcodes defined in op_codes.h to ensure assembler coverage

.org $2000

start:
    ; ===========================================
    ; Load/Store Operations - LDA variants
    ; ===========================================
    LDA #$42        ; LDA_IM - Load Accumulator Immediate
    LDA $80         ; LDA_ZP - Load Accumulator Zero Page
    LDA $80,X       ; LDA_ZPX - Load Accumulator Zero Page,X
    LDA $1000       ; LDA_AB - Load Accumulator Absolute
    LDA $1000,X     ; LDA_ABSX - Load Accumulator Absolute,X
    LDA $1000,Y     ; LDA_ABSY - Load Accumulator Absolute,Y
    LDA ($80,X)     ; LDA_INX - Load Accumulator (Indirect,X)
    LDA ($80),Y     ; LDA_INY - Load Accumulator (Indirect),Y

    ; ===========================================
    ; LDX variants
    ; ===========================================
    LDX #$10        ; LDX_IM - Load X Register Immediate
    LDX $80         ; LDX_ZP - Load X Register Zero Page
    LDX $80,Y       ; LDX_ZPY - Load X Register Zero Page,Y
    LDX $1000       ; LDX_AB - Load X Register Absolute
    LDX $1000,Y     ; LDX_ABSY - Load X Register Absolute,Y

    ; ===========================================
    ; LDY variants
    ; ===========================================
    LDY #$20        ; LDY_IM - Load Y Register Immediate
    LDY $80         ; LDY_ZP - Load Y Register Zero Page
    LDY $80,X       ; LDY_ZPX - Load Y Register Zero Page,X
    LDY $1000       ; LDY_AB - Load Y Register Absolute
    LDY $1000,X     ; LDY_ABSX - Load Y Register Absolute,X

    ; ===========================================
    ; STA variants
    ; ===========================================
    STA $80         ; STA_ZP - Store Accumulator Zero Page
    STA $80,X       ; STA_ZPX - Store Accumulator Zero Page,X
    STA $1000       ; STA_ABS - Store Accumulator Absolute
    STA $1000,X     ; STA_ABSX - Store Accumulator Absolute,X
    STA $1000,Y     ; STA_ABSY - Store Accumulator Absolute,Y
    STA ($80,X)     ; STA_INX - Store Accumulator (Indirect,X)
    STA ($80),Y     ; STA_INY - Store Accumulator (Indirect),Y

    ; ===========================================
    ; STX variants
    ; ===========================================
    STX $80         ; STX_ZP - Store X Register Zero Page
    STX $80,Y       ; STX_ZPY - Store X Register Zero Page,Y
    STX $1000       ; STX_ABS - Store X Register Absolute

    ; ===========================================
    ; STY variants
    ; ===========================================
    STY $80         ; STY_ZP - Store Y Register Zero Page
    STY $80,X       ; STY_ZPX - Store Y Register Zero Page,X
    STY $1000       ; STY_ABS - Store Y Register Absolute

    ; ===========================================
    ; Register Transfer Operations
    ; ===========================================
    TAX             ; TAX - Transfer Accumulator to X
    TAY             ; TAY - Transfer Accumulator to Y
    TXA             ; TXA - Transfer X to Accumulator
    TYA             ; TYA - Transfer Y to Accumulator
    TSX             ; TSX - Transfer Stack Pointer to X
    TXS             ; TXS - Transfer X to Stack Pointer

    ; ===========================================
    ; Stack Operations
    ; ===========================================
    PHA             ; PHA - Push Accumulator on Stack
    PHP             ; PHP - Push Processor Status on Stack
    PLA             ; PLA - Pull Accumulator from Stack
    PLP             ; PLP - Pull Processor Status from Stack

    ; ===========================================
    ; Logical Operations - AND variants
    ; ===========================================
    AND #$0F        ; AND_IM - AND Accumulator with Immediate
    AND $80         ; AND_ZP - AND Accumulator with Zero Page
    AND $80,X       ; AND_ZPX - AND Accumulator with Zero Page,X
    AND $1000       ; AND_AB - AND Accumulator with Absolute
    AND $1000,X     ; AND_ABSX - AND Accumulator with Absolute,X
    AND $1000,Y     ; AND_ABSY - AND Accumulator with Absolute,Y
    AND ($80,X)     ; AND_INX - AND Accumulator with (Indirect,X)
    AND ($80),Y     ; AND_INY - AND Accumulator with (Indirect),Y

    ; ===========================================
    ; EOR variants
    ; ===========================================
    EOR #$FF        ; EOR_IM - XOR Accumulator with Immediate
    EOR $80         ; EOR_ZP - XOR Accumulator with Zero Page
    EOR $80,X       ; EOR_ZPX - XOR Accumulator with Zero Page,X
    EOR $1000       ; EOR_AB - XOR Accumulator with Absolute
    EOR $1000,X     ; EOR_ABSX - XOR Accumulator with Absolute,X
    EOR $1000,Y     ; EOR_ABSY - XOR Accumulator with Absolute,Y
    EOR ($80,X)     ; EOR_INX - XOR Accumulator with (Indirect,X)
    EOR ($80),Y     ; EOR_INY - XOR Accumulator with (Indirect),Y

    ; ===========================================
    ; ORA variants
    ; ===========================================
    ORA #$F0        ; ORA_IM - OR Accumulator with Immediate
    ORA $80         ; ORA_ZP - OR Accumulator with Zero Page
    ORA $80,X       ; ORA_ZPX - OR Accumulator with Zero Page,X
    ORA $1000       ; ORA_AB - OR Accumulator with Absolute
    ORA $1000,X     ; ORA_ABSX - OR Accumulator with Absolute,X
    ORA $1000,Y     ; ORA_ABSY - OR Accumulator with Absolute,Y
    ORA ($80,X)     ; ORA_INX - OR Accumulator with (Indirect,X)
    ORA ($80),Y     ; ORA_INY - OR Accumulator with (Indirect),Y

    ; ===========================================
    ; BIT variants
    ; ===========================================
    BIT $80         ; BIT_ZP - Test Bits in Memory Zero Page
    BIT $1000       ; BIT_AB - Test Bits in Memory Absolute

    ; ===========================================
    ; Arithmetic Operations - ADC variants
    ; ===========================================
    ADC #$01        ; ADC_IM - Add with Carry Immediate
    ADC $80         ; ADC_ZP - Add with Carry Zero Page
    ADC $80,X       ; ADC_ZPX - Add with Carry Zero Page,X
    ADC $1000       ; ADC_AB - Add with Carry Absolute
    ADC $1000,X     ; ADC_ABSX - Add with Carry Absolute,X
    ADC $1000,Y     ; ADC_ABSY - Add with Carry Absolute,Y
    ADC ($80,X)     ; ADC_INX - Add with Carry (Indirect,X)
    ADC ($80),Y     ; ADC_INY - Add with Carry (Indirect),Y

    ; ===========================================
    ; SBC variants
    ; ===========================================
    SBC #$01        ; SBC_IM - Subtract with Carry Immediate
    SBC $80         ; SBC_ZP - Subtract with Carry Zero Page
    SBC $80,X       ; SBC_ZPX - Subtract with Carry Zero Page,X
    SBC $1000       ; SBC_AB - Subtract with Carry Absolute
    SBC $1000,X     ; SBC_ABSX - Subtract with Carry Absolute,X
    SBC $1000,Y     ; SBC_ABSY - Subtract with Carry Absolute,Y
    SBC ($80,X)     ; SBC_INX - Subtract with Carry (Indirect,X)
    SBC ($80),Y     ; SBC_INY - Subtract with Carry (Indirect),Y

    ; ===========================================
    ; Compare Operations - CMP variants
    ; ===========================================
    CMP #$42        ; CMP_IM - Compare Accumulator Immediate
    CMP $80         ; CMP_ZP - Compare Accumulator Zero Page
    CMP $80,X       ; CMP_ZPX - Compare Accumulator Zero Page,X
    CMP $1000       ; CMP_AB - Compare Accumulator Absolute
    CMP $1000,X     ; CMP_ABSX - Compare Accumulator Absolute,X
    CMP $1000,Y     ; CMP_ABSY - Compare Accumulator Absolute,Y
    CMP ($80,X)     ; CMP_INX - Compare Accumulator (Indirect,X)
    CMP ($80),Y     ; CMP_INY - Compare Accumulator (Indirect),Y

    ; ===========================================
    ; CPX variants
    ; ===========================================
    CPX #$10        ; CPX_IM - Compare X Register Immediate
    CPX $80         ; CPX_ZP - Compare X Register Zero Page
    CPX $1000       ; CPX_AB - Compare X Register Absolute

    ; ===========================================
    ; CPY variants
    ; ===========================================
    CPY #$20        ; CPY_IM - Compare Y Register Immediate
    CPY $80         ; CPY_ZP - Compare Y Register Zero Page
    CPY $1000       ; CPY_AB - Compare Y Register Absolute

    ; ===========================================
    ; Increment and Decrement - INC variants
    ; ===========================================
    INC $80         ; INC_ZP - Increment Memory Zero Page
    INC $80,X       ; INC_ZPX - Increment Memory Zero Page,X
    INC $1000       ; INC_AB - Increment Memory Absolute
    INC $1000,X     ; INC_ABSX - Increment Memory Absolute,X
    INX             ; INX - Increment X Register
    INY             ; INY - Increment Y Register

    ; ===========================================
    ; DEC variants
    ; ===========================================
    DEC $80         ; DEC_ZP - Decrement Memory Zero Page
    DEC $80,X       ; DEC_ZPX - Decrement Memory Zero Page,X
    DEC $1000       ; DEC_AB - Decrement Memory Absolute
    DEC $1000,X     ; DEC_ABSX - Decrement Memory Absolute,X
    DEX             ; DEX - Decrement X Register
    DEY             ; DEY - Decrement Y Register

    ; ===========================================
    ; Shifts and Rotates - ASL variants
    ; ===========================================
    ASL A           ; ASL_A - Arithmetic Shift Left Accumulator
    ASL $80         ; ASL_ZP - Arithmetic Shift Left Zero Page
    ASL $80,X       ; ASL_ZPX - Arithmetic Shift Left Zero Page,X
    ASL $1000       ; ASL_AB - Arithmetic Shift Left Absolute
    ASL $1000,X     ; ASL_ABSX - Arithmetic Shift Left Absolute,X

    ; ===========================================
    ; LSR variants
    ; ===========================================
    LSR A           ; LSR_A - Logical Shift Right Accumulator
    LSR $80         ; LSR_ZP - Logical Shift Right Zero Page
    LSR $80,X       ; LSR_ZPX - Logical Shift Right Zero Page,X
    LSR $1000       ; LSR_AB - Logical Shift Right Absolute
    LSR $1000,X     ; LSR_ABSX - Logical Shift Right Absolute,X

    ; ===========================================
    ; ROL variants
    ; ===========================================
    ROL A           ; ROL_A - Rotate Left Accumulator
    ROL $80         ; ROL_ZP - Rotate Left Zero Page
    ROL $80,X       ; ROL_ZPX - Rotate Left Zero Page,X
    ROL $1000       ; ROL_AB - Rotate Left Absolute
    ROL $1000,X     ; ROL_ABSX - Rotate Left Absolute,X

    ; ===========================================
    ; ROR variants
    ; ===========================================
    ROR A           ; ROR_A - Rotate Right Accumulator
    ROR $80         ; ROR_ZP - Rotate Right Zero Page
    ROR $80,X       ; ROR_ZPX - Rotate Right Zero Page,X
    ROR $1000       ; ROR_AB - Rotate Right Absolute
    ROR $1000,X     ; ROR_ABSX - Rotate Right Absolute,X

    ; ===========================================
    ; Jump and Branch Operations
    ; ===========================================
    JSR subroutine  ; JSR - Jump to Subroutine
    JMP end_test    ; JMP - Jump Absolute

subroutine:
    RTS             ; RTS - Return from Subroutine

jump_test:
    JMP ($1500)     ; JMPI - Jump Indirect

branch_test:
    ; ===========================================
    ; Branch Operations
    ; ===========================================
    BCC branch_target   ; BCC - Branch if Carry Clear
    BCS branch_target   ; BCS - Branch if Carry Set
    BEQ branch_target   ; BEQ - Branch if Equal (Zero Set)
    BMI branch_target   ; BMI - Branch if Minus (Negative Set)
    BNE branch_target   ; BNE - Branch if Not Equal (Zero Clear)
    BPL branch_target   ; BPL - Branch if Plus (Negative Clear)
    BVC branch_target   ; BVC - Branch if Overflow Clear
    BVS branch_target   ; BVS - Branch if Overflow Set

branch_target:
    NOP             ; Branch target

    ; ===========================================
    ; Status Flag Operations
    ; ===========================================
    CLC             ; CLC - Clear Carry Flag
    CLD             ; CLD - Clear Decimal Mode Flag
    CLI             ; CLI - Clear Interrupt Disable Flag
    CLV             ; CLV - Clear Overflow Flag
    SEC             ; SEC - Set Carry Flag
    SED             ; SED - Set Decimal Mode Flag
    SEI             ; SEI - Set Interrupt Disable Flag

    ; ===========================================
    ; No Operation and Break
    ; ===========================================
    NOP             ; NOP - No Operation
    BRK             ; BRK - Force Interrupt (Break)

end_test:
    ; Infinite loop to end test
loop:
    NOP
    JMP loop

; ===========================================
; Data Section
; ===========================================
.org $1500
indirect_target:
    .word jump_test     ; Target for indirect jump

; ===========================================
; Reset Vector
; ===========================================
.org $FFFC
    .word start         ; Reset vector points to start
