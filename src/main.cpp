#include <strings.h>

#include <cstdint>
#include <cstring>
#include <iostream>

using byte = uint8_t;
using word = uint16_t;
using u32 = unsigned int;

enum class Register : byte { A = 0, X = 1, Y = 2 };
enum class Op : byte {
    LDA_IM = 0xA9,
    LDA_ZP = 0xA5,
    LDA_ZPX = 0xB5,
    JSR = 0x20,
};

constexpr byte op(const Op o) {
    return static_cast<byte>(o);
}

struct Mem {
    static constexpr u32 MAX_MEM = 1024 * 64;
    byte data[MAX_MEM];

    void init() {
        std::memset(data, 0, MAX_MEM);  // Zero out the memory block
    }

    void write_word(u32& cycles, word value, u32 address) {
        data[address] = (value & 0xFF);
        data[address + 1] = (value >> 8);
        cycles -= 2;
    }

    byte operator[](u32 addr) const { return data[addr]; }

    byte& operator[](u32 addr) { return data[addr]; }
};

struct Cpu {
   private:
    void LDA_SetFlags() {
        Z = (A == 0);              // Set the Zero flag
        N = (A & 0b10000000) > 0;  // Set the Negative flag (if the 8th bit is 1, that means its the sign bit)
    }
    byte registers[3];  // Index registers

   public:
    word PC;  // Program counter register
    word SP;  // Stack pointer register

    byte& A = registers[static_cast<byte>(Register::A)];
    byte& X = registers[static_cast<byte>(Register::X)];
    byte& Y = registers[static_cast<byte>(Register::Y)];

    byte C : 1;  // Carry Flag
    byte Z : 1;  // Zero Flag
    byte I : 1;  // Interrupt Disable Flag
    byte D : 1;  // Decimal Mode Flag
    byte B : 1;  // Break Flag
    byte V : 1;  // Overflow Flag
    byte N : 1;  // Negative Flag
    byte U : 1;  // The 8th bit (can be used or not)

    byte& get(const Register r) { return registers[static_cast<byte>(r)]; }

    void set(Register r, byte val) { registers[static_cast<byte>(r)] = val; }

    void reset(Mem& mem) {
        PC = 0xFFFC;                                   // Reset the Program counter to its original position
        SP = 0x0100;                                   // Reset the stack pointer to its original position
        C = Z = I = D = B = V = N = U = 0;             // Reset the flags
        registers[static_cast<int>(Register::A)] = 0;  // Reset the registers
        registers[static_cast<int>(Register::X)] = 0;  // Reset the registers
        registers[static_cast<int>(Register::Y)] = 0;  // Reset the registers

        mem.init();
    }

    byte fetch_byte(u32& cycles, Mem& mem) {
        byte d = mem[PC];
        PC++;
        cycles--;
        return d;
    }

    word fetch_word(u32& cycles, Mem& mem) {
        // little endian mode
        word d = mem[PC];  // LSB
        PC++;

        d |= (mem[PC] << 8);  // MSB
        PC++;

        cycles -= 2;
        return d;
    }

    byte read_byte(byte zp_addr, u32& cycles, Mem& mem) {
        byte d = mem[zp_addr];
        cycles--;
        return d;
    }

    void execute(u32 cycles, Mem& mem) {
        while (cycles > 0) {
            byte ins = fetch_byte(cycles, mem);

            switch (ins) {
                case op(Op::LDA_IM): {
                    byte v = fetch_byte(cycles, mem);  // Fetch the 2nd byte from the memory
                                                       // as that is the value to be set
                    A = v;                             // Set the value to the accumulator register
                    LDA_SetFlags();                    // Set the Negative and zero flags
                } break;
                case op(Op::LDA_ZP): {
                    byte addr = fetch_byte(cycles, mem);  // Fetch the 2nd byte from the memory
                                                          // as that is the address for the zero page
                    A = read_byte(addr, cycles, mem);     // Set the address to the A register
                    LDA_SetFlags();                       // Set the Negative and zero flags
                } break;
                case op(Op::LDA_ZPX): {
                    byte addr = fetch_byte(cycles, mem);  // Fetch the 2nd byte from the memory
                                                          // as that is the address for the zero page
                    addr += X;
                    cycles--;
                    A = read_byte(addr, cycles, mem);
                    LDA_SetFlags();
                } break;
                case op(Op::JSR): {
                    word addr = fetch_word(cycles, mem);  // Get the absolute address (16bit)
                    mem.write_word(cycles, PC - 1, SP);
                    PC = addr;
                    cycles--;
                } break;
                default: {
                    std::cout << "Invalid op code: 0x" << std::hex << static_cast<int>(ins) << std::dec << " at address 0x" << std::hex << (PC - 1) << std::dec << std::endl;
                } break;
            }
        }
    }
};

int main() {
    Cpu cpu;
    Mem mem;

    cpu.reset(mem);

    // start - inline test
    mem[0xFFFC] = op(Op::JSR);
    mem[0xFFFD] = 0x42;
    mem[0xFFFE] = 0x42;

    // set something at 0x4242
    mem[0x4242] = op(Op::LDA_IM);
    mem[0x4243] = 0x84;
    // end - inline test

    cpu.execute(9, mem);

    return 0;
}
