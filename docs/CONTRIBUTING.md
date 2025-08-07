# Contributing to the 6502 CPU Emulator

Thank you for considering contributing to the 6502 CPU Emulator! This document provides guidelines and instructions for contributing to the project.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Development Process](#development-process)
- [Coding Standards](#coding-standards)
- [Testing](#testing)
- [Pull Request Process](#pull-request-process)
- [Adding New Instructions](#adding-new-instructions)
- [Reporting Bugs](#reporting-bugs)
- [Feature Requests](#feature-requests)

## Code of Conduct

This project is committed to providing a welcoming and inclusive environment for all contributors. We expect all participants to adhere to the following principles:

- Be respectful and considerate
- Be collaborative and constructive
- Focus on what is best for the community
- Show empathy towards other community members

## Getting Started

1. Fork the repository
2. Clone your fork: `git clone https://github.com/your-username/6502_cpu_emulator.git`
3. Add the upstream repository: `git remote add upstream https://github.com/original-owner/6502_cpu_emulator.git`
4. Create a branch for your changes: `git checkout -b feature/your-feature-name`

## Development Process

1. Make sure your local `main` branch is up to date with the upstream repository
2. Create a new branch for each feature or bug fix
3. Make your changes in the branch
4. Run tests to ensure they pass
5. Submit a pull request from your branch to the project's `main` branch

## Coding Standards

We follow the Google C++ Style Guide with a few modifications as defined in `.clang-format`:

- 4-space indentation
- 120 character line limit
- Left pointer alignment

Make sure to format your code before submitting a pull request:
```bash
clang-format -i src/*.cpp include/*.h
```

### Naming Conventions

- Use meaningful variable and function names
- Use `snake_case` for variables and functions
- Use `PascalCase` for class names
- Use `UPPER_CASE` for constants and macros

### Comments

- Use comments to explain *why* the code does something, not *what* it does
- Include documentation for all public functions, classes, and methods
- Use doxygen-style comments for API documentation

## Testing

Always include tests for your changes. We use a custom testing framework:

1. Add new test cases to the appropriate test file in the `tests/` directory
2. For new instructions, create a new test file following the existing pattern
3. Run tests with `make test` to ensure all tests pass

## Pull Request Process

1. Update the documentation with details of your changes
2. Add tests for your changes
3. Ensure all tests pass with `make test`
4. Submit a pull request with a clear description of the changes and why they are needed

## Adding New Instructions

To add a new 6502 instruction to the emulator:

1. Define the opcode in `include/op_codes.h`
2. Implement the instruction in a new file in `src/instructions/` (or add to an existing file if related)
3. Add the instruction handler to the switch statement in `src/cpu.cpp`
4. Write tests for the instruction in `tests/`
5. Update the documentation in `docs/OPCODES.md`

### Instruction Implementation Template

```cpp
void INSTRUCTION_ADDRESSINGMODE(Cpu& cpu, i32& cycles, Mem& mem) {
    // Fetch operands based on addressing mode
    // Perform the operation
    // Update flags as needed
    // Decrement cycles
}
```

## Reporting Bugs

When reporting bugs, please include:

- A clear description of the bug
- Steps to reproduce the issue
- Expected behavior
- Actual behavior
- Screenshots or log output if applicable
- Your operating system and compiler version

## Feature Requests

Feature requests are welcome. Please provide:

- A clear description of the feature
- Why it would be useful to the project
- If possible, a rough proposal for implementation
- Any relevant resources or background information

Thank you for contributing to the 6502 CPU emulator project!
