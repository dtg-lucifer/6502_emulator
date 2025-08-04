#include <iostream>
#include <string>

#include "op_codes.h"

namespace opcodes {
std::string from_byte(byte opcode) {
    switch (opcode) {
        case 0x00:
            return "BRK (Force Interrupt)";
        case 0xEA:
            return "NOP (No Operation)";
        case 0xA9:
            return "LDA_IM (Load Accumulator Immediate)";
        case 0xAD:
            return "LDA_AB (Load Accumulator Absolute)";
        case 0xA5:
            return "LDA_ZP (Load Accumulator Zero Page)";
        case 0xB5:
            return "LDA_ZPX (Load Accumulator Zero Page,X)";
        case 0xBD:
            return "LDA_ABSX (Load Accumulator Absolute,X)";
        case 0xB9:
            return "LDA_ABSY (Load Accumulator Absolute,Y)";
        case 0xA1:
            return "LDA_INX (Load Accumulator (Indirect,X))";
        case 0xB1:
            return "LDA_INY (Load Accumulator (Indirect),Y)";
        case 0x20:
            return "JSR (Jump to Subroutine)";
        case 0x60:
            return "RTS (Return from Subroutine)";
        default:
            return "Unknown opcode: 0x" + std::to_string(opcode);
    }
}
}  // namespace opcodes
