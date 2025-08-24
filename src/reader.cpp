#include "reader.h"

#include <fstream>
#include <iostream>
#include <vector>

namespace binary_reader {
void read_from_array(Cpu& cpu, Mem& mem, const std::map<u32, std::vector<byte>> data) {
    for (const auto& [address, bytes] : data) {
        for (size_t i = 0; i < bytes.size(); ++i) {
            mem[address + i] = bytes[i];
        }
    }
}

void read_from_binary_file(Cpu& cpu, Mem& mem, const std::string& file_path, const u32 m_offset) {
    std::ifstream file(file_path, std::ios::binary);

    if (!file) {
        std::cerr << "Error: Could not open binary file: " << file_path << std::endl;
        return;
    }

    // Get file size
    file.seekg(0, std::ios::end);
    std::streampos file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    // Read the file into a buffer
    std::vector<byte> buffer(file_size);
    if (file.read(reinterpret_cast<char*>(buffer.data()), file_size)) {
        // Write the buffer to memory starting at m_offset
        for (size_t i = 0; i < buffer.size(); ++i) {
            mem[m_offset + i] = buffer[i];
        }

        std::cout << "Successfully loaded " << file_size << " bytes to memory at address 0x" << std::hex << m_offset
                  << std::dec << std::endl;
    } else {
        std::cerr << "Error: Failed to read " << file_size << " bytes from file." << std::endl;
    }
}
}  // namespace binary_reader
