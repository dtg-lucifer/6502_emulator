#include "instructions.h"

namespace instructions {
void PLA(Cpu& cpu, i32& cycles, Mem& mem) {
    cpu.SP++;                                 // Increment stack pointer to point to the next value
    byte value = mem[cpu.SP + 0x0100];        // Pull value from stack
    cpu.set(Register::A, value);              // Set the accumulator with the pulled value
    cpu.FLAGS_Z = (value == 0);               // Set Zero flag if value is zero
    cpu.FLAGS_N = (value & 0b10000000) != 0;  // Set Negative flag if bit 7 is set
    cycles -= 4;                              // PLA takes 4 cycles
}
}  // namespace instructions
