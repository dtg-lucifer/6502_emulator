#include "instructions.h"

namespace instructions {

// RTS (Return from Subroutine)
void RTS(Cpu& cpu, i32& cycles, Mem& mem) {
    // Pull return address from stack - low byte first, then high byte
    cpu.SP++;
    cycles--;
    byte lo = mem[0x0100 + cpu.SP];  // Read low byte from stack

    cpu.SP++;
    cycles--;
    byte hi = mem[0x0100 + cpu.SP];  // Read high byte from stack

    // Reconstruct the 16-bit address
    word return_addr = (hi << 8) | lo;

    // Set PC to return address + 1 (since JSR stored PC-1)
    cpu.PC = return_addr + 1;
    cycles -= 2;  // Extra cycles for updating PC
}

// NOP (No Operation)
void NOP(Cpu& cpu, i32& cycles, Mem& mem) {
    // No operation, just consume a cycle
    cycles--;
}

}  // namespace instructions
