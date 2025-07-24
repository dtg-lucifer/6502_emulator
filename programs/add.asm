    ADR1 = $0400
    ADR2 = $0401
    ADR3 = $0402

    CLC             ; clear carry bit
    CLD             ; clear decimal bit

ADD LDA     ADR1    ; load the contents of ADR1 into accumulator
    ADC     ADR2    ; add the contents of ADR2 to accumulator
    STA     ADR3    ; save result to ADR3
