#include <map>
#include <vector>

#include "op_codes.h"
#include "types.h"

namespace demo_programs {

// This function returns a demo program that showcases LDA instructions
// with various addressing modes
std::map<u32, std::vector<byte>> get_lda_demo() {
    std::map<u32, std::vector<byte>> demo = {
        // Reset vector points to 0x2000 (low byte first, then high byte)
        {0xFFFC, {0x00, 0x20}},

        // Main routine at 0x2000
        // Test LDA instructions with various addressing modes
        {0x2000,
         {
             // LDA Immediate
             op(Op::LDA_IM),
             0x42,

             // LDA Zero Page
             op(Op::LDA_ZP),
             0x80,

             // LDA Zero Page,X
             op(Op::LDA_ZPX),
             0x70,

             // LDA Absolute
             op(Op::LDA_AB),
             0x00,
             0x30,

             // LDA Absolute,X
             op(Op::LDA_ABSX),
             0x10,
             0x30,

             // LDA Absolute,Y
             op(Op::LDA_ABSY),
             0x20,
             0x30,

             // Test indirect addressing modes
             // LDA (Indirect,X)
             op(Op::LDA_INX),
             0x90,

             // LDA (Indirect),Y
             op(Op::LDA_INY),
             0x92,
         }},

        // Subroutine at 0x2050
        {0x2050,
         {
             // Do something in the subroutine
             op(Op::LDA_IM),
             0xFF,
         }},

        // Data for various addressing modes
        {0x0080, {0x55}},  // Zero page data for LDA_ZP
        {0x0070, {0x00}},  // Base for Zero page,X
        {0x0075, {0x66}},  // Zero page,X data (0x70+X where X=5)
        {0x3000, {0x77}},  // Absolute data for LDA_AB
        {0x3015, {0x88}},  // Absolute,X data (0x3010+X where X=5)
        {0x3022, {0x99}},  // Absolute,Y data (0x3020+Y where Y=2)

        // Indirect addressing data
        {0x0090, {0x00}},        // Zero page for indirect X
        {0x0095, {0x00, 0x40}},  // Indirect address pointer after X offset (0x90+X where X=5)
        {0x4000, {0xAA}},        // Target for Indirect,X

        {0x0092, {0x30, 0x40}},  // Zero page for indirect Y
        {0x4032, {0xBB}},        // Target for Indirect,Y (0x4030+Y where Y=2)
    };

    return demo;
}

// This function returns a demo program that showcases LDX instructions
// with various addressing modes
std::map<u32, std::vector<byte>> get_ldx_demo() {
    std::map<u32, std::vector<byte>> demo = {
        // Reset vector points to 0x2000 (low byte first, then high byte)
        {0xFFFC, {0x00, 0x20}},

        // Main routine at 0x2000
        {0x2000,
         {// LDX Immediate
          op(Op::LDX_IM), 0xAA,

          // LDX Zero Page
          op(Op::LDX_ZP), 0xA0,

          // LDX Zero Page,Y
          op(Op::LDX_ZPY), 0xA5,

          // LDX Absolute
          op(Op::LDX_AB), 0x50, 0x30,

          // LDX Absolute,Y
          op(Op::LDX_ABSY), 0x60, 0x30,

          // Use BRK to exit the program instead of RTS
          0x00}},  // BRK instruction (0x00)

        // Data for various addressing modes
        {0x00A0, {0xCC}},  // Zero page data for LDX_ZP
        {0x00A7, {0xDD}},  // Zero page,Y data (0xA5+Y where Y=2)
        {0x3050, {0xEE}},  // Absolute data for LDX_AB
        {0x3062, {0xFF}},  // Absolute,Y data (0x3060+Y where Y=2)
    };

    return demo;
}

// This function returns a demo program that showcases LDY instructions
// with various addressing modes
std::map<u32, std::vector<byte>> get_ldy_demo() {
    std::map<u32, std::vector<byte>> demo = {
        // Reset vector points to 0x2000 (low byte first, then high byte)
        {0xFFFC, {0x00, 0x20}},

        // Main routine at 0x2000
        {0x2000,
         {// LDY Immediate
          op(Op::LDY_IM), 0xBB,

          // LDY Zero Page
          op(Op::LDY_ZP), 0xB0,

          // LDY Zero Page,X
          op(Op::LDY_ZPX), 0xB5,

          // LDY Absolute
          op(Op::LDY_AB), 0x70, 0x30,

          // LDY Absolute,X
          op(Op::LDY_ABSX), 0x80, 0x30,

          // Use BRK to exit the program instead of RTS
          0x00}},  // BRK instruction (0x00)

        // Data for various addressing modes
        {0x00B0, {0x11}},  // Zero page data for LDY_ZP
        {0x00BA, {0x22}},  // Zero page,X data (0xB5+X where X=5)
        {0x3070, {0x33}},  // Absolute data for LDY_AB
        {0x3085, {0x44}},  // Absolute,X data (0x3080+X where X=5)
    };

    return demo;
}

// Combined instruction demo that runs all types of instructions
std::map<u32, std::vector<byte>> get_instruction_demo() {
    std::map<u32, std::vector<byte>> demo = {
        // Reset vector points to 0x2000 (low byte first, then high byte)
        {0xFFFC, {0x00, 0x20}},

        // Main routine at 0x2000
        // Test LDA instructions with various addressing modes
        {0x2000,
         {// LDA Immediate
          op(Op::LDA_IM), 0x42,

          // LDA Zero Page
          op(Op::LDA_ZP), 0x80,

          // LDA Zero Page,X
          op(Op::LDA_ZPX), 0x70,

          // LDA Absolute
          op(Op::LDA_AB), 0x00, 0x30,

          // LDA Absolute,X
          op(Op::LDA_ABSX), 0x10, 0x30,

          // LDA Absolute,Y
          op(Op::LDA_ABSY), 0x20, 0x30,

          // Test indirect addressing modes
          // LDA (Indirect,X)
          op(Op::LDA_INX), 0x90,

          // LDA (Indirect),Y
          op(Op::LDA_INY), 0x92,

          // Test LDY instructions with various addressing modes first
          // (while X is still 0x05 from the initial setup)

          // LDY Immediate
          op(Op::LDY_IM), 0xBB,

          // LDY Zero Page
          op(Op::LDY_ZP), 0xB0,

          // LDY Zero Page,X
          op(Op::LDY_ZPX), 0xB5,

          // LDY Absolute
          op(Op::LDY_AB), 0x70, 0x30,

          // LDY Absolute,X
          op(Op::LDY_ABSX), 0x80, 0x30,

          // Now we can change X with LDX instructions
          // LDX Immediate
          op(Op::LDX_IM), 0xAA,

          // LDX Zero Page
          op(Op::LDX_ZP), 0xA0,

          // LDX Zero Page,Y
          op(Op::LDX_ZPY), 0xA5,

          // LDX Absolute
          op(Op::LDX_AB), 0x50, 0x30,

          // LDX Absolute,Y
          op(Op::LDX_ABSY), 0x60, 0x30,

          // Jump to subroutine
          op(Op::JSR), 0x50, 0x20,

          // NOP instructions
          op(Op::NOP), op(Op::NOP),

          // Use BRK to exit the program instead of RTS
          0x00}},  // BRK instruction (0x00)

        // Subroutine at 0x2050
        {0x2050,
         {// Do something in the subroutine
          op(Op::LDA_IM), 0xFF,
          // Return from subroutine
          0x00}},  // BRK instruction (0x00)

        // Data for various addressing modes
        {0x0080, {0x55}},  // Zero page data for LDA_ZP
        {0x0070, {0x00}},  // Base for Zero page,X
        {0x0075, {0x66}},  // Zero page,X data (0x70+X where X=5)
        {0x3000, {0x77}},  // Absolute data for LDA_AB
        {0x3015, {0x88}},  // Absolute,X data (0x3010+X where X=5)
        {0x3022, {0x99}},  // Absolute,Y data (0x3020+Y where Y=2)

        // LDX data locations
        {0x00A0, {0xCC}},  // Zero page data for LDX_ZP
        {0x00A7, {0xDD}},  // Zero page,Y data (0xA5+Y where Y=2)
        {0x3050, {0xEE}},  // Absolute data for LDX_AB
        {0x3062, {0xFF}},  // Absolute,Y data (0x3060+Y where Y=2)

        // LDY data locations
        {0x00B0, {0x11}},  // Zero page data for LDY_ZP
        {0x00BA, {0x22}},  // Zero page,X data (0xB5+X where X=5)
        {0x3070, {0x33}},  // Absolute data for LDY_AB
        {0x3085, {0x44}},  // Absolute,X data (0x3080+X where X=5)

        // Indirect addressing data
        {0x0090, {0x00}},        // Zero page for indirect X
        {0x0095, {0x00, 0x40}},  // Indirect address pointer after X offset (0x90+X where X=5)
        {0x4000, {0xAA}},        // Target for Indirect,X

        {0x0092, {0x30, 0x40}},  // Zero page for indirect Y
        {0x4032, {0xBB}},        // Target for Indirect,Y (0x4030+Y where Y=2)
    };

    return demo;
}

// Simple counter program that increments X register and stores it to memory
std::map<u32, std::vector<byte>> get_counter_program() {
    std::map<u32, std::vector<byte>> program = {
        // Program starts at 0x8000
        {0x8000,
         {
             // LDX #$00 - Initialize X register with 0
             op(Op::LDX_IM), 0x00,

             // Loop start (increment X and repeat)
             // INX instruction - not implemented yet, placeholder
             0xE8,  // INX opcode

             // NOP (placeholder for future STX implementation)
             op(Op::NOP),

             // JMP back to loop start (placeholder, not implemented yet)
             0x4C, 0x02, 0x80,  // JMP $8002 opcode and address
         }},

        // Reset vector points to 0x8000 (low byte first, then high byte)
        {0xFFFC, {0x00, 0x80}},
    };

    return program;
}

}  // namespace demo_programs
