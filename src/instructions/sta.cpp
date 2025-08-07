#include "instructions.h"
#include "op_codes.h"

namespace instructions {

// STA Zero Page mode
void STA_ZP(Cpu& cpu, i32& cycles, Mem& mem) {
    // 1. Fetch the zero page address
    byte addr = cpu.fetch_byte(cycles, mem);  // takes 1 cycle
    // 2. Store the value of the accumulator at that address
    mem[addr] = cpu.get(Register::A);  // Get the accumulator
    // 3. Decrement cycles as needed
    cycles--;  // 1 cycle for storing
    cycles--;  // 1 cycle
    // total 3 cycles
}

// STA Zero Page,X mode
void STA_ZPX(Cpu& cpu, i32& cycles, Mem& mem) {
    // 1. Fetch the zero page address
    byte addr = cpu.fetch_byte(cycles, mem);  // takes 1 cycle
    // 2. Add X register to it (with zero page wraparound)
    addr = (addr + cpu.get(Register::X)) & 0xFF;  // Zero page wraparound
    // 3. Store the value of the accumulator at the calculated address
    mem[addr] = cpu.get(Register::A);
    // 4. Decrement cycles as needed
    cycles--;  // 1 cycle for adding
    cycles--;  // 1 cycle for storing
    cycles--;  // 1 cycle
}

// STA Absolute mode
void STA_ABS(Cpu& cpu, i32& cycles, Mem& mem) {
    // 1. Fetch the absolute address (16-bit)
    word addr = cpu.fetch_word(cycles, mem);  // takes 2 cycles
    // 2. Store the value of the accumulator at that address
    mem[addr] = cpu.get(Register::A);
    // 3. Decrement cycles as needed
    cycles--;  // 1 cycle for storing
    cycles--;  // 1 final cycle
}

// STA Absolute,X mode
void STA_ABSX(Cpu& cpu, i32& cycles, Mem& mem) {
    // 1. Fetch the absolute address (16-bit)
    word addr = cpu.fetch_word(cycles, mem);  // takes 2 cycles
    // 2. Add X register to it
    addr += cpu.get(Register::X);  // Add X to the address
    // 3. Store the value of the accumulator at the calculated address
    mem[addr] = cpu.get(Register::A);  // Store the accumulator value
    // 4. Decrement cycles as needed
    // INFO: 2 cycles for fetching the absolute address
    cycles--;  // 1 cycle for adding X
    cycles--;  // 1 cycle for storing
    cycles--;  // 1 cycle
}

// STA Absolute,Y mode
void STA_ABSY(Cpu& cpu, i32& cycles, Mem& mem) {
    // 1. Fetch the absolute address (16-bit)
    word addr = cpu.fetch_word(cycles, mem);  // takes 2 cycles
    // 2. Add Y register to it
    addr += cpu.get(Register::Y);  // Add Y to the address
    // 3. Store the value of the accumulator at the calculated address
    mem[addr] = cpu.get(Register::A);  // Store the accumulator value
    // 4. Decrement cycles as needed
    cycles--;  // 1 cycle for adding Y
    cycles--;  // 1 cycle for storing
    cycles--;  // 1 final cycle
}

// STA Indirect,X mode
void STA_INX(Cpu& cpu, i32& cycles, Mem& mem) {
    // 1. Fetch zero page address
    byte zp_addr = cpu.fetch_byte(cycles, mem);  // takes 1 cycle
    // 2. Add X to it (with zero page wraparound)
    zp_addr = (zp_addr + cpu.get(Register::X)) & 0xFF;  // Zero page wraparound
    // 3. Read the effective address from the zero page (2 bytes)
    byte lo_byte = cpu.read_byte(zp_addr, cycles, mem);               // Low byte of the address
    byte hi_byte = cpu.read_byte((zp_addr + 1), cycles, mem) & 0xFF;  // High byte of the address (wraparound)
    word effective_addr = (hi_byte << 8) | lo_byte;                   // Combine to form the effective address
    // 4. Store the value of the accumulator at the effective address
    mem[effective_addr] = cpu.get(Register::A);  // Store the accumulator value
    // 5. Decrement cycles as needed
    cycles--;  // 1 cycle for adding X
    cycles--;  // 1 cycle for storing
    cycles--;  // 1 final cycle
}

// STA Indirect,Y mode
void STA_INY(Cpu& cpu, i32& cycles, Mem& mem) {
    // 1. Fetch zero page address
    byte zp_addr = cpu.fetch_byte(cycles, mem);  // takes 1 cycle
    // 2. Read the base address from the zero page (2 bytes)
    byte lo_byte = cpu.read_byte(zp_addr, cycles, mem);               // Low byte of the address
    byte hi_byte = cpu.read_byte((zp_addr + 1), cycles, mem) & 0xFF;  // High byte of the address (wraparound)
    word base_addr = (hi_byte << 8) | lo_byte;                        // Combine
    // 3. Add Y to the base address
    base_addr += cpu.get(Register::Y);  // Add Y to the base address
    // 4. Store the value of the accumulator at the calculated address
    mem[base_addr] = cpu.get(Register::A);  // Store the accumulator value
    // 5. Decrement cycles as needed
    cycles--;  // 1 cycle for fetching zero page address
    cycles--;  // 1 cycle for adding Y
    cycles--;  // 1 final cycle for storing
}

}  // namespace instructions
