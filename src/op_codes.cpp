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
        // LDX opcodes
        case 0xA2:
            return "LDX_IM";
        case 0xA6:
            return "LDX_ZP";
        case 0xB6:
            return "LDX_ZPY";
        case 0xAE:
            return "LDX_AB";
        case 0xBE:
            return "LDX_ABSY";
        // LDY opcodes
        case 0xA0:
            return "LDY_IM";
        case 0xA4:
            return "LDY_ZP";
        case 0xB4:
            return "LDY_ZPX";
        case 0xAC:
            return "LDY_AB";
        case 0xBC:
            return "LDY_ABSX";
        default:
            return "Unknown opcode: 0x" + std::to_string(opcode);
    }
}
}  // namespace opcodes
