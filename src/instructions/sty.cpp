#include "instructions.h"
#include "op_codes.h"

namespace instructions {

// STY Zero Page mode
void STY_ZP(Cpu& cpu, i32& cycles, Mem& mem) {
    // 1. Fetch the zero page address
    byte zp_addr = cpu.fetch_byte(cycles, mem);
    // 2. Store the value of the Y register at that address
    mem[zp_addr] = cpu.get(Register::Y);
    // 3. Decrement cycles as needed
    cycles -= 2;
}

// STY Zero Page,X mode
void STY_ZPX(Cpu& cpu, i32& cycles, Mem& mem) {
    // 1. Fetch the zero page address
    byte zp_addr = cpu.fetch_byte(cycles, mem);
    // 2. Add X register to it (with zero page wraparound)
    zp_addr += cpu.get(Register::X);
    zp_addr &= 0xFF;  // Ensure it wraps around to zero page
    // 3. Store the value of the Y register at the calculated address
    mem[zp_addr] = cpu.get(Register::Y);
    // 4. Decrement cycles as needed
    cycles -= 3;  // Two cycles for the zero page write, one for the addition
}

// STY Absolute mode
void STY_ABS(Cpu& cpu, i32& cycles, Mem& mem) {
    // 1. Fetch the absolute address (16-bit)
    word abs_addr = cpu.fetch_word(cycles, mem);
    // 2. Store the value of the Y register at that address
    mem[abs_addr] = cpu.get(Register::Y);
    // 3. Decrement cycles as needed
    cycles -= 2;  // Two cycles for the absolute write operation
}

}  // namespace instructions
