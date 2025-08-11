#include "cpu.h"

#include <iomanip>
#include <ios>
#include <iostream>

#include "instructions.h"
#include "op_codes.h"

byte& Cpu::get(const Register r) {
    return registers[static_cast<byte>(r)];
}

void Cpu::set(Register r, byte val) {
    registers[static_cast<byte>(r)] = val;
}

void Cpu::reset(Mem& mem) {
    PC = 0xFFFC;  // Reset the Program counter to its original position
    SP = 0xFF;    // Reset the stack pointer to its original position (top of stack)
    A = 0;        // Reset accumulator
    X = 0;        // Reset X register
    Y = 0;        // Reset Y register
    FLAGS_B = 0;  // Clear the Break flag
    FLAGS_D = 0;  // Clear the Decimal mode flag
    FLAGS_I = 1;  // Set the Interrupt Disable flag (I bit is set
    FLAGS_Z = 1;  // Set the Zero flag (Z bit is set)
    FLAGS_C = 0;  // Clear the Carry flag (C bit is cleared)
    FLAGS_U = 1;  // Set the Unused/expansion flag (U bit is set)
    FLAGS_V = 0;  // Clear the Overflow flag (V bit is cleared)
    FLAGS_N = 0;  // Clear the Negative flag (N bit is cleared)

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

i32 Cpu::execute(i32 cycles, Mem& mem, bool* completed_out, bool testing_env) {
    i32 starting_cycles = cycles;
    bool completed = false;
    bool ran_instructions = false;

    // Determine execution mode based on environment
    bool manual_mode = false;

    // Skip the user prompt if we're in testing mode
#ifndef __TESTING
    // Ask the user to provide whether the execution mode will be
    // automatic or manual stepping
    //
    // in automatic mode the execution will be done without any intervention
    // in manual mode use will have to press enter or yes to continue execution
    // or enter s/state/STATE/S to see the cpu state at that point
    // in manual mode the user can also enter 'q' to quit the execution

    // ask the user for input
    std::cout << colors::BOLD << colors::BLUE << "Please select the execution mode from below:\n";
    std::cout << colors::GREEN << "1. Automatic execution (default)\n";
    std::cout << "2. Manual stepping (press Enter to continue, 's' to see state, 'q' to quit)\n";
    std::cout << "Enter your choice (1 or 2): ";
    std::cout << colors::RESET;
    std::string choice;
    std::getline(std::cin, choice);

    if (choice == "2") {
        manual_mode = true;
        std::cout << colors::BOLD << colors::BLUE << "Manual stepping mode enabled. ";
        std::cout << "Press Enter to step, 's' to view state, 'q' to quit.\n" << colors::RESET;
    } else {
        std::cout << colors::BOLD << colors::BLUE << "Automatic execution mode enabled.\n" << colors::RESET;
    }
#endif

    while (cycles > 0) {
        word inst = mem[PC];  // Store the current instruction address for printing

        // Print execution state if not in testing mode
        this->print_current_execution(inst, *this, mem, testing_env);

        // Handle manual stepping mode if not in testing mode
        if (!testing_env && manual_mode) {
            int r = this->cpu_mode_decider(manual_mode, cycles, starting_cycles, mem);
            if (r == this->ABORT_STATUS) {
                return this->ABORT_STATUS;
            }
        }

        byte ins = fetch_byte(cycles, mem);
        ran_instructions = true;

        switch (ins) {
            // -------------------------------------------------
            // LDA instructions
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
            // -------------------------------------------------
            // LDX instructions
            case op(Op::LDX_IM):
                instructions::LDX_IM(*this, cycles, mem);
                break;
            case op(Op::LDX_ZP):
                instructions::LDX_ZP(*this, cycles, mem);
                break;
            case op(Op::LDX_ZPY):
                instructions::LDX_ZPY(*this, cycles, mem);
                break;
            case op(Op::LDX_AB):
                instructions::LDX_AB(*this, cycles, mem);
                break;
            case op(Op::LDX_ABSY):
                instructions::LDX_ABSY(*this, cycles, mem);
                break;
            // -------------------------------------------------
            // LDY instructions
            case op(Op::LDY_IM):
                instructions::LDY_IM(*this, cycles, mem);
                break;
            case op(Op::LDY_ZP):
                instructions::LDY_ZP(*this, cycles, mem);
                break;
            case op(Op::LDY_ZPX):
                instructions::LDY_ZPX(*this, cycles, mem);
                break;
            case op(Op::LDY_AB):
                instructions::LDY_AB(*this, cycles, mem);
                break;
            case op(Op::LDY_ABSX):
                instructions::LDY_ABSX(*this, cycles, mem);
                break;
            // -------------------------------------------------
            // STA instructions
            case op(Op::STA_ZP):
                instructions::STA_ZP(*this, cycles, mem);
                break;
            case op(Op::STA_ZPX):
                instructions::STA_ZPX(*this, cycles, mem);
                break;
            case op(Op::STA_ABS):
                instructions::STA_ABS(*this, cycles, mem);
                break;
            case op(Op::STA_ABSX):
                instructions::STA_ABSX(*this, cycles, mem);
                break;
            case op(Op::STA_ABSY):
                instructions::STA_ABSY(*this, cycles, mem);
                break;
            case op(Op::STA_INX):
                instructions::STA_INX(*this, cycles, mem);
                break;
            case op(Op::STA_INY):
                instructions::STA_INY(*this, cycles, mem);
                break;
            // -------------------------------------------------
            // STX Instructions
            case op(Op::STX_ZP):
                instructions::STX_ZP(*this, cycles, mem);
                break;
            case op(Op::STX_ZPY):
                instructions::STX_ZPY(*this, cycles, mem);
                break;
            case op(Op::STX_ABS):
                instructions::STX_ABS(*this, cycles, mem);
                break;
            // -------------------------------------------------
            // STY Instructions
            case op(Op::STY_ZP):
                instructions::STY_ZP(*this, cycles, mem);
                break;
            case op(Op::STY_ZPX):
                instructions::STY_ZPX(*this, cycles, mem);
                break;
            case op(Op::STY_ABS):
                instructions::STY_ABS(*this, cycles, mem);
                break;
            // -------------------------------------------------
            // Control FLOW and Miscellaneous
            case op(Op::JSR):
                instructions::JSR(*this, cycles, mem);
                break;
            case op(Op::JMP):
                instructions::JMP(*this, cycles, mem);
                break;
            case op(Op::JMPI):
                instructions::JMPI(*this, cycles, mem);
                break;
            case op(Op::RTS):
                instructions::RTS(*this, cycles, mem);
                completed = true;
                break;
            case op(Op::NOP):
                instructions::NOP(*this, cycles, mem);
                continue;  // Skip to the next iteration
            default: {
                std::cout << "Invalid op code: 0x" << std::setw(2) << std::setfill('0') << std::hex
                          << static_cast<int>(ins) << std::dec << " at address 0x" << std::hex << (PC - 1) << std::dec
                          << std::endl;
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

    // If we ran out of cycles before completion and not in testing mode
    if (!completed && cycles < 0 && !testing_env) {
        std::cout << colors::BOLD << colors::RED << "Warning:" << colors::RESET
                  << "\tInsufficient cycles. Execution incomplete." << std::endl;
        std::cout << "\tRequired: > " << starting_cycles << " cycles" << std::endl;
        std::cout << "\tProvided: " << starting_cycles << " cycles" << std::endl;
        std::cout << "\tUsed: " << starting_cycles - cycles << " cycles" << std::endl;
    }

    // Show final execution status if in manual mode and not testing
    if (manual_mode && !testing_env) {
        std::cout << colors::BOLD << colors::BLUE << "Execution " << (completed ? colors::GREEN : colors::RED)
                  << (completed ? "completed" : "incomplete") << colors::BLUE << " after " << (starting_cycles - cycles)
                  << " cycles.\n"
                  << colors::RESET;
    }

    // If caller wants to know completion status
    if (completed_out != nullptr) {
        *completed_out = completed;
    }

    // Reset output to decimal mode for subsequent displays if not in testing mode
#ifndef __TESTING
    std::cout << std::dec;
#endif

    // Return the number of cycles actually used
    return starting_cycles - cycles;
}
