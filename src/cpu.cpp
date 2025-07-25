#include "cpu.h"

#include <iostream>

#include "op_codes.h"
#include "test_utils.h"

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
        byte ins = fetch_byte(cycles, mem);
        ran_instructions = true;

        switch (ins) {
            case op(Op::LDA_IM): {
                byte v = fetch_byte(cycles, mem);  // Fetch the 2nd byte from the memory
                // as that is the value to be set
                this->set(Register::A, v);  // Set the value to the A register
                LDA_SetFlags();             // Set the Negative and zero flags
            } break;
            case op(Op::LDA_ZP): {
                byte addr = fetch_byte(cycles, mem);                   // Fetch the 2nd byte from the memory
                                                                       // as that is the address for the zero page
                this->set(Register::A, read_byte(addr, cycles, mem));  // Set the address to the A register
                LDA_SetFlags();                                        // Set the Negative and zero flags
            } break;
            case op(Op::LDA_ZPX): {
                byte addr = fetch_byte(cycles, mem);  // Fetch the 2nd byte from the memory
                                                      // as that is the address for the zero page
                addr += this->get(Register::X);       // Add the X register value to the address
                cycles--;
                this->set(Register::A, read_byte(addr, cycles, mem));
                LDA_SetFlags();
            } break;
            case op(Op::LDA_AB): {
                word addr = fetch_word(cycles, mem);  // Fetch the 2nd and 3rd byte from the memory
                                                      // as that is the absolute address
                this->set(Register::A, mem[addr]);    // Set the value at the absolute address to the A register
                cycles--;                             // Fetching the absolute address takes 1 cycle
                LDA_SetFlags();                       // Set the Negative and zero flags
            } break;
            case op(Op::LDA_ABSX): {
                word addr = fetch_word(cycles, mem);  // Fetch the 2nd and 3rd byte from the memory
                                                      // as that is the absolute address
                addr += this->get(Register::X);       // Add the X register value to the address
                cycles--;                             // Fetching the absolute address takes 1 cycle
                A = mem[addr];                        // Set the value at the absolute address to the A register
                LDA_SetFlags();                       // Set the Negative and zero flags
            } break;
            case op(Op::LDA_ABSY): {
                word addr = fetch_word(cycles, mem);  // Fetch the 2nd and 3rd byte from the memory
                                                      // as that is the absolute address
                addr += this->get(Register::Y);       // Add the Y register value to the
                cycles--;                             // Fetching the absolute address takes 1 cycle
                this->set(Register::A, mem[addr]);    // Set the value at the absolute address to the A register
                LDA_SetFlags();                       // Set the Negative and zero flags
            } break;
            case op(Op::LDA_INX): {
                byte addr = fetch_byte(cycles, mem);      // Fetch the 2nd byte from the memory
                                                          // as that is the zero page address
                addr += this->get(Register::X);           // Add the X register value to the
                cycles--;                                 // Fetching the zero page address takes 1 cycle
                byte low = mem[addr];                     // Low byte of the effective address
                byte high = mem[addr + 1];                // High byte of the effective address
                cycles -= 2;                              // Reading the effective address takes 2 cycles
                word effective_addr = (high << 8) | low;  // Combine low and
                this->set(Register::A,
                          mem[effective_addr]);  // Set the value at the effective address to the A register
                LDA_SetFlags();                  // Set the Negative and zero flags
            } break;
            case op(Op::LDA_INY): {
                byte addr = fetch_byte(cycles, mem);  // Fetch zero page address
                cycles--;                             // Fetching zero page address takes 1 cycle

                // Read the 16-bit address from zero page, with wraparound
                byte low = mem[addr];                // Low byte of the effective address
                byte high = mem[(addr + 1) & 0xFF];  // High byte with zero-page wraparound
                cycles -= 2;                         // Reading the effective address takes 2 cycles

                word effective_addr = (high << 8) | low;   // Combine to form 16-bit address
                effective_addr += this->get(Register::Y);  // Add Y register offset

                this->set(Register::A, mem[effective_addr]);  // Load value into A
                LDA_SetFlags();                               // Set the flags
            } break;
            case op(Op::JSR): {
                word addr = fetch_word(cycles, mem);  // Get the absolute address (16bit)
                // Push return address (PC-1) to stack - high byte first, then low byte
                mem[0x0100 + SP] = (PC - 1) >> 8;  // Push high byte
                SP--;
                cycles--;
                mem[0x0100 + SP] = (PC - 1) & 0xFF;  // Push low byte
                SP--;
                cycles--;
                PC = addr;
                cycles--;
            } break;
            case op(Op::RTS): {
                // Pull return address from stack - low byte first, then high byte
                SP++;
                cycles--;
                byte lo = mem[0x0100 + SP];  // Read from stack address

                SP++;
                cycles--;
                byte hi = mem[0x0100 + SP];  // Read from stack address

                // Reconstruct the 16-bit address
                word return_addr = (hi << 8) | lo;

                // Set PC to return address + 1 (since JSR stored PC-1)
                PC = return_addr + 1;
                cycles -= 2;  // Extra cycles for updating PC
            } break;
            case op(Op::NOP): {
                // No operation, just consume a cycle
                cycles--;
                continue;  // Skip to the next iteration
            }
            default: {
                std::cout << "Invalid op code: 0x" << std::hex << static_cast<int>(ins) << std::dec << " at address 0x"
                          << std::hex << (PC - 1) << std::dec << std::endl;
            } break;
        }

        // Mark execution as complete after RTS instruction
        if (ins == op(Op::RTS)) {
            completed = true;
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
    if (!completed && cycles <= 0) {
        std::cout << colors::BOLD << colors::RED << "Warning:" << colors::RESET
                  << " Insufficient cycles. Execution incomplete." << std::endl;
        std::cout << "  Required: > " << starting_cycles << " cycles" << std::endl;
        std::cout << "  Provided: " << starting_cycles << " cycles" << std::endl;
        std::cout << "  Used: " << starting_cycles - cycles << " cycles" << std::endl;
    }

    // If caller wants to know completion status
    if (completed_out != nullptr) {
        *completed_out = completed;
    }

    // Return the number of cycles actually used
    return starting_cycles - cycles;
}
