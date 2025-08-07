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

### Running Tests

```bash
make test
```

## 📊 Project Status

The emulator currently implements:

- Basic CPU structure with registers (A, X, Y, PC, SP)
- Status flags (C, Z, I, D, B, V, N)
- Memory access with a 64KB address space
- Instruction implementations:
    - Load/Store operations (LDA, LDX, LDY, STA, STX, STY)
    - Subroutine handling (JSR, RTS)
    - No Operation (NOP)

## 🔨 Development

The project uses CMake for building and testing:

- `make setup` - Configure the build system
- `make build` - Build the emulator
- `make run` - Run the emulator
- `make test` - Run the test suite
- `make debug` - Debug the emulator with GDB

## 📝 License

This project is open source and available under the [MIT License](LICENSE).

## 🤝 Contributing

Contributions are welcome! Please check the [Contributing Guide](docs/CONTRIBUTING.md) for details on how to get started.
