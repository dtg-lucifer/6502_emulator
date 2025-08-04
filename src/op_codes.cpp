#include "op_codes.h"

#include <string>

namespace opcodes {
std::string from_byte(byte opcode) {
    switch (opcode) {
        case 0x00:
            return "BRK";
        case 0xEA:
            return "NOP";
        case 0xA9:
            return "LDA_IM";
        case 0xAD:
            return "LDA_AB";
        case 0xA5:
            return "LDA_ZP";
        case 0xB5:
            return "LDA_ZPX";
        case 0xBD:
            return "LDA_ABSX";
        case 0xB9:
            return "LDA_ABSY";
        case 0xA1:
            return "LDA_INX";
        case 0xB1:
            return "LDA_INY";
        case 0x20:
            return "JSR";
        case 0x60:
            return "RTS";
        default:
            return "Unknown opcode: 0x" + std::to_string(opcode);
    }
}
}  // namespace opcodes
