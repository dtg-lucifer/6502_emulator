#include "instructions.h"
#include "op_codes.h"

namespace instructions {

// STA Zero Page mode
void STA_ZP(Cpu& cpu, i32& cycles, Mem& mem) {
    // 1. Fetch the zero page address
    // 2. Store the value of the accumulator at that address
    // 3. Decrement cycles as needed
}

// STA Zero Page,X mode
void STA_ZPX(Cpu& cpu, i32& cycles, Mem& mem) {
    // 1. Fetch the zero page address
    // 2. Add X register to it (with zero page wraparound)
    // 3. Store the value of the accumulator at the calculated address
    // 4. Decrement cycles as needed
}

// STA Absolute mode
void STA_ABS(Cpu& cpu, i32& cycles, Mem& mem) {
    // 1. Fetch the absolute address (16-bit)
    // 2. Store the value of the accumulator at that address
    // 3. Decrement cycles as needed
}

// STA Absolute,X mode
void STA_ABSX(Cpu& cpu, i32& cycles, Mem& mem) {
    // 1. Fetch the absolute address (16-bit)
    // 2. Add X register to it
    // 3. Store the value of the accumulator at the calculated address
    // 4. Decrement cycles as needed
}

// STA Absolute,Y mode
void STA_ABSY(Cpu& cpu, i32& cycles, Mem& mem) {
    // 1. Fetch the absolute address (16-bit)
    // 2. Add Y register to it
    // 3. Store the value of the accumulator at the calculated address
    // 4. Decrement cycles as needed
}

// STA Indirect,X mode
void STA_INX(Cpu& cpu, i32& cycles, Mem& mem) {
    // 1. Fetch zero page address
    // 2. Add X to it (with zero page wraparound)
    // 3. Read the effective address from the zero page (2 bytes)
    // 4. Store the value of the accumulator at the effective address
    // 5. Decrement cycles as needed
}

// STA Indirect,Y mode
void STA_INY(Cpu& cpu, i32& cycles, Mem& mem) {
    // 1. Fetch zero page address
    // 2. Read the base address from the zero page (2 bytes)
    // 3. Add Y to the base address
    // 4. Store the value of the accumulator at the calculated address
    // 5. Decrement cycles as needed
}

}  // namespace instructions
