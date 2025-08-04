#ifndef CPU_H
#define CPU_H

#include <iomanip>
#include <iostream>

#include "memory.h"
#include "op_codes.h"
#include "types.h"

class Cpu {
   private:
    void LDA_SetFlags();
    byte registers[3];  // Index registers

   public:
    word PC;  // Program counter register
    byte SP;  // Stack pointer register (8-bit)

    // Register references for easier access
    byte& A = registers[static_cast<byte>(Register::A)];
    byte& X = registers[static_cast<byte>(Register::X)];
    byte& Y = registers[static_cast<byte>(Register::Y)];

    // Status flags
    byte N : 1;  // Negative Flag (bit 7)
    byte V : 1;  // Overflow Flag (bit 6)
    byte U : 1;  // Unused/expansion (bit 5)
    byte B : 1;  // Break Flag (bit 4)
    byte D : 1;  // Decimal Mode Flag (bit 3)
    byte I : 1;  // Interrupt Disable Flag (bit 2)
    byte Z : 1;  // Zero Flag (bit 1)
    byte C : 1;  // Carry Flag (bit 0)

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
    i32 execute(i32 cycles, Mem& mem, bool* completed = nullptr);

    void print_state() const {
        // Save the current cout formatting state
        auto flags = std::cout.flags();

        // Create a nicer formatted output with borders and aligned values
        std::cout << "\n┌─────────────── CPU STATE ───────────────────┐\n";

        // Register section
        std::cout << "│ PC (16-bit): 0x" << std::setw(4) << std::setfill('0') << std::hex << PC << " -- "
                  << "SP (8-bit): 0x" << std::setw(4) << std::setfill('0') << std::hex << static_cast<int>(SP)
                  << "   │\n";

        std::cout << "├─────────────────────────────────────────────┤\n";

        // Display A, X, Y registers with both hex and decimal values
        std::cout << "│ A: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(A) << " ("
                  << std::setw(3) << std::setfill(' ') << std::dec << static_cast<int>(A) << ")";
        std::cout << "  X: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(X) << " ("
                  << std::setw(3) << std::setfill(' ') << std::dec << static_cast<int>(X) << ")";
        std::cout << "  Y: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(Y) << " ("
                  << std::setw(3) << std::setfill(' ') << std::dec << static_cast<int>(Y) << ") │\n";

        // Status flags section
        std::cout << "├────────────── STATUS FLAGS ─────────────────┤\n";
        std::cout << "│  N   V   U   B   D   I   Z   C              │\n";
        std::cout << "│  " << static_cast<int>(N) << "   " << static_cast<int>(V) << "   " << static_cast<int>(U)
                  << "   " << static_cast<int>(B) << "   " << static_cast<int>(D) << "   " << static_cast<int>(I)
                  << "   " << static_cast<int>(Z) << "   " << static_cast<int>(C) << "              │\n";
        std::cout << "└─────────────────────────────────────────────┘\n";

        // Restore the original cout formatting state
        std::cout.flags(flags);
    }
};

#endif  // CPU_H
