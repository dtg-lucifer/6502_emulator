#include "instructions.h"
#include "op_codes.h"

namespace instructions {
void TSX(Cpu& cpu, i32& cycles, Mem& mem) {
    cpu.set(Register::X, cpu.SP);
    cpu.FLAGS_Z = (cpu.get(Register::X) == 0);
    cpu.FLAGS_N = (cpu.get(Register::X) & 0b10000000) != 0;  // Set Negative flag if bit 7 is set
    cycles -= 2;                                             // TSX takes 2 cycles
}
}  // namespace instructions
