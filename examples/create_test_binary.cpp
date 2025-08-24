#include <fstream>
#include <iostream>
#include <vector>

#include "op_codes.h"
#include "types.h"

// Utility to create test binary files for the 6502 CPU emulator
// This program creates a simple binary file that can be loaded by the emulator

using byte = unsigned char;

// Function to write a binary file
bool write_binary_file(const std::string& filename, const std::vector<byte>& data) {
    std::ofstream file(filename, std::ios::binary);

    if (!file) {
        std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
        return false;
    }

    file.write(reinterpret_cast<const char*>(data.data()), data.size());

    if (!file) {
        std::cerr << "Error: Failed to write data to file: " << filename << std::endl;
        return false;
    }

    std::cout << "Successfully wrote " << data.size() << " bytes to " << filename << std::endl;
    return true;
}

// Create a simple test program
std::vector<byte> create_simple_program() {
    std::vector<byte> program = {
        0xA9, 0x42,  // LDA #$42 - Load accumulator with 0x42
        0xA2, 0x24,  // LDX #$24 - Load X register with 0x24
        0xA0, 0x10,  // LDY #$10 - Load Y register with 0x10
        0x00         // BRK - Break (halt execution)
    };

    return program;
}

// Create a program that loads all registers with different addressing modes
std::vector<byte> create_comprehensive_program() {
    std::vector<byte> program = {
        // Set X and Y registers for indexed addressing
        0xA2, 0x05,  // LDX #$05 - Set X to 5
        0xA0, 0x02,  // LDY #$02 - Set Y to 2

        // Test LDA instructions with various addressing modes
        0xA9, 0x42,        // LDA #$42 - Immediate
        0xA5, 0x80,        // LDA $80 - Zero Page
        0xB5, 0x70,        // LDA $70,X - Zero Page,X
        0xAD, 0x00, 0x30,  // LDA $3000 - Absolute
        0xBD, 0x10, 0x30,  // LDA $3010,X - Absolute,X
        0xB9, 0x20, 0x30,  // LDA $3020,Y - Absolute,Y
        0xA1, 0x90,        // LDA ($90,X) - Indirect,X
        0xB1, 0x92,        // LDA ($92),Y - Indirect,Y

        // Test store instructions
        0x85, 0xF0,  // STA $F0 - Store A in zero page $F0
        0x86, 0xF1,  // STX $F1 - Store X in zero page $F1
        0x84, 0xF2,  // STY $F2 - Store Y in zero page $F2

        // End program
        0x00  // BRK - Break (halt execution)
    };

    // Include data for the different addressing modes
    // We'll append these at the end for convenience - the emulator will need to
    // place them at the correct memory locations

    // Data for various addressing modes
    std::vector<byte> memory_data = {
        0x80, 0x55,        // Zero page address $80 contains $55
        0x70, 0x00,        // Zero page address $70 contains $00
        0x75, 0x66,        // Zero page address $75 contains $66 (for ZP,X where X=5)
        0x00, 0x30, 0x77,  // Absolute address $3000 contains $77
        0x15, 0x30, 0x88,  // Absolute address $3015 contains $88 (for ABS,X where X=5)
        0x22, 0x30, 0x99,  // Absolute address $3022 contains $99 (for ABS,Y where Y=2)

        // Indirect addressing data
        0x90, 0x00,        // Zero page for indirect X
        0x95, 0x00, 0x40,  // Indirect address pointer after X offset (0x90+X where X=5)
        0x00, 0x40, 0xAA,  // Target for Indirect,X

        0x92, 0x30, 0x40,  // Zero page for indirect Y
        0x32, 0x40, 0xBB   // Target for Indirect,Y (0x4030+Y where Y=2)
    };

    // We don't append memory_data to the program since it needs to be placed at specific
    // addresses in memory. This is just for documentation purposes.

    return program;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <output_filename> [program_type]\n";
        std::cout << "  program_type: 1 = simple program (default), 2 = comprehensive program\n";
        return 1;
    }

    std::string filename = argv[1];
    int program_type = (argc > 2) ? std::stoi(argv[2]) : 1;

    std::vector<byte> program;

    switch (program_type) {
        case 2:
            std::cout << "Creating comprehensive test program...\n";
            program = create_comprehensive_program();
            break;
        case 1:
        default:
            std::cout << "Creating simple test program...\n";
            program = create_simple_program();
            break;
    }

    if (write_binary_file(filename, program)) {
        std::cout << "Binary file created successfully.\n";
        return 0;
    } else {
        std::cerr << "Failed to create binary file.\n";
        return 1;
    }
}
