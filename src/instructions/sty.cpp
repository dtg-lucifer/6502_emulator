#include "instructions.h"
#include "op_codes.h"

namespace instructions {

// STY Zero Page mode
void STY_ZP(Cpu& cpu, i32& cycles, Mem& mem) {
    // 1. Fetch the zero page address
    // 2. Store the value of the Y register at that address
    // 3. Decrement cycles as needed
}

// STY Zero Page,X mode
void STY_ZPX(Cpu& cpu, i32& cycles, Mem& mem) {
    // 1. Fetch the zero page address
    // 2. Add X register to it (with zero page wraparound)
    // 3. Store the value of the Y register at the calculated address
    // 4. Decrement cycles as needed
}

// STY Absolute mode
void STY_ABS(Cpu& cpu, i32& cycles, Mem& mem) {
    // 1. Fetch the absolute address (16-bit)
    // 2. Store the value of the Y register at that address
    // 3. Decrement cycles as needed
}

}  // namespace instructions
