#include <iomanip>
#include <iostream>
#include <string>

#include "emulator_api.h"
#include "op_codes.h"

// Helper function to print CPU state
void print_cpu_state(const CpuState& state) {
    std::cout << "\n┌───────────────── CPU STATE ─────────────────┐\n";
    std::cout << "| Execution " << (state.execution_completed ? "COMPLETED" : "INCOMPLETE") << " using "
              << state.cycles_used << " cycles" << std::setw(13) << std::setfill(' ') << " │\n";
    std::cout << "├───────────────── REGISTERS ─────────────────┤\n";

    // Register section
    std::cout << "│ ";
    std::cout << "PC" << " (16-bit): 0x" << std::setw(4) << std::setfill('0') << std::hex << state.PC << " -- ";
    std::cout << "SP" << " (8-bit): 0x01" << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(state.SP)
              << "   │\n";

    std::cout << "├─────────────────────────────────────────────┤\n";

    // Display A, X, Y registers with both hex and decimal values
    std::cout << "│ ";
    std::cout << "A" << ": 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(state.A) << " ("
              << std::setw(3) << std::setfill(' ') << std::dec << static_cast<int>(state.A) << ")";

    std::cout << "  " << "X" << ": 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(state.X)
              << " (" << std::setw(3) << std::setfill(' ') << std::dec << static_cast<int>(state.X) << ")";

    std::cout << "  " << "Y" << ": 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(state.Y)
              << " (" << std::setw(3) << std::setfill(' ') << std::dec << static_cast<int>(state.Y) << ")" << " │\n";

    // Status flags section
    std::cout << "├─────────────── STATUS FLAGS ────────────────┤\n";
    std::cout << "│  ";
    std::cout << "N   V   U   B   D   I   Z   C" << "              │\n";

    std::cout << "│  " << static_cast<int>(state.flags.N) << "   " << static_cast<int>(state.flags.V) << "   "
              << static_cast<int>(state.flags.U) << "   " << static_cast<int>(state.flags.B) << "   "
              << static_cast<int>(state.flags.D) << "   " << static_cast<int>(state.flags.I) << "   "
              << static_cast<int>(state.flags.Z) << "   " << static_cast<int>(state.flags.C) << "              │\n";

    std::cout << "└─────────────────────────────────────────────┘\n";
    std::cout << std::dec;  // Reset to decimal output
}

// Example 1: Create a simple program that uses LDA, LDX, LDY instructions
void example_simple_program() {
    std::cout << "\n=== Example 1: Simple Program ===\n";

    EmulatorApi emulator;
    emulator.initialize();

    // Program start address
    word program_start = 0x1000;

    // Create a simple program: LDA #$42, LDX #$24, LDY #$10, BRK
    std::vector<byte> program = {
        op(Op::LDA_IM),
        0x42,  // LDA #$42
        op(Op::LDX_IM),
        0x24,  // LDX #$24
        op(Op::LDY_IM),
        0x10,  // LDY #$10
        0x00   // BRK (halt execution)
    };

    // Load the program into memory
    emulator.set_memory_block(program_start, program);

    // Set the reset vector to point to our program
    emulator.set_reset_vector(program_start);

    // Execute the program
    CpuState final_state = emulator.execute_program();

    // Print the final CPU state
    std::cout << "Program executed. Final CPU state:\n";
    print_cpu_state(final_state);
}

// Example 2: Load from binary file
bool example_load_binary(const std::string& filename) {
    std::cout << "\n=== Example 2: Load Binary File ===\n";

    EmulatorApi emulator;
    emulator.initialize();

    // Try to load the binary file at address 0x2000
    if (!emulator.load_binary(filename, 0x2000)) {
        std::cout << "Failed to load binary file: " << filename << std::endl;
        return false;
    }

    // Set the reset vector to the load address
    emulator.set_reset_vector(0x2000);

    // Set initial register values before execution
    emulator.set_register(Register::X, 0x05);
    emulator.set_register(Register::Y, 0x02);

    // Execute the program
    CpuState final_state = emulator.execute_program();

    // Print the final CPU state
    std::cout << "Binary program executed. Final CPU state:\n";
    print_cpu_state(final_state);

    return true;
}

// Example 3: Create a counter program
void example_counter_program() {
    std::cout << "\n=== Example 3: Counter Program ===\n";

    EmulatorApi emulator;
    emulator.initialize();

    // Program start address
    word program_start = 0x4000;

    // Create a program that counts from 0 to 10 in the X register
    std::vector<byte> program = {
        op(Op::LDX_IM),
        0x00,  // LDX #$00 - Initialize X with 0
        0xE8,  // INX - Increment X
        op(Op::STA_ZP),
        0x80,  // STA $80 - Store A at zero page address $80
        op(Op::LDA_IM),
        0x0A,  // LDA #$0A - Load 10 into A
        0xE0,  // CPX - Compare X with immediate value (next byte)
        0x0A,  // Value 10
        0xD0,  // BNE - Branch if not equal
        0xF8,  // -8 bytes (branch back to INX)
        0x00   // BRK - Halt execution
    };

    // Load the program into memory
    emulator.set_memory_block(program_start, program);

    // Set the reset vector to point to our program
    emulator.set_reset_vector(program_start);

    // Execute the program
    CpuState final_state = emulator.execute_program();

    // Print the final CPU state
    std::cout << "Counter program executed. Final CPU state:\n";
    print_cpu_state(final_state);

    // Print the value stored at $80
    byte result = emulator.read_memory(0x80);
    std::cout << "Value stored at memory location 0x80: 0x" << std::hex << static_cast<int>(result) << std::dec
              << std::endl;
}

int main(int argc, char* argv[]) {
    // Run example 1: Simple program
    example_simple_program();

    // Run example 2: Load binary file if one is provided
    if (argc > 1) {
        example_load_binary(argv[1]);
    } else {
        std::cout << "\nNo binary file provided for example 2. Skipping...\n";
        std::cout << "Usage: " << argv[0] << " [binary_file_path]\n";
    }

    // Run example 3: Counter program
    example_counter_program();

    return 0;
}
