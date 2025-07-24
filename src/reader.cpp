#include "reader.h"

namespace binary_reader {
void read_from_array(Cpu& cpu, Mem& mem, const std::map<u32, std::vector<byte>> data) {
    for (const auto& [address, bytes] : data) {
        for (size_t i = 0; i < bytes.size(); ++i) {
            mem[address + i] = bytes[i];
        }
    }
}
}  // namespace binary_reader
