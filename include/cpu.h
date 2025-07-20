#ifndef CPU_H
#define CPU_H

#include "memory.h"
#include "op_codes.h"
#include "types.h"

class Cpu {
   private:
    void LDA_SetFlags();
    byte registers[3];  // Index registers

   public:
    word PC;                   // Program counter register
    byte SP;                   // Stack pointer register (8-bit)
    word stack_base = 0x0100;  // Base address for the stack (page 1)

    // Register references for easier access
    byte& A = registers[static_cast<byte>(Register::A)];
    byte& X = registers[static_cast<byte>(Register::X)];
    byte& Y = registers[static_cast<byte>(Register::Y)];

    // Status flags
    byte C : 1;  // Carry Flag
    byte Z : 1;  // Zero Flag
    byte I : 1;  // Interrupt Disable Flag
    byte D : 1;  // Decimal Mode Flag
    byte B : 1;  // Break Flag
    byte V : 1;  // Overflow Flag
    byte N : 1;  // Negative Flag
    byte U : 1;  // The 8th bit (unused/expansion)

    // Register access methods
    byte& get(const Register r);
    void set(Register r, byte val);

    // CPU operations
    void reset(Mem& mem);
    byte fetch_byte(u32& cycles, Mem& mem);
    word fetch_word(u32& cycles, Mem& mem);
    byte read_byte(byte zp_addr, u32& cycles, Mem& mem);
    void execute(u32 cycles, Mem& mem);
};

#endif  // CPU_H
