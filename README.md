# 6502 CPU Emulator

A software emulation of the MOS Technology 6502 processor, a historically significant 8-bit microprocessor that powered iconic systems like the Apple II, Commodore 64, Atari 2600, and Nintendo Entertainment System (NES).

<div align="center">

![6502 CPU](https://upload.wikimedia.org/wikipedia/commons/thumb/4/49/MOS_6502AD_4585_top.jpg/320px-MOS_6502AD_4585_top.jpg)

</div>

## 📑 Documentation

This project is documented across multiple files for better organization:

- [Architecture](docs/ARCHITECTURE.md) - System architecture and component interaction
- [CPU](docs/CPU.md) - CPU implementation, registers, and flags
- [Memory](docs/MEMORY.md) - Memory organization and special regions
- [Opcodes](docs/OPCODES.md) - Instruction set and addressing modes
- [Assembler](docs/ASSEMBLER.md) - 6502 assembler usage and instruction reference
- [Demo Programs](docs/DEMO_PROGRAMS.md) - Example programs and execution
- [Testing](docs/TESTING.md) - Testing framework and utilities

## 🚀 Quick Start

### Prerequisites

- C++17 compatible compiler
- CMake 3.10 or higher
- Make

### Building the Project

1. Clone the repository:

```bash
git clone https://github.com/your-username/6502_cpu_emulator.git
cd 6502_cpu_emulator
```

2. Build the project:

```bash
make setup
make build
```

3. Run the emulator:

```bash
make run
```

### Using the Assembler

To assemble 6502 assembly files:

```bash
# Assemble a program
./build/bin/6502_assembler programs/counter.s

# Assemble with verbose output
./build/bin/6502_assembler -v programs/counter.s

# Specify output file
./build/bin/6502_assembler -o counter.bin programs/counter.s
```

See the [Assembler Documentation](docs/ASSEMBLER.md) for complete instruction reference and usage examples.

### Running Tests

```bash
make test
```

## 📊 Project Status

The emulator currently implements:

- **CPU Emulation**: Complete 6502 CPU with registers (A, X, Y, PC, SP) and status flags (C, Z, I, D, B, V, N)
- **Memory System**: Full 64KB address space with zero page and stack handling
- **Complete Instruction Set**: All standard 6502 instructions including:
    - Load/Store operations (LDA, LDX, LDY, STA, STX, STY)
    - Arithmetic operations (ADC, SBC, CMP, CPX, CPY)
    - Logical operations (AND, EOR, ORA, BIT)
    - Shift/Rotate operations (ASL, LSR, ROL, ROR)
    - Increment/Decrement operations (INC, INX, INY, DEC, DEX, DEY)
    - Branch operations (BCC, BCS, BEQ, BMI, BNE, BPL, BVC, BVS)
    - Jump and subroutine operations (JMP, JSR, RTS, BRK)
    - Stack operations (PHA, PHP, PLA, PLP, TXS, TSX)
    - Register transfer operations (TAX, TAY, TXA, TYA)
    - Status flag operations (CLC, CLD, CLI, CLV, SEC, SED, SEI)
- **6502 Assembler**: Complete assembler that converts assembly language to binary
    - All 6502 addressing modes
    - Labels and symbolic addressing
    - Assembler directives (.org, .word, .byte)
    - Multiple number formats (decimal, hex, binary)
    - Comprehensive error reporting

## 🔨 Development

The project uses CMake for building and testing:

- `make setup` - Configure the build system
- `make build` - Build the emulator and assembler
- `make run` - Run the emulator
- `make test` - Run the test suite
- `make debug` - Debug the emulator with GDB

### Available Executables

After building, you'll find these executables in `build/bin/`:

- `6502_cpu_emulator` - The main CPU emulator
- `6502_assembler` - The 6502 assembler for converting assembly to binary

## 📝 License

This project is open source and available under the [MIT License](LICENSE).

## 🤝 Contributing

Contributions are welcome! Please check the [Contributing Guide](docs/CONTRIBUTING.md) for details on how to get started.
