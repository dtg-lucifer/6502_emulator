#include "instructions.h"

namespace instructions {
void PHA(Cpu& cpu, i32& cycles, Mem& mem) {
    mem[cpu.SP + 0x0100] = cpu.get(Register::A);  // Push Accumulator onto stack
    cpu.SP--;                                     // Decrement stack pointer
    cycles -= 3;                                  // PHA takes 3 cycles
}
}  // namespace instructions
