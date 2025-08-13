#ifndef TYPES_H
#define TYPES_H

#include <cstdint>

// Basic types used throughout the emulator
using byte = uint8_t;
using word = uint16_t;
using pinl_t = uint32_t;

using u32 = unsigned int;
using i32 = int;

u32 as_u32(int num);
i32 as_i32(int num);

#endif  // TYPES_H
