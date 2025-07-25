#include "instructions.h"
#include "op_codes.h"

namespace instructions {

// Helper function to set flags for LDA instructions
static void LDA_SetFlags(Cpu& cpu) {
    cpu.Z = (cpu.A == 0);              // Set the Zero flag
    cpu.N = (cpu.A & 0b10000000) > 0;  // Set the Negative flag
}

// LDA Immediate mode
void LDA_IM(Cpu& cpu, i32& cycles, Mem& mem) {
    byte value = cpu.fetch_byte(cycles, mem);
    cpu.set(Register::A, value);
    LDA_SetFlags(cpu);
}

// LDA Zero Page mode
void LDA_ZP(Cpu& cpu, i32& cycles, Mem& mem) {
    byte addr = cpu.fetch_byte(cycles, mem);
    cpu.set(Register::A, cpu.read_byte(addr, cycles, mem));
    LDA_SetFlags(cpu);
}

// LDA Zero Page,X mode
void LDA_ZPX(Cpu& cpu, i32& cycles, Mem& mem) {
    byte addr = cpu.fetch_byte(cycles, mem);
    addr += cpu.get(Register::X);
    cycles--;  // Additional cycle for adding X
    cpu.set(Register::A, cpu.read_byte(addr, cycles, mem));
    LDA_SetFlags(cpu);
}

// LDA Absolute mode
void LDA_AB(Cpu& cpu, i32& cycles, Mem& mem) {
    word addr = cpu.fetch_word(cycles, mem);
    cpu.set(Register::A, mem[addr]);
    cycles--;  // Additional cycle for reading from absolute address
    LDA_SetFlags(cpu);
}

// LDA Absolute,X mode
void LDA_ABSX(Cpu& cpu, i32& cycles, Mem& mem) {
    word addr = cpu.fetch_word(cycles, mem);
    addr += cpu.get(Register::X);
    cycles--;           // Additional cycle for adding X
    cpu.A = mem[addr];  // Directly access A register
    LDA_SetFlags(cpu);
}

// LDA Absolute,Y mode
void LDA_ABSY(Cpu& cpu, i32& cycles, Mem& mem) {
    word addr = cpu.fetch_word(cycles, mem);
    addr += cpu.get(Register::Y);
    cycles--;  // Additional cycle for adding Y
    cpu.set(Register::A, mem[addr]);
    LDA_SetFlags(cpu);
}

// LDA (Indirect,X) mode
void LDA_INX(Cpu& cpu, i32& cycles, Mem& mem) {
    byte addr = cpu.fetch_byte(cycles, mem);
    addr += cpu.get(Register::X);
    cycles--;  // Additional cycle for adding X

    // Read the effective address
    byte low = mem[addr];
    byte high = mem[addr + 1];
    cycles -= 2;  // Two cycles for reading the address

    word effective_addr = (high << 8) | low;
    cpu.set(Register::A, mem[effective_addr]);
    LDA_SetFlags(cpu);
}

// LDA (Indirect),Y mode
void LDA_INY(Cpu& cpu, i32& cycles, Mem& mem) {
    byte addr = cpu.fetch_byte(cycles, mem);
    cycles--;  // Cycle for fetching zero page address

    // Read the 16-bit address from zero page with wraparound
    byte low = mem[addr];
    byte high = mem[(addr + 1) & 0xFF];  // Zero-page wraparound
    cycles -= 2;                         // Two cycles for reading the address

    word effective_addr = (high << 8) | low;
    effective_addr += cpu.get(Register::Y);

    cpu.set(Register::A, mem[effective_addr]);
    LDA_SetFlags(cpu);
}

}  // namespace instructions
