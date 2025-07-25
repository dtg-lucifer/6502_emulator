#include "instructions.h"
#include "op_codes.h"

namespace instructions {

// JSR (Jump to Subroutine)
void JSR(Cpu& cpu, i32& cycles, Mem& mem) {
    // Get the absolute address for the subroutine
    word addr = cpu.fetch_word(cycles, mem);

    // Push return address (PC-1) to stack - high byte first, then low byte
    mem[0x0100 + cpu.SP] = (cpu.PC - 1) >> 8;  // Push high byte
    cpu.SP--;
    cycles--;

    mem[0x0100 + cpu.SP] = (cpu.PC - 1) & 0xFF;  // Push low byte
    cpu.SP--;
    cycles--;

    // Set program counter to the subroutine address
    cpu.PC = addr;
    cycles--;  // Extra cycle for updating PC
}

}  // namespace instructions
