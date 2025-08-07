#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "cpu.h"
#include "memory.h"
#include "types.h"

namespace instructions {

// LDA Instructions
void LDA_IM(Cpu& cpu, i32& cycles, Mem& mem);
void LDA_ZP(Cpu& cpu, i32& cycles, Mem& mem);
void LDA_ZPX(Cpu& cpu, i32& cycles, Mem& mem);
void LDA_AB(Cpu& cpu, i32& cycles, Mem& mem);
void LDA_ABSX(Cpu& cpu, i32& cycles, Mem& mem);
void LDA_ABSY(Cpu& cpu, i32& cycles, Mem& mem);
void LDA_INX(Cpu& cpu, i32& cycles, Mem& mem);
void LDA_INY(Cpu& cpu, i32& cycles, Mem& mem);

// LDX Instructions
void LDX_IM(Cpu& cpu, i32& cycles, Mem& mem);
void LDX_ZP(Cpu& cpu, i32& cycles, Mem& mem);
void LDX_ZPY(Cpu& cpu, i32& cycles, Mem& mem);
void LDX_AB(Cpu& cpu, i32& cycles, Mem& mem);
void LDX_ABSY(Cpu& cpu, i32& cycles, Mem& mem);

// LDY Instructions
void LDY_IM(Cpu& cpu, i32& cycles, Mem& mem);
void LDY_ZP(Cpu& cpu, i32& cycles, Mem& mem);
void LDY_ZPX(Cpu& cpu, i32& cycles, Mem& mem);
void LDY_AB(Cpu& cpu, i32& cycles, Mem& mem);
void LDY_ABSX(Cpu& cpu, i32& cycles, Mem& mem);

// JSR Instruction
void JSR(Cpu& cpu, i32& cycles, Mem& mem);

// RTS Instruction
void RTS(Cpu& cpu, i32& cycles, Mem& mem);

// NOP Instruction
void NOP(Cpu& cpu, i32& cycles, Mem& mem);

}  // namespace instructions

#endif  // INSTRUCTIONS_H
