#include "instructions.h"
#include "op_codes.h"

namespace instructions {

// Helper function to set flags for LDX instructions
static void LDX_SetFlags(Cpu& cpu) {
    cpu.Z = (cpu.X == 0);              // Set the Zero flag
    cpu.N = (cpu.X & 0b10000000) > 0;  // Set the Negative flag
}

// LDX Immediate mode
void LDX_IM(Cpu& cpu, i32& cycles, Mem& mem) {
    byte value = cpu.fetch_byte(cycles, mem);
    cpu.set(Register::X, value);
    LDX_SetFlags(cpu);
}

// LDX Zero Page mode
void LDX_ZP(Cpu& cpu, i32& cycles, Mem& mem) {
    byte addr = cpu.fetch_byte(cycles, mem);
    cpu.set(Register::X, cpu.read_byte(addr, cycles, mem));
    LDX_SetFlags(cpu);
}

// LDX Zero Page,Y mode
void LDX_ZPY(Cpu& cpu, i32& cycles, Mem& mem) {
    byte addr = cpu.fetch_byte(cycles, mem);
    addr += cpu.get(Register::Y);
    cycles--;  // Additional cycle for adding Y
    cpu.set(Register::X, cpu.read_byte(addr, cycles, mem));
    LDX_SetFlags(cpu);
}

// LDX Absolute mode
void LDX_AB(Cpu& cpu, i32& cycles, Mem& mem) {
    word addr = cpu.fetch_word(cycles, mem);
    cpu.set(Register::X, mem[addr]);
    cycles--;  // Additional cycle for reading from absolute address
    LDX_SetFlags(cpu);
}

// LDX Absolute,Y mode
void LDX_ABSY(Cpu& cpu, i32& cycles, Mem& mem) {
    word addr = cpu.fetch_word(cycles, mem);
    addr += cpu.get(Register::Y);
    cycles--;  // Additional cycle for adding Y
    cpu.set(Register::X, mem[addr]);
    LDX_SetFlags(cpu);
}

}  // namespace instructions
