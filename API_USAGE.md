# Using the 6502 CPU Emulator as a Library

This document explains how to use the 6502 CPU emulator as an external library in your own projects.

## Installation

### Building the Library

```bash
# Clone the repository
git clone https://github.com/yourusername/6502_cpu_emulator.git
cd 6502_cpu_emulator

# Create build directory
mkdir -p build && cd build

# Build the library
cmake ..
make

# Install the library (optional)
sudo make install
```

After building, you'll find:
- Static library: `build/libemulator_core.a`
- Shared library: `build/libemulator_core.so` (on Linux)

### Linking with Your Project

#### CMake-based Projects

Add the following to your `CMakeLists.txt`:

```cmake
# Find the 6502 CPU emulator library
find_package(emulator_core REQUIRED)

# Add your executable
add_executable(your_app main.cpp)

# Link against the emulator library
target_link_libraries(your_app PRIVATE emulator_core)
```

#### Manual Linking

```bash
g++ -o your_app main.cpp -I/path/to/6502_cpu_emulator/include -L/path/to/6502_cpu_emulator/build -lemulator_core
```

## Quick Start Guide

Here's a minimal example to get started:

```cpp
#include <iostream>
#include "emulator_api.h"

int main() {
    // Create an emulator instance
    EmulatorApi emulator;

    // Initialize the emulator
    emulator.initialize();

    // Load a binary file
    if (!emulator.load_binary("your_program.bin", 0x2000)) {
        std::cerr << "Failed to load binary file" << std::endl;
        return 1;
    }

    // Set the reset vector to point to your program
    emulator.set_reset_vector(0x2000);

    // Execute the program
    CpuState final_state = emulator.execute_program();

    // Print the results
    std::cout << "Program execution completed.\n";
    std::cout << "Final accumulator value: " << static_cast<int>(final_state.A) << std::endl;

    return 0;
}
```

## API Reference

### EmulatorApi Class

The main class for interacting with the emulator.

#### Initialization

```cpp
EmulatorApi emulator;              // Create an instance
emulator.initialize();             // Initialize memory and CPU
```

#### Loading Programs

```cpp
// Load a binary file at the specified address
bool load_binary(const std::string& file_path, u32 load_address = 0x0000);

// Set a block of memory directly
void set_memory_block(word start_address, const std::vector<byte>& data);

// Set the reset vector (0xFFFC-0xFFFD)
void set_reset_vector(word address);
```

#### Execution

```cpp
// Execute the program currently in memory
// If reset_first is true, calls cpu.reset() before execution
// If reset_vector_override is non-zero, uses that as the PC instead of the reset vector
CpuState execute_program(bool reset_first = true, word reset_vector_override = 0);
```

#### Register and Memory Access

```cpp
// Get current CPU state without executing
CpuState get_current_state() const;

// Manually set registers
void set_register(Register reg, byte value);
void set_pc(word value);
void set_sp(byte value);

// Memory access
byte read_memory(word address) const;
void write_memory(word address, byte value);
```

### CpuState Struct

Structure that holds the CPU state after execution.

```cpp
struct CpuState {
    // Register values
    byte A;   // Accumulator
    byte X;   // X index register
    byte Y;   // Y index register
    byte SP;  // Stack pointer
    word PC;  // Program counter

    // Status flags
    struct {
        byte C : 1;  // Carry Flag (bit 0)
        byte Z : 1;  // Zero Flag (bit 1)
        byte I : 1;  // Interrupt Disable Flag (bit 2)
        byte D : 1;  // Decimal Mode Flag (bit 3)
        byte B : 1;  // Break Flag (bit 4)
        byte U : 1;  // Unused/expansion (bit 5)
        byte V : 1;  // Overflow Flag (bit 6)
        byte N : 1;  // Negative Flag (bit 7)
    } flags;

    // Execution statistics
    i32 cycles_used;
    bool execution_completed;
};
```

## Creating Programs for the Emulator

### Program Structure

A valid 6502 program should:

1. Set up a reset vector at address 0xFFFC-0xFFFD to point to the program's start address
2. End with a BRK instruction (0x00) to halt execution

### Example: Creating a Simple Binary

You can use the provided `create_test_binary` utility:

```bash
./create_test_binary simple_program.bin 1
```

Or create your own binaries using any method that produces raw binary files.

### Example: Binary File Format

The binary file should contain the raw 6502 machine code bytes. For example:

```
A9 42    ; LDA #$42 - Load accumulator with value 0x42
A2 24    ; LDX #$24 - Load X register with value 0x24
A0 10    ; LDY #$10 - Load Y register with value 0x10
00       ; BRK - Break (halt execution)
```

## Complete Example

See the `examples/api_example.cpp` file for a complete working example of using the emulator API.

## Troubleshooting

### Common Issues

1. **Program doesn't execute or completes immediately**
   - Make sure you've set the reset vector correctly
   - Verify your program ends with a BRK (0x00) instruction

2. **Unexpected register values**
   - Set register values after calling `reset()` as reset clears all registers

3. **Memory access errors**
   - Ensure all memory accesses are within the valid range (0x0000 - 0xFFFF)

4. **Missing symbols when linking**
   - Make sure you've included the correct header files
   - Check that you're linking against the emulator library

### Getting Help

If you encounter issues not covered here, please file an issue on the GitHub repository.
