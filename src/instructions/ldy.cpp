#include "instructions.h"
#include "op_codes.h"

namespace instructions {

// Helper function to set flags for LDY instructions
static void LDY_SetFlags(Cpu& cpu) {
    cpu.Z = (cpu.Y == 0);              // Set the Zero flag
    cpu.N = (cpu.Y & 0b10000000) > 0;  // Set the Negative flag
}

// LDY Immediate mode
void LDY_IM(Cpu& cpu, i32& cycles, Mem& mem) {
    byte value = cpu.fetch_byte(cycles, mem);
    cpu.set(Register::Y, value);
    LDY_SetFlags(cpu);
}

// LDY Zero Page mode
void LDY_ZP(Cpu& cpu, i32& cycles, Mem& mem) {
    byte addr = cpu.fetch_byte(cycles, mem);
    byte value = cpu.read_byte(addr, cycles, mem);
    cpu.set(Register::Y, value);
    LDY_SetFlags(cpu);
}

// LDY Zero Page,X mode
void LDY_ZPX(Cpu& cpu, i32& cycles, Mem& mem) {
    byte addr = cpu.fetch_byte(cycles, mem);
    byte x_val = cpu.get(Register::X);
    addr += x_val;
    cycles--;  // Additional cycle for adding X
    byte value = cpu.read_byte(addr, cycles, mem);
    cpu.set(Register::Y, value);
    LDY_SetFlags(cpu);
}

// LDY Absolute mode
void LDY_AB(Cpu& cpu, i32& cycles, Mem& mem) {
    word addr = cpu.fetch_word(cycles, mem);
    byte value = mem[addr];
    cpu.set(Register::Y, value);
    cycles--;  // Additional cycle for reading from absolute address
    LDY_SetFlags(cpu);
}

// LDY Absolute,X mode
void LDY_ABSX(Cpu& cpu, i32& cycles, Mem& mem) {
    word addr = cpu.fetch_word(cycles, mem);
    byte x_val = cpu.get(Register::X);
    addr += x_val;
    cycles--;  // Additional cycle for adding X
    byte value = mem[addr];
    cpu.set(Register::Y, value);
    LDY_SetFlags(cpu);
}

}  // namespace instructions
