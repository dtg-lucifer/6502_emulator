#include "instructions.h"
#include "op_codes.h"

namespace instructions {

// STX Zero Page mode
void STX_ZP(Cpu& cpu, i32& cycles, Mem& mem) {
    // 1. Fetch the zero page address
    // 2. Store the value of the X register at that address
    // 3. Decrement cycles as needed
}

// STX Zero Page,Y mode
void STX_ZPY(Cpu& cpu, i32& cycles, Mem& mem) {
    // 1. Fetch the zero page address
    // 2. Add Y register to it (with zero page wraparound)
    // 3. Store the value of the X register at the calculated address
    // 4. Decrement cycles as needed
}

// STX Absolute mode
void STX_ABS(Cpu& cpu, i32& cycles, Mem& mem) {
    // 1. Fetch the absolute address (16-bit)
    // 2. Store the value of the X register at that address
    // 3. Decrement cycles as needed
}

}  // namespace instructions
