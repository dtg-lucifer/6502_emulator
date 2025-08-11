#ifndef CPU_H
#define CPU_H

#include <iomanip>
#include <iostream>

#include "colors.h"
#include "memory.h"
#include "op_codes.h"
#include "types.h"

class Cpu {
   private:
    void LDA_SetFlags();
    byte registers[3];          // Index registers
    i32 ABORT_STATUS = -99999;  // Special status for aborting execution

   public:
    word PC;  // Program counter register
    byte SP;  // Stack pointer register (8-bit)

    // Register references for easier access
    byte& A = registers[static_cast<byte>(Register::A)];
    byte& X = registers[static_cast<byte>(Register::X)];
    byte& Y = registers[static_cast<byte>(Register::Y)];

    union {
        byte FLAGS;  // Status flags byte
        struct {
            byte FLAGS_N : 1;  // Negative Flag (bit 7)
            byte FLAGS_V : 1;  // Overflow Flag (bit 6)
            byte FLAGS_U : 1;  // Unused/expansion (bit 5)
            byte FLAGS_B : 1;  // Break Flag (bit 4)
            byte FLAGS_D : 1;  // Decimal Mode Flag (bit 3)
            byte FLAGS_I : 1;  // Interrupt Disable Flag (bit 2)
            byte FLAGS_Z : 1;  // Zero Flag (bit 1)
            byte FLAGS_C : 1;  // Carry Flag (bit 0)
        };
    };

    // Register access methods
    byte& get(const Register r);
    void set(Register r, byte val);

    // CPU operations
    void reset(Mem& mem);
    byte fetch_byte(i32& cycles, Mem& mem);
    word fetch_word(i32& cycles, Mem& mem);
    byte read_byte(byte zp_addr, i32& cycles, Mem& mem);
    // Execute CPU instructions for the given number of cycles
    // Returns the number of cycles actually used
    // Sets the completed flag to true if execution finished with RTS
    i32 execute(i32 cycles, Mem& mem, bool* completed = nullptr, bool testing_env = false);

    i32 cpu_mode_decider(bool manual_mode, i32& cycles, i32 starting_cycles, Mem& mem, bool* completed_out = nullptr) {
        if (manual_mode) {
            while (true) {
                std::cout << colors::YELLOW << "[Step: Enter/s/q]: " << colors::RESET;
                std::string input;
                std::getline(std::cin, input);

                // Convert input to lowercase for case-insensitive comparison
                for (auto& c : input) {
                    c = std::tolower(c);
                }

                if (input.empty() || input == "y" || input == "yes") {
                    // Continue execution - just break from this loop
                    break;
                } else if (input == "s" || input == "state") {
                    // Show CPU state without advancing
                    print_state(starting_cycles - cycles, false);
                    continue;
                } else if (input == "q" || input == "quit") {
                    // Quit execution
                    std::cout << colors::BOLD << colors::BLUE << "Execution terminated by user.\n" << colors::RESET;

                    // If caller wants to know completion status
                    if (completed_out != nullptr) {
                        *completed_out = false;
                    }

                    // Return the number of cycles actually used
                    return ABORT_STATUS;
                } else {
                    std::cout << colors::RED << "Invalid input. Press Enter to continue, 's' for state, 'q' to quit.\n"
                              << colors::RESET;
                    continue;
                }
            }
        }
        return starting_cycles - cycles;
    }

    void print_current_execution(word ins, Cpu& cpu, Mem& mem, bool testing_env = false) {
        // Skip printing in testing mode
        if (testing_env) {
            return;
        }

        word operand_word = static_cast<word>(mem[PC]) | (static_cast<word>(mem[PC + 1]) << 8);

        std::cout << colors::BOLD << colors::BLUE;
        std::cout << "0x" << std::setfill('0') << std::setw(4) << std::hex << (PC) << ": ";
        std::cout << colors::GREEN << "sp = 0x01" << std::setfill('0') << std::setw(2) << std::hex
                  << static_cast<int>(SP) << "  ";
        std::cout << "pc = 0x" << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(PC) << "  ";
        std::cout << colors::RESET << colors::BLUE;
        std::cout << "ins = 0x" << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(ins);
        std::cout << " [" << std::setfill(' ') << std::setw(10) << opcodes::from_byte(ins) << "]";
        std::cout << " [argument (next 2-bytes) = 0x" << std::setfill('0') << std::setw(4) << std::hex << operand_word
                  << "]";
        std::cout << colors::RESET << std::endl;
        std::cout << std::dec;  // Ensure we print in decimal mode for the rest of the output
    }

    void print_state(int cycles_used, bool program_completed, bool testing_env = false) const {
        // Skip printing in testing mode
        if (testing_env) {
            return;
        }
        // Save the current cout formatting state
        auto flags = std::cout.flags();

        // Create a nicer formatted output with borders and aligned values
        std::cout << colors::GREEN << "\n";
        std::cout << "┌───────────────── CPU STATE ─────────────────┐\n";
        std::cout << "| " << colors::BOLD << "Execution " << (program_completed ? colors::GREEN : colors::RED)
                  << (program_completed ? "COMPLETED" : "INCOMPLETE") << colors::RESET << colors::GREEN << colors::BOLD
                  << " using " << cycles_used << " cycles" << std::setw(13) << std::setfill(' ') << " │\n";
        std::cout << "├───────────────── REGISTERS ─────────────────┤\n";
        // Register section
        std::cout << "│ ";
        std::cout << colors::BOLD << "PC" << colors::GREEN << " (16-bit): 0x" << std::setw(4) << std::setfill('0')
                  << std::hex << PC << " -- ";
        std::cout << colors::BOLD << "SP" << colors::GREEN << " (8-bit): 0x01" << std::setw(2) << std::setfill('0')
                  << std::hex << static_cast<int>(SP) << "   │\n";

        std::cout << "├─────────────────────────────────────────────┤\n";

        // Display A, X, Y registers with both hex and decimal values
        std::cout << "│ ";
        std::cout << colors::BOLD << "A" << colors::GREEN << ": 0x" << std::setw(2) << std::setfill('0') << std::hex
                  << static_cast<int>(A) << " (" << std::setw(3) << std::setfill(' ') << std::dec << static_cast<int>(A)
                  << ")";

        std::cout << "  " << colors::BOLD << "X" << colors::GREEN << ": 0x" << std::setw(2) << std::setfill('0')
                  << std::hex << static_cast<int>(X) << " (" << std::setw(3) << std::setfill(' ') << std::dec
                  << static_cast<int>(X) << ")";

        std::cout << "  " << colors::BOLD << "Y" << colors::GREEN << ": 0x" << std::setw(2) << std::setfill('0')
                  << std::hex << static_cast<int>(Y) << " (" << std::setw(3) << std::setfill(' ') << std::dec
                  << static_cast<int>(Y) << ")"
                  << " │\n";

        // Status flags section
        std::cout << "├─────────────── STATUS FLAGS ────────────────┤\n";
        std::cout << "│  ";
        std::cout << colors::BOLD << "N   V   U   B   D   I   Z   C" << colors::GREEN << "              │\n";

        std::cout << "│  " << static_cast<int>(FLAGS_N) << "   " << static_cast<int>(FLAGS_V) << "   "
                  << static_cast<int>(FLAGS_U) << "   " << static_cast<int>(FLAGS_B) << "   "
                  << static_cast<int>(FLAGS_D) << "   " << static_cast<int>(FLAGS_I) << "   "
                  << static_cast<int>(FLAGS_Z) << "   " << static_cast<int>(FLAGS_C) << "              │\n";

        std::cout << "└─────────────────────────────────────────────┘" << colors::RESET << "\n";

        // Restore the original cout formatting state
        std::cout.flags(flags);
    }
};

#endif  // CPU_H
