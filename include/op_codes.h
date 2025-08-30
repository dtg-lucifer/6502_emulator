#ifndef OP_CODES_H
#define OP_CODES_H

#include <string>

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
    // -----------------------------------------------
    LDX_IM = 0xA2,    // Load X Register Immediate
    LDX_ZP = 0xA6,    // Load X Register Zero Page
    LDX_ZPY = 0xB6,   // Load X Register Zero Page,Y
    LDX_AB = 0xAE,    // Load X Register Absolute
    LDX_ABSY = 0xBE,  // Load X Register Absolute,Y
    // -----------------------------------------------
    LDY_IM = 0xA0,    // Load Y Register Immediate
    LDY_ZP = 0xA4,    // Load Y Register Zero Page
    LDY_ZPX = 0xB4,   // Load Y Register Zero Page,X
    LDY_AB = 0xAC,    // Load Y Register Absolute
    LDY_ABSX = 0xBC,  // Load Y Register Absolute,X
    // -----------------------------------------------
    STA_ZP = 0x85,    // Store Accumulator Zero Page
    STA_ZPX = 0x95,   // Store Accumulator Zero Page,X
    STA_ABS = 0x8D,   // Store Accumulator Absolute
    STA_ABSX = 0x9D,  // Store Accumulator Absolute,X
    STA_ABSY = 0x99,  // Store Accumulator Absolute,Y
    STA_INX = 0x81,   // Store Accumulator (Indirect,X)
    STA_INY = 0x91,   // Store Accumulator (Indirect),Y
    // -----------------------------------------------
    STX_ZP = 0x86,   // Store X Register Zero Page
    STX_ZPY = 0x96,  // Store X Register Zero Page,Y
    STX_ABS = 0x8E,  // Store X Register Absolute
    // -----------------------------------------------
    STY_ZP = 0x84,   // Store Y Register Zero Page
    STY_ZPX = 0x94,  // Store Y Register Zero Page,X
    STY_ABS = 0x8C,  // Store Y Register Absolute
    // -----------------------------------------------
    JSR = 0x20,   // Jump to Subroutine
    RTS = 0x60,   // Return from Subroutine
    JMP = 0x4C,   // Jump Absolute
    JMPI = 0x6C,  // Jump Indirect
    // -----------------------------------------------
    // Stack Operations
    PHA = 0x48,  // Push Accumulator on Stack
    PHP = 0x08,  // Push Processor Status on Stack
    PLA = 0x68,  // Pull Accumulator from Stack
    PLP = 0x28,  // Pull Processor Status from Stack
    // -----------------------------------------------
    // Register Transfer Operations
    TSX = 0xBA,  // Transfer Stack Pointer to X
    TXS = 0x9A,  // Transfer X to Stack Pointer
    TAX = 0xAA,  // Transfer Accumulator to X
    TAY = 0xA8,  // Transfer Accumulator to Y
    TXA = 0x8A,  // Transfer X to Accumulator
    TYA = 0x98,  // Transfer Y to Accumulator
    // -----------------------------------------------
    // Logical Operations
    AND_IM = 0x29,    // AND Accumulator with Immediate
    AND_ZP = 0x25,    // AND Accumulator with Zero Page
    AND_ZPX = 0x35,   // AND Accumulator with Zero Page,X
    AND_AB = 0x2D,    // AND Accumulator with Absolute
    AND_ABSX = 0x3D,  // AND Accumulator with Absolute,X
    AND_ABSY = 0x39,  // AND Accumulator with Absolute,Y
    AND_INX = 0x21,   // AND Accumulator with (Indirect,X)
    AND_INY = 0x31,   // AND Accumulator with (Indirect),Y
    // -----------------------------------------------
    EOR_IM = 0x49,    // XOR Accumulator with Immediate
    EOR_ZP = 0x45,    // XOR Accumulator with Zero Page
    EOR_ZPX = 0x55,   // XOR Accumulator with Zero Page,X
    EOR_AB = 0x4D,    // XOR Accumulator with Absolute
    EOR_ABSX = 0x5D,  // XOR Accumulator with Absolute,X
    EOR_ABSY = 0x59,  // XOR Accumulator with Absolute,Y
    EOR_INX = 0x41,   // XOR Accumulator with (Indirect,X)
    EOR_INY = 0x51,   // XOR Accumulator with (Indirect),Y
    // -----------------------------------------------
    ORA_IM = 0x09,    // OR Accumulator with Immediate
    ORA_ZP = 0x05,    // OR Accumulator with Zero Page
    ORA_ZPX = 0x15,   // OR Accumulator with Zero Page,X
    ORA_AB = 0x0D,    // OR Accumulator with Absolute
    ORA_ABSX = 0x1D,  // OR Accumulator with Absolute,X
    ORA_ABSY = 0x19,  // OR Accumulator with Absolute,Y
    ORA_INX = 0x01,   // OR Accumulator with (Indirect,X)
    ORA_INY = 0x11,   // OR Accumulator with (Indirect),Y
    // -----------------------------------------------
    // Arithmetic Operations
    ADC_IM = 0x69,    // Add with Carry Immediate
    ADC_ZP = 0x65,    // Add with Carry Zero Page
    ADC_ZPX = 0x75,   // Add with Carry Zero Page,X
    ADC_AB = 0x6D,    // Add with Carry Absolute
    ADC_ABSX = 0x7D,  // Add with Carry Absolute,X
    ADC_ABSY = 0x79,  // Add with Carry Absolute,Y
    ADC_INX = 0x61,   // Add with Carry (Indirect,X)
    ADC_INY = 0x71,   // Add with Carry (Indirect),Y
    // -----------------------------------------------
    SBC_IM = 0xE9,    // Subtract with Carry Immediate
    SBC_ZP = 0xE5,    // Subtract with Carry Zero Page
    SBC_ZPX = 0xF5,   // Subtract with Carry Zero Page,X
    SBC_AB = 0xED,    // Subtract with Carry Absolute
    SBC_ABSX = 0xFD,  // Subtract with Carry Absolute,X
    SBC_ABSY = 0xF9,  // Subtract with Carry Absolute,Y
    SBC_INX = 0xE1,   // Subtract with Carry (Indirect,X)
    SBC_INY = 0xF1,   // Subtract with Carry (Indirect),Y
    // -----------------------------------------------
    // Increment and Decrement Operations
    INC_ZP = 0xE6,    // Increment Memory Zero Page
    INC_ZPX = 0xF6,   // Increment Memory Zero Page,X
    INC_AB = 0xEE,    // Increment Memory Absolute
    INC_ABSX = 0xFE,  // Increment Memory Absolute,X
    INX = 0xE8,       // Increment X Register
    INY = 0xC8,       // Increment Y Register
    // -----------------------------------------------
    DEC_ZP = 0xC6,    // Decrement Memory Zero Page
    DEC_ZPX = 0xD6,   // Decrement Memory Zero Page,X
    DEC_AB = 0xCE,    // Decrement Memory Absolute
    DEC_ABSX = 0xDE,  // Decrement Memory Absolute,X
    DEX = 0xCA,       // Decrement X Register
    DEY = 0x88,       // Decrement Y Register
    // -----------------------------------------------
    // Compare Operations
    CMP_IM = 0xC9,    // Compare Accumulator Immediate
    CMP_ZP = 0xC5,    // Compare Accumulator Zero Page
    CMP_ZPX = 0xD5,   // Compare Accumulator Zero Page,X
    CMP_AB = 0xCD,    // Compare Accumulator Absolute
    CMP_ABSX = 0xDD,  // Compare Accumulator Absolute,X
    CMP_ABSY = 0xD9,  // Compare Accumulator Absolute,Y
    CMP_INX = 0xC1,   // Compare Accumulator (Indirect,X)
    CMP_INY = 0xD1,   // Compare Accumulator (Indirect),Y
    // -----------------------------------------------
    CPX_IM = 0xE0,  // Compare X Register Immediate
    CPX_ZP = 0xE4,  // Compare X Register Zero Page
    CPX_AB = 0xEC,  // Compare X Register Absolute
    // -----------------------------------------------
    CPY_IM = 0xC0,  // Compare Y Register Immediate
    CPY_ZP = 0xC4,  // Compare Y Register Zero Page
    CPY_AB = 0xCC,  // Compare Y Register Absolute
    // -----------------------------------------------
    // Bit Manipulation
    BIT_ZP = 0x24,  // Test Bits in Memory with Accumulator Zero Page
    BIT_AB = 0x2C,  // Test Bits in Memory with Accumulator Absolute
    // -----------------------------------------------
    // Shifts and Rotates
    ASL_A = 0x0A,     // Arithmetic Shift Left Accumulator
    ASL_ZP = 0x06,    // Arithmetic Shift Left Zero Page
    ASL_ZPX = 0x16,   // Arithmetic Shift Left Zero Page,X
    ASL_AB = 0x0E,    // Arithmetic Shift Left Absolute
    ASL_ABSX = 0x1E,  // Arithmetic Shift Left Absolute,X
    // -----------------------------------------------
    LSR_A = 0x4A,     // Logical Shift Right Accumulator
    LSR_ZP = 0x46,    // Logical Shift Right Zero Page
    LSR_ZPX = 0x56,   // Logical Shift Right Zero Page,X
    LSR_AB = 0x4E,    // Logical Shift Right Absolute
    LSR_ABSX = 0x5E,  // Logical Shift Right Absolute,X
    // -----------------------------------------------
    ROL_A = 0x2A,     // Rotate Left Accumulator
    ROL_ZP = 0x26,    // Rotate Left Zero Page
    ROL_ZPX = 0x36,   // Rotate Left Zero Page,X
    ROL_AB = 0x2E,    // Rotate Left Absolute
    ROL_ABSX = 0x3E,  // Rotate Left Absolute,X
    // -----------------------------------------------
    ROR_A = 0x6A,     // Rotate Right Accumulator
    ROR_ZP = 0x66,    // Rotate Right Zero Page
    ROR_ZPX = 0x76,   // Rotate Right Zero Page,X
    ROR_AB = 0x6E,    // Rotate Right Absolute
    ROR_ABSX = 0x7E,  // Rotate Right Absolute,X
    // -----------------------------------------------
    // Status Flag Operations
    CLC = 0x18,  // Clear Carry Flag
    CLD = 0xD8,  // Clear Decimal Mode Flag
    CLI = 0x58,  // Clear Interrupt Disable Flag
    CLV = 0xB8,  // Clear Overflow Flag
    SEC = 0x38,  // Set Carry Flag
    SED = 0xF8,  // Set Decimal Mode Flag
    SEI = 0x78,  // Set Interrupt Disable Flag
    // -----------------------------------------------
    // Branch Operations
    BCC = 0x90,  // Branch if Carry Clear
    BCS = 0xB0,  // Branch if Carry Set
    BEQ = 0xF0,  // Branch if Equal (Zero Set)
    BMI = 0x30,  // Branch if Minus (Negative Set)
    BNE = 0xD0,  // Branch if Not Equal (Zero Clear)
    BPL = 0x10,  // Branch if Plus (Negative Clear)
    BVC = 0x50,  // Branch if Overflow Clear
    BVS = 0x70,  // Branch if Overflow Set
    // -----------------------------------------------
};

// Helper function to convert from Op enum to byte
constexpr byte op(const Op o) {
    return static_cast<byte>(o);
}

namespace opcodes {
std::string from_byte(byte opcode);
}  // namespace opcodes

#endif  // OP_CODES_H
