#include "instructions.h"

namespace instructions {
void TXS(Cpu& cpu, i32& cycles, Mem& mem) {
    cpu.SP = cpu.get(Register::X);  // Transfer X register to Stack Pointer
    cycles -= 2;                    // TXS takes 2 cycles
}
}  // namespace instructions
