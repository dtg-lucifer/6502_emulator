#ifndef OP_CODES_H
#define OP_CODES_H

#include "types.h"

enum class Register : byte { A = 0, X = 1, Y = 2 };

enum class Op : byte {
    BRK = 0x00,       // Force Interrupt (Break)
    NOP = 0xEA,       // No Operation
    LDA_IM = 0xA9,    // Load Accumulator Immediate
    LDA_AB = 0xAD,    // Load Accumulator Absolute
    LDA_ZP = 0xA5,    // Load Accumulator Zero Page
    LDA_ZPX = 0xB5,   // Load Accumulator Zero Page,X
    LDA_ABSX = 0xBD,  // Load Accumulator Absolute,X
    LDA_ABSY = 0xB9,  // Load Accumulator Absolute,Y
    LDA_INX = 0xA1,   // Load Accumulator (Indirect,X)
    LDA_INY = 0xB1,   // Load Accumulator (Indirect),Y
    JSR = 0x20,       // Jump to Subroutine
    RTS = 0x60,       // Return from Subroutine
};

// Helper function to convert from Op enum to byte
constexpr byte op(const Op o) {
    return static_cast<byte>(o);
}

#endif  // OP_CODES_H
