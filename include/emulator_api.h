#ifndef EMULATOR_API_H
#define EMULATOR_API_H

#include <string>
#include <vector>

#include "cpu.h"
#include "cpu_state.h"
#include "memory.h"
#include "types.h"

// Main API class for external projects to use the 6502 CPU emulator
class EmulatorApi {
   private:
    Cpu cpu;
    Mem mem;
    bool initialized;

   public:
    // Constructor
    EmulatorApi();

    // Initialize the emulator
    void initialize();

    // Load a binary file into memory at the specified address
    // Returns true if loading was successful
    bool load_binary(const std::string& file_path, u32 load_address = 0x0000);

    // Execute the program currently in memory
    // If reset_first is true, calls cpu.reset() before execution
    // If reset_vector_override is non-zero, uses that as the PC instead of the reset vector
    CpuState execute_program(bool reset_first = true, word reset_vector_override = 0);

    // Get current CPU state without executing
    CpuState get_current_state() const;

    // Manually set registers (useful for testing or initialization)
    void set_register(Register reg, byte value);
    void set_pc(word value);
    void set_sp(byte value);

    // Access to memory for direct manipulation if needed
    byte read_memory(word address) const;
    void write_memory(word address, byte value);

    // Set a range of memory at once
    void set_memory_block(word start_address, const std::vector<byte>& data);

    // Utility function to set reset vector (0xFFFC-0xFFFD)
    void set_reset_vector(word address);
};

#endif  // EMULATOR_API_H
