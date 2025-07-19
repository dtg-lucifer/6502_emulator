#include <cstdint>
#include <iostream>

using Byte = uint8_t;
using Word = uint16_t;

enum class Register : Byte { A = 0, X = 1, Y = 2 };

struct Cpu {
    Word PC;  // Program counter register
    Word SP;  // Stack pointer register

    Byte registers[3];  // Index registers

    Byte C : 1;  // Carry Flag
    Byte Z : 1;  // Zero Flag
    Byte I : 1;  // Interrupt Disable Flag
    Byte D : 1;  // Decimal Mode Flag
    Byte B : 1;  // Break Flag
    Byte V : 1;  // Overflow Flag
    Byte N : 1;  // Negative Flag
    Byte U : 1;  // The 8th bit (can be used or not)

    Byte& get(const Register r) { return registers[static_cast<int>(r)]; }

    void reset() {
        PC = 0xFFFC;
        SP = 0x0100;
        C = Z = I = D = B = V = N = U = 0;
        registers[static_cast<int>(Register::A)] = 0;
        registers[static_cast<int>(Register::X)] = 0;
        registers[static_cast<int>(Register::Y)] = 0;
    }
};

int main() {
    Cpu cpu;

    cpu.reset();

    cpu.get(Register::A) = 0x42;

    std::cout << "Register A: " << static_cast<int>(cpu.get(Register::A)) << std::endl;

    return 0;
}
