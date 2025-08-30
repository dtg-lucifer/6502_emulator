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
        // STA opcodes
        case 0x85:
            return "STA_ZP";
        case 0x95:
            return "STA_ZPX";
        case 0x8D:
            return "STA_ABS";
        case 0x9D:
            return "STA_ABSX";
        case 0x99:
            return "STA_ABSY";
        case 0x81:
            return "STA_INX";
        case 0x91:
            return "STA_INY";
        // STX opcodes
        case 0x86:
            return "STX_ZP";
        case 0x96:
            return "STX_ZPY";
        case 0x8E:
            return "STX_ABS";
        // STY opcodes
        case 0x84:
            return "STY_ZP";
        case 0x94:
            return "STY_ZPX";
        case 0x8C:
            return "STY_ABS";
        // Jump opcodes
        case 0x4C:
            return "JMP";
        case 0x6C:
            return "JMPI";
        // Stack operations
        case 0x48:
            return "PHA";
        case 0x08:
            return "PHP";
        case 0x68:
            return "PLA";
        case 0x28:
            return "PLP";
        // Register transfer operations
        case 0xBA:
            return "TSX";
        case 0x9A:
            return "TXS";
        case 0xAA:
            return "TAX";
        case 0xA8:
            return "TAY";
        case 0x8A:
            return "TXA";
        case 0x98:
            return "TYA";
        // Logical operations - AND
        case 0x29:
            return "AND_IM";
        case 0x25:
            return "AND_ZP";
        case 0x35:
            return "AND_ZPX";
        case 0x2D:
            return "AND_AB";
        case 0x3D:
            return "AND_ABSX";
        case 0x39:
            return "AND_ABSY";
        case 0x21:
            return "AND_INX";
        case 0x31:
            return "AND_INY";
        // Logical operations - EOR (XOR)
        case 0x49:
            return "EOR_IM";
        case 0x45:
            return "EOR_ZP";
        case 0x55:
            return "EOR_ZPX";
        case 0x4D:
            return "EOR_AB";
        case 0x5D:
            return "EOR_ABSX";
        case 0x59:
            return "EOR_ABSY";
        case 0x41:
            return "EOR_INX";
        case 0x51:
            return "EOR_INY";
        // Logical operations - ORA (OR)
        case 0x09:
            return "ORA_IM";
        case 0x05:
            return "ORA_ZP";
        case 0x15:
            return "ORA_ZPX";
        case 0x0D:
            return "ORA_AB";
        case 0x1D:
            return "ORA_ABSX";
        case 0x19:
            return "ORA_ABSY";
        case 0x01:
            return "ORA_INX";
        case 0x11:
            return "ORA_INY";
        // Arithmetic operations - ADC
        case 0x69:
            return "ADC_IM";
        case 0x65:
            return "ADC_ZP";
        case 0x75:
            return "ADC_ZPX";
        case 0x6D:
            return "ADC_AB";
        case 0x7D:
            return "ADC_ABSX";
        case 0x79:
            return "ADC_ABSY";
        case 0x61:
            return "ADC_INX";
        case 0x71:
            return "ADC_INY";
        // Arithmetic operations - SBC
        case 0xE9:
            return "SBC_IM";
        case 0xE5:
            return "SBC_ZP";
        case 0xF5:
            return "SBC_ZPX";
        case 0xED:
            return "SBC_AB";
        case 0xFD:
            return "SBC_ABSX";
        case 0xF9:
            return "SBC_ABSY";
        case 0xE1:
            return "SBC_INX";
        case 0xF1:
            return "SBC_INY";
        // Increment operations
        case 0xE6:
            return "INC_ZP";
        case 0xF6:
            return "INC_ZPX";
        case 0xEE:
            return "INC_AB";
        case 0xFE:
            return "INC_ABSX";
        case 0xE8:
            return "INX";
        case 0xC8:
            return "INY";
        // Decrement operations
        case 0xC6:
            return "DEC_ZP";
        case 0xD6:
            return "DEC_ZPX";
        case 0xCE:
            return "DEC_AB";
        case 0xDE:
            return "DEC_ABSX";
        case 0xCA:
            return "DEX";
        case 0x88:
            return "DEY";
        // Compare operations - CMP
        case 0xC9:
            return "CMP_IM";
        case 0xC5:
            return "CMP_ZP";
        case 0xD5:
            return "CMP_ZPX";
        case 0xCD:
            return "CMP_AB";
        case 0xDD:
            return "CMP_ABSX";
        case 0xD9:
            return "CMP_ABSY";
        case 0xC1:
            return "CMP_INX";
        case 0xD1:
            return "CMP_INY";
        // Compare operations - CPX
        case 0xE0:
            return "CPX_IM";
        case 0xE4:
            return "CPX_ZP";
        case 0xEC:
            return "CPX_AB";
        // Compare operations - CPY
        case 0xC0:
            return "CPY_IM";
        case 0xC4:
            return "CPY_ZP";
        case 0xCC:
            return "CPY_AB";
        // Bit manipulation
        case 0x24:
            return "BIT_ZP";
        case 0x2C:
            return "BIT_AB";
        // Shifts and rotates - ASL
        case 0x0A:
            return "ASL_A";
        case 0x06:
            return "ASL_ZP";
        case 0x16:
            return "ASL_ZPX";
        case 0x0E:
            return "ASL_AB";
        case 0x1E:
            return "ASL_ABSX";
        // Shifts and rotates - LSR
        case 0x4A:
            return "LSR_A";
        case 0x46:
            return "LSR_ZP";
        case 0x56:
            return "LSR_ZPX";
        case 0x4E:
            return "LSR_AB";
        case 0x5E:
            return "LSR_ABSX";
        // Shifts and rotates - ROL
        case 0x2A:
            return "ROL_A";
        case 0x26:
            return "ROL_ZP";
        case 0x36:
            return "ROL_ZPX";
        case 0x2E:
            return "ROL_AB";
        case 0x3E:
            return "ROL_ABSX";
        // Shifts and rotates - ROR
        case 0x6A:
            return "ROR_A";
        case 0x66:
            return "ROR_ZP";
        case 0x76:
            return "ROR_ZPX";
        case 0x6E:
            return "ROR_AB";
        case 0x7E:
            return "ROR_ABSX";
        // Status flag operations
        case 0x18:
            return "CLC";
        case 0xD8:
            return "CLD";
        case 0x58:
            return "CLI";
        case 0xB8:
            return "CLV";
        case 0x38:
            return "SEC";
        case 0xF8:
            return "SED";
        case 0x78:
            return "SEI";
        // Branch operations
        case 0x90:
            return "BCC";
        case 0xB0:
            return "BCS";
        case 0xF0:
            return "BEQ";
        case 0x30:
            return "BMI";
        case 0xD0:
            return "BNE";
        case 0x10:
            return "BPL";
        case 0x50:
            return "BVC";
        case 0x70:
            return "BVS";
        default:
            return "Unknown opcode: 0x" + std::to_string(opcode);
    }
}
}  // namespace opcodes
