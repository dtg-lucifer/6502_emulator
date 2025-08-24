#ifndef CPU_STATE_H
#define CPU_STATE_H

#include "types.h"

// Structure to hold the CPU state for API usage
struct CpuState {
    // Register values
    byte A;   // Accumulator
    byte X;   // X index register
    byte Y;   // Y index register
    byte SP;  // Stack pointer
    word PC;  // Program counter

    // Status flags
    struct {
        byte C : 1;  // Carry Flag (bit 0)
        byte Z : 1;  // Zero Flag (bit 1)
        byte I : 1;  // Interrupt Disable Flag (bit 2)
        byte D : 1;  // Decimal Mode Flag (bit 3)
        byte B : 1;  // Break Flag (bit 4)
        byte U : 1;  // Unused/expansion (bit 5)
        byte V : 1;  // Overflow Flag (bit 6)
        byte N : 1;  // Negative Flag (bit 7)
    } flags;

    // Execution statistics
    i32 cycles_used;
    bool execution_completed;
};

#endif  // CPU_STATE_H
