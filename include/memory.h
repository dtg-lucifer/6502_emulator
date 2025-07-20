#ifndef MEMORY_H
#define MEMORY_H

#include <cstring>

#include "types.h"

class Mem {
   public:
    static constexpr u32 MAX_MEM = 1024 * 64;
    byte data[MAX_MEM];

    void init();

    // Write a 16-bit word to memory (little-endian)
    void write_word(u32& cycles, word value, u32 address);

    // Memory access operators
    byte operator[](u32 addr) const;
    byte& operator[](u32 addr);
};

#endif  // MEMORY_H
