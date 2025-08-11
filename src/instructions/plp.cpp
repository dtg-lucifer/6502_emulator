#include "instructions.h"

namespace instructions {
void PLP(Cpu& cpu, i32& cycles, Mem& mem) {
    cpu.SP++;                            // Increment stack pointer
    byte status = mem[cpu.SP + 0x0100];  // Pull processor status from stack
    cpu.FLAGS = status;                  // Set processor status flags from stack
    cycles -= 4;                         // PLP takes 4 cycles
}
}  // namespace instructions
