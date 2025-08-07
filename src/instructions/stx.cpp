#include "instructions.h"
#include "op_codes.h"

namespace instructions {

// STX Zero Page mode
void STX_ZP(Cpu& cpu, i32& cycles, Mem& mem) {
    // 1. Fetch the zero page address
    word zp_addr = cpu.fetch_byte(cycles, mem);
    // 2. Store the value of the X register at that address
    mem[zp_addr] = cpu.get(Register::X);
    // 3. Decrement cycles as needed
    cycles--;  // Decrement cycles for the zero page write
    cycles--;  // Additional cycle for the write operation
}

// STX Zero Page,Y mode
void STX_ZPY(Cpu& cpu, i32& cycles, Mem& mem) {
    // 1. Fetch the zero page address
    word zp_addr = cpu.fetch_byte(cycles, mem);
    // 2. Add Y register to it (with zero page wraparound)
    zp_addr += cpu.get(Register::Y);
    zp_addr &= 0xFF;  // Ensure it wraps around to zero page
    // 3. Store the value of the X register at the calculated address
    mem[zp_addr] = cpu.get(Register::X);
    // 4. Decrement cycles as needed
    cycles--;
    cycles--;
    cycles--;
}

// STX Absolute mode
void STX_ABS(Cpu& cpu, i32& cycles, Mem& mem) {
    // 1. Fetch the absolute address (16-bit)
    word abs_addr = cpu.fetch_word(cycles, mem);
    // 2. Store the value of the X register at that address
    mem[abs_addr] = cpu.get(Register::X);
    // 3. Decrement cycles as needed
    cycles -= 2;  // Two cycles for the absolute write operation
}

}  // namespace instructions
