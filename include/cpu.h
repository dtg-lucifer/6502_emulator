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

    // Pin layout for MOS 6502 with address/data bus access
    union {
        pinl_t PINS;  // Raw access to all pins at once

        // === Individual pin mapping ===
        struct {
            pinl_t PIN_1 : 1;  // VSS   - Ground
            pinl_t PIN_2 : 1;  // RDY   - Ready (halts CPU when low)
            pinl_t PIN_3 : 1;  // PHI1O - Phase 1 clock output
            pinl_t PIN_4 : 1;  // IRQB  - Interrupt Request (active low)
            pinl_t PIN_5 : 1;  // NC    - Not connected
            pinl_t PIN_6 : 1;  // NMIB  - Non-Maskable Interrupt (active low)
            pinl_t PIN_7 : 1;  // SYNC  - Indicates opcode fetch
            pinl_t PIN_8 : 1;  // VCC   - +5V power

            // Address bus (low half)
            pinl_t A0 : 1;   // Address line bit 0
            pinl_t A1 : 1;   // Address line bit 1
            pinl_t A2 : 1;   // Address line bit 2
            pinl_t A3 : 1;   // Address line bit 3
            pinl_t A4 : 1;   // Address line bit 4
            pinl_t A5 : 1;   // Address line bit 5
            pinl_t A6 : 1;   // Address line bit 6
            pinl_t A7 : 1;   // Address line bit 7
            pinl_t A8 : 1;   // Address line bit 8
            pinl_t A9 : 1;   // Address line bit 9
            pinl_t A10 : 1;  // Address line bit 10
            pinl_t A11 : 1;  // Address line bit 11

            pinl_t VSS2 : 1;  // VSS   - Ground (second ground at pin 21)

            // Address bus (high half)
            pinl_t A12 : 1;  // Address line bit 12
            pinl_t A13 : 1;  // Address line bit 13
            pinl_t A14 : 1;  // Address line bit 14
            pinl_t A15 : 1;  // Address line bit 15

            // Data bus
            pinl_t D7 : 1;  // Data line bit 7
            pinl_t D6 : 1;  // Data line bit 6
            pinl_t D5 : 1;  // Data line bit 5
            pinl_t D4 : 1;  // Data line bit 4
            pinl_t D3 : 1;  // Data line bit 3
            pinl_t D2 : 1;  // Data line bit 2
            pinl_t D1 : 1;  // Data line bit 1
            pinl_t D0 : 1;  // Data line bit 0

            pinl_t RWB : 1;    // Read (high) / Write (low)
            pinl_t NC36 : 1;   // NC
            pinl_t NC37 : 1;   // NC
            pinl_t PHI0 : 1;   // PHI0 - External clock input
            pinl_t S0 : 1;     // S0
            pinl_t PHI2O : 1;  // PHI2O - Phase 2 clock output
            pinl_t RESB : 1;   // Reset (active low)
        };

        // === Packed access to buses ===
        struct {
            pinl_t _skipA0 : 8;  // Skip non-address pins before A0
            pinl_t ADDR : 16;    // A0..A15 (low to high bit order)
        } addr_bus;

        struct {
            pinl_t _skipD0 : 24;  // Skip pins before D7
            pinl_t DATA : 8;      // D7..D0
        } data_bus;
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
