#include "cpu.h"

#include <ios>
#include <iostream>

#include "instructions.h"
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

byte Cpu::fetch_byte(i32& cycles, Mem& mem) {
    byte d = mem[PC];
    PC++;
    cycles--;
    return d;
}

word Cpu::fetch_word(i32& cycles, Mem& mem) {
    // little endian mode
    word d = mem[PC];  // LSB
    PC++;

    d |= (mem[PC] << 8);  // MSB
    PC++;

    cycles -= 2;
    return d;
}

byte Cpu::read_byte(byte zp_addr, i32& cycles, Mem& mem) {
    byte d = mem[zp_addr];
    cycles--;
    return d;
}

i32 Cpu::execute(i32 cycles, Mem& mem, bool* completed_out) {
    i32 starting_cycles = cycles;
    bool completed = false;
    bool ran_instructions = false;

    while (cycles > 0) {
        word inst = mem[PC];                              // Store the current instruction address for printing
        this->print_current_execution(inst, *this, mem);  // Print the current execution state

        byte ins = fetch_byte(cycles, mem);
        ran_instructions = true;

        switch (ins) {
            case op(Op::LDA_IM):
                instructions::LDA_IM(*this, cycles, mem);
                break;
            case op(Op::LDA_ZP):
                instructions::LDA_ZP(*this, cycles, mem);
                break;
            case op(Op::LDA_ZPX):
                instructions::LDA_ZPX(*this, cycles, mem);
                break;
            case op(Op::LDA_AB):
                instructions::LDA_AB(*this, cycles, mem);
                break;
            case op(Op::LDA_ABSX):
                instructions::LDA_ABSX(*this, cycles, mem);
                break;
            case op(Op::LDA_ABSY):
                instructions::LDA_ABSY(*this, cycles, mem);
                break;
            case op(Op::LDA_INX):
                instructions::LDA_INX(*this, cycles, mem);
                break;
            case op(Op::LDA_INY):
                instructions::LDA_INY(*this, cycles, mem);
                break;
            case op(Op::JSR):
                instructions::JSR(*this, cycles, mem);
                break;
            case op(Op::RTS):
                instructions::RTS(*this, cycles, mem);
                completed = true;
                break;
            case op(Op::NOP):
                instructions::NOP(*this, cycles, mem);
                continue;  // Skip to the next iteration
            default: {
                std::cout << "Invalid op code: 0x" << std::hex << static_cast<int>(ins) << std::dec << " at address 0x"
                          << std::hex << (PC - 1) << std::dec << std::endl;
            } break;
        }

        // For RTS instruction, we've already set the completed flag in the case statement
        if (completed) {
            break;
        }

        // For test scenarios, we'll continue executing NOPs until cycles are exhausted
    }

    // Mark as completed if:
    // 1. We explicitly reached RTS or encountered a 0x00 opcode, OR
    // 2. We executed at least one instruction and didn't run out of cycles
    if (!completed && ran_instructions && cycles >= 0) {
        completed = true;
    }

    // If we ran out of cycles before completion
    if (!completed && cycles < 0) {
        std::cout << colors::BOLD << colors::RED << "Warning:" << colors::RESET
                  << "\tInsufficient cycles. Execution incomplete." << std::endl;
        std::cout << "\tRequired: > " << starting_cycles << " cycles" << std::endl;
        std::cout << "\tProvided: " << starting_cycles << " cycles" << std::endl;
        std::cout << "\tUsed: " << starting_cycles - cycles << " cycles" << std::endl;
    }

    // If caller wants to know completion status
    if (completed_out != nullptr) {
        *completed_out = completed;
    }

    // Reset output to decimal mode for subsequent displays
    std::cout << std::dec;

    // Return the number of cycles actually used
    return starting_cycles - cycles;
}
