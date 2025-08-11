#include "instructions.h"

namespace instructions {
void PHP(Cpu& cpu, i32& cycles, Mem& mem) {
    mem[cpu.SP + 0x0100] = cpu.FLAGS;  // Push processor status onto stack
    cpu.SP--;                          // Decrement stack pointer
    cycles -= 3;                       // PHP takes 3 cycles
}
}  // namespace instructions
