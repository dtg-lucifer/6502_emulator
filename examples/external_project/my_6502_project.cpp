#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

// Include the 6502 CPU emulator API
#include "emulator_api.h"
#include "op_codes.h"

// Function to print CPU state in a nice format
void print_cpu_state(const CpuState& state) {
    // Save the current cout formatting state
    auto flags = std::cout.flags();

    // Create a nicely formatted output with borders and aligned values
    std::cout << "\n";
    std::cout << "┌───────────────── CPU STATE ─────────────────┐\n";
    std::cout << "| Cycles used: " << std::setw(6) << std::setfill(' ') << state.cycles_used;
    std::cout << "  Status: " << (state.execution_completed ? "COMPLETED" : "INCOMPLETE") << std::setw(15) << " │\n";
    std::cout << "├───────────────── REGISTERS ─────────────────┤\n";

    // Register section
    std::cout << "│ ";
    std::cout << "PC" << " (16-bit): 0x" << std::setw(4) << std::setfill('0') << std::hex << state.PC << " -- ";
    std::cout << "SP" << " (8-bit): 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(state.SP)
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

    // Restore the original cout formatting state
    std::cout.flags(flags);
}

// Create a program that calculates the Fibonacci sequence
std::vector<byte> create_fibonacci_program() {
    /* Pseudo code:
       A = 0      // First Fibonacci number
       X = 1      // Second Fibonacci number
       Y = 10     // Counter (calculate 10 Fibonacci numbers)

       loop:
         Store A to memory (0x0200 + counter position)
         temp = A + X  // Calculate next number
         A = X
         X = temp
         Decrement Y
         If Y != 0 goto loop
         BRK
    */

    std::vector<byte> program = {
        // Initialize registers
        0xA9, 0x00,  // LDA #$00 - Load A with 0 (first Fibonacci number)
        0xA2, 0x01,  // LDX #$01 - Load X with 1 (second Fibonacci number)
        0xA0, 0x0A,  // LDY #$0A - Load Y with 10 (counter)

        // Store current Fibonacci number (in A) to memory
        // We'll store at addresses 0x0200-0x0209
        0x8D, 0x00, 0x02,  // STA $0200 - Store A to address 0x0200

        // Calculate next Fibonacci number
        0x18,        // CLC - Clear carry flag before addition
        0x65, 0x02,  // ADC X - Add X to A, storing result in A
        0x86, 0x04,  // STX $04 - Store X to zero page temporarily
        0xA5, 0x04,  // LDA $04 - Load the old X value into A
        0x86, 0x05,  // STX $05 - Store the result in X (next Fibonacci number)
        0x88,        // DEY - Decrement Y (counter)
        0xD0, 0xF1,  // BNE -15 - Branch back if Y is not zero

        // End program
        0x00  // BRK - Break (halt execution)
    };

    return program;
}

// Function to write a binary file
void write_binary_file(const std::string& filename, const std::vector<byte>& data) {
    std::ofstream file(filename, std::ios::binary);

    if (!file) {
        std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
        return;
    }

    file.write(reinterpret_cast<const char*>(data.data()), data.size());

    if (!file) {
        std::cerr << "Error: Failed to write data to file: " << filename << std::endl;
        return;
    }

    std::cout << "Successfully wrote " << data.size() << " bytes to " << filename << std::endl;
}

// Function to read memory from the emulator and display it
void print_memory_range(const EmulatorApi& emulator, word start_address, int count) {
    std::cout << "\nMemory dump starting at 0x" << std::hex << start_address << ":\n";
    std::cout << "-------------------------------------\n";

    for (int i = 0; i < count; i++) {
        if (i % 8 == 0) {
            if (i > 0) std::cout << "\n";
            std::cout << std::hex << std::setw(4) << std::setfill('0') << (start_address + i) << ": ";
        }

        std::cout << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(emulator.read_memory(start_address + i)) << " ";
    }

    std::cout << "\n" << std::dec;
}

int main(int argc, char* argv[]) {
    std::cout << "=== 6502 CPU Emulator External Project Example ===" << std::endl;

    // Create an instance of the emulator API
    EmulatorApi emulator;

    // Initialize the emulator
    emulator.initialize();

    // Create and load our Fibonacci program
    std::vector<byte> fibonacci_program = create_fibonacci_program();

    // Create a binary file for future use
    write_binary_file("fibonacci.bin", fibonacci_program);

    // Program start address
    word program_start = 0x1000;

    // Load the program into memory
    emulator.set_memory_block(program_start, fibonacci_program);

    // Set the reset vector to point to our program
    emulator.set_reset_vector(program_start);

    // Execute the program
    std::cout << "\nExecuting Fibonacci program...\n";
    CpuState final_state = emulator.execute_program();

    // Print the final CPU state
    std::cout << "Program execution complete.\n";
    print_cpu_state(final_state);

    // Print the memory where Fibonacci numbers were stored
    print_memory_range(emulator, 0x0200, 10);

    // If a binary file was provided as a command line argument, load and run it
    if (argc > 1) {
        std::string binary_file = argv[1];
        std::cout << "\nLoading and executing external binary: " << binary_file << std::endl;

        if (emulator.load_binary(binary_file, 0x2000)) {
            emulator.set_reset_vector(0x2000);

            // Set some initial register values for testing
            emulator.set_register(Register::X, 0x05);
            emulator.set_register(Register::Y, 0x02);

            // Execute the loaded program
            CpuState loaded_state = emulator.execute_program();

            std::cout << "External binary execution complete.\n";
            print_cpu_state(loaded_state);
        } else {
            std::cerr << "Failed to load binary file: " << binary_file << std::endl;
        }
    }

    return 0;
}
