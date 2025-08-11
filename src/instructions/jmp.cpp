#include "instructions.h"

namespace instructions {
void JMP(Cpu& cpu, i32& cycles, Mem& mem) {
    word address = cpu.fetch_word(cycles, mem);  // Fetch the address from the next two bytes

    cpu.PC = address;  // Set the Program Counter to the new address
    cycles -= 1;
}

void JMPI(Cpu& cpu, i32& cycles, Mem& mem) {
    // Fetch the address from the memory location pointed to by the next two bytes
    word indirect_address = cpu.fetch_word(cycles, mem);

    // Read the low byte and high byte from the memory at the indirect address
    byte low_byte = mem[indirect_address];

    // The 6502 has a bug where if the indirect address is at the end of a page,
    // the high byte is fetched from the beginning of the same page rather than
    // the next page. E.g., if indirect_address is $30FF, then low_byte comes from
    // $30FF but high_byte comes from $3000, not $3100.
    word high_byte_addr = (indirect_address & 0xFF00) | ((indirect_address + 1) & 0xFF);

    byte high_byte = mem[high_byte_addr];

    // Set the Program Counter to the new address
    cpu.PC = static_cast<word>(low_byte) | (static_cast<word>(high_byte) << 8);

    cycles -= 3;  // Adjust cycles for this operation
}
}  // namespace instructions
