#include "memory.h"

void Mem::init() {
    std::memset(data, 0, MAX_MEM);  // Zero out the memory block
}

void Mem::write_word(i32& cycles, word value, u32 address) {
    data[address] = (value & 0xFF);    // Low byte first (little-endian)
    data[address + 1] = (value >> 8);  // High byte second
    cycles -= 2;
}

byte Mem::operator[](u32 addr) const {
    return data[addr];
}

byte& Mem::operator[](u32 addr) {
    return data[addr];
}
