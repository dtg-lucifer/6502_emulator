#include <iostream>
#include <map>
#include <vector>

#include "cpu.h"
#include "memory.h"
#include "op_codes.h"
#include "reader.h"
#include "test_utils.h"
#include "types.h"

using namespace colors;

int main() {
    Cpu cpu;
    Mem mem;
    cpu.reset(mem);

    // Set X and Y register values for indexed addressing operations
    cpu.set(Register::X, 0x05);
    cpu.set(Register::Y, 0x02);

    // Normal operation mode (not testing)

    // Program flow:
    //  - load a C-64 assembly code
    //  - parse it
    //  - load into a binary format
    //  - feed that to the memory to read
    //  - execute the script
    //  - print the final state after halting

    // Read a comprehensive demo program that tests all implemented opcodes
    std::map<u32, std::vector<byte> > demo = {
        // Program entry point (JSR to main routine)
        {0xFFFC, {op(Op::JSR), 0x00, 0x20}},

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

          // Jump to subroutine
          op(Op::JSR), 0x50, 0x20,

          // NOP instructions
          op(Op::NOP), op(Op::NOP),

          // Return from the main program
          op(Op::RTS)}},

        // Subroutine at 0x2050
        {0x2050,
         {// Do something in the subroutine
          op(Op::LDA_IM), 0xFF,
          // Return from subroutine
          op(Op::RTS)}},

        // Data for various addressing modes
        {0x0080, {0x55}},  // Zero page data
        {0x0070, {0x00}},  // Base for Zero page,X
        {0x0075, {0x66}},  // Zero page,X data (0x70+X where X=5)
        {0x3000, {0x77}},  // Absolute data
        {0x3015, {0x88}},  // Absolute,X data (0x3010+X where X=5)
        {0x3022, {0x99}},  // Absolute,Y data (0x3020+Y where Y=2)

        // Indirect addressing data
        {0x0090, {0x00}},        // Zero page for indirect X
        {0x0095, {0x00, 0x40}},  // Indirect address pointer after X offset (0x90+X where X=5)
        {0x4000, {0xAA}},        // Target for Indirect,X

        {0x0092, {0x30, 0x40}},  // Zero page for indirect Y
        {0x4032, {0xBB}},        // Target for Indirect,Y (0x4030+Y where Y=2)
    };

    binary_reader::read_from_array(cpu, mem, demo);

    bool program_completed = false;
    i32 cycles_used = cpu.execute(100, mem, &program_completed);  // Execute the program with a limit of 100 cycles

    std::cout << "Execution " << BOLD << (program_completed ? GREEN : RED)
              << (program_completed ? "COMPLETED" : "INCOMPLETE") << RESET << " using " << cycles_used << " cycles"
              << std::endl;

    cpu.print_state();
    return 0;
}
