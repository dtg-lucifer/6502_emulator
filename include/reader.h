#ifndef READER_H
#define READER_H

#include <map>
#include <string>
#include <vector>

#include "cpu.h"
#include "memory.h"
#include "types.h"

namespace binary_reader {
// This function reads the binary
// translated version of the C-64 assembly
// into the CPU memory
//
// Parameters:
// - `const std::vector<byte> data` - The byte array for the hex encoded binary
// - `const u32 m_offset` - The starting address from where this method would start
//                           writing the data into the memory
void read_from_array(Cpu& cpu, Mem& mem, const std::map<u32, std::vector<byte>> data);

// This function reaad the binary from
// pre compiled file which was translated from the C-64 assembly
//
// Parameters:
// - `const std::string& file_path` - The file path for the binary
// - `const u32 m_offset` - The starting address from where this method would start
//                           writing the data into the memory
void read_from_binary_file(Cpu& cpu, Mem& mem, const std::string& file_path, const u32 m_offset);
}  // namespace binary_reader

#endif  // READER_H
