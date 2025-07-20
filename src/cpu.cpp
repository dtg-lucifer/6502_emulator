#include "cpu.h"

#include <iostream>

#include "op_codes.h"

void Cpu::LDA_SetFlags() {
    Z = (A == 0);              // Set the Zero flag
    N = (A & 0b10000000) > 0;  // Set the Negative flag (if the 8th bit is 1, that means its the sign bit)
}

byte& Cpu::get(const Register r) {
    return registers[static_cast<byte>(r)];
}

void Cpu::set(Register r, byte val) {
    registers[static_cast<byte>(r)] = val;
}

void Cpu::reset(Mem& mem) {
    PC = 0xFFFC;                        // Reset the Program counter to its original position
    SP = 0xFF;                          // Reset the stack pointer to its original position (top of stack)
    C = Z = I = D = B = V = N = U = 0;  // Reset the flags
    A = 0;                              // Reset accumulator
    X = 0;                              // Reset X register
    Y = 0;                              // Reset Y register

    mem.init();
}

byte Cpu::fetch_byte(u32& cycles, Mem& mem) {
    byte d = mem[PC];
    PC++;
    cycles--;
    return d;
}

word Cpu::fetch_word(u32& cycles, Mem& mem) {
    // little endian mode
    word d = mem[PC];  // LSB
    PC++;

    d |= (mem[PC] << 8);  // MSB
    PC++;

    cycles -= 2;
    return d;
}

byte Cpu::read_byte(byte zp_addr, u32& cycles, Mem& mem) {
    byte d = mem[zp_addr];
    cycles--;
    return d;
}

void Cpu::execute(u32 cycles, Mem& mem) {
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
                // Push return address (PC-1) to stack - high byte first, then low byte
                mem[stack_base + SP] = (PC - 1) >> 8;  // Push high byte
                SP--;
                cycles--;
                mem[stack_base + SP] = (PC - 1) & 0xFF;  // Push low byte
                SP--;
                cycles--;
                PC = addr;
                cycles--;
            } break;
            case op(Op::RTS): {
                // Pull return address from stack - low byte first, then high byte
                SP++;
                cycles--;
                byte lo = mem[stack_base + SP];

                SP++;
                cycles--;
                byte hi = mem[stack_base + SP];

                // Reconstruct the 16-bit address
                word return_addr = (hi << 8) | lo;

                // Set PC to return address + 1 (since JSR stored PC-1)
                PC = return_addr + 1;
                cycles -= 2;  // Extra cycles for updating PC
            } break;
            default: {
                std::cout << "Invalid op code: 0x" << std::hex << static_cast<int>(ins) << std::dec << " at address 0x" << std::hex << (PC - 1) << std::dec << std::endl;
            } break;
        }
    }
}
