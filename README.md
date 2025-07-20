# 6502 CPU Emulator

This project is a software emulation of the MOS Technology 6502 processor, a historically significant 8-bit microprocessor that powered many iconic computers and game systems from the 1970s and 1980s, including the Apple II, Commodore 64, Atari 2600, and the Nintendo Entertainment System (NES).

## Table of Contents

- [6502 CPU Architecture](#6502-cpu-architecture)
  - [Registers](#registers)
  - [Status Flags](#status-flags)
  - [Memory Organization](#memory-organization)
  - [Stack Operations](#stack-operations)
  - [Addressing Modes](#addressing-modes)
- [Instruction Set](#instruction-set)
- [Current Implementation Status](#current-implementation-status)
- [Future Improvements](#future-improvements)

## 6502 CPU Architecture

### Registers

The 6502 has a relatively simple register set:

- **Accumulator (A)**: 8-bit register used for arithmetic and logic operations.
- **Index Register X (X)**: 8-bit register often used for indexing and offsets.
- **Index Register Y (Y)**: 8-bit register often used for indexing and offsets.
- **Program Counter (PC)**: 16-bit register that points to the next instruction to execute.
- **Stack Pointer (SP)**: 8-bit register that points to the next free location on the stack.
- **Processor Status (P)**: 8-bit register containing status flags.

### Status Flags

The 6502's processor status register contains the following flags:

- **N (Negative)**: Set if the result of the last operation has bit 7 set (is negative).
- **V (Overflow)**: Set if the last operation caused an arithmetic overflow.
- **B (Break)**: Set if a BRK instruction has been executed.
- **D (Decimal Mode)**: Set if the processor is in decimal mode.
- **I (Interrupt Disable)**: Set if interrupts are disabled.
- **Z (Zero)**: Set if the result of the last operation was zero.
- **C (Carry)**: Set if the last operation resulted in a carry or during shifts.

### Memory Organization

The 6502 has a 16-bit address bus, allowing it to address 64KB of memory (0x0000 to 0xFFFF).

Special memory regions:
- **Zero Page** (0x0000-0x00FF): First 256 bytes of memory. Special addressing modes allow for more efficient access.
- **Stack** (0x0100-0x01FF): Fixed 256-byte stack located on page 1 of memory.
- **Vectors** (0xFFFA-0xFFFF): Contains addresses for interrupt handlers and reset:
  - 0xFFFA-0xFFFB: Non-maskable interrupt vector
  - 0xFFFC-0xFFFD: Reset vector (processor starts execution from this address on reset)
  - 0xFFFE-0xFFFF: Interrupt request vector

### Stack Operations

The 6502 stack operates as follows:

- The stack is always located in page 1 of memory (0x0100-0x01FF).
- The Stack Pointer (SP) is an 8-bit register that holds only the low byte of the stack address.
- The stack grows downward from 0x01FF to 0x0100.
- On reset, SP is initialized to 0xFF (pointing to stack address 0x01FF).
- When pushing a byte to the stack:
  1. The byte is stored at the current stack address (0x0100 + SP)
  2. SP is decremented

- When pulling a byte from the stack:
  1. SP is incremented
  2. The byte is read from the current stack address (0x0100 + SP)

- For 16-bit values (like return addresses):
  - The high byte is pushed first, then the low byte
  - The low byte is pulled first, then the high byte

### Addressing Modes

The 6502 supports several addressing modes:

1. **Implied/Implicit**: The instruction itself implies the operand (e.g., `TAX` - Transfer A to X).
2. **Accumulator**: The operation is performed on the accumulator (e.g., `ASL A` - Shift A left).
3. **Immediate**: The operand is the byte following the instruction (e.g., `LDA #$42`).
4. **Zero Page**: The operand is in the zero page, specified by a single byte (e.g., `LDA $42`).
5. **Zero Page,X**: Zero page address is offset by X register (e.g., `LDA $42,X`).
6. **Zero Page,Y**: Zero page address is offset by Y register (e.g., `LDX $42,Y`).
7. **Absolute**: The operand's full 16-bit address follows the instruction (e.g., `LDA $4242`).
8. **Absolute,X**: Absolute address is offset by X register (e.g., `LDA $4242,X`).
9. **Absolute,Y**: Absolute address is offset by Y register (e.g., `LDA $4242,Y`).
10. **Indirect**: The instruction specifies a memory location containing the actual 16-bit address (e.g., `JMP ($4242)`).
11. **Indexed Indirect (X)**: The operand is a zero-page address that is offset by X, which contains the effective address (e.g., `LDA ($42,X)`).
12. **Indirect Indexed (Y)**: The operand is a zero-page address containing a 16-bit address, which is then offset by Y (e.g., `LDA ($42),Y`).
13. **Relative**: Used for branches, where a signed offset is added to the program counter (e.g., `BNE $42`).

## Instruction Set

The 6502 has 56 official instructions. Here are some of the most common ones currently implemented in this emulator:

- **LDA**: Load Accumulator
  - LDA_IM: Load immediate value into A
  - LDA_ZP: Load value from zero page into A
  - LDA_ZPX: Load value from zero page with X offset into A

- **JSR**: Jump to Subroutine
  - Pushes the return address (PC-1) onto the stack and jumps to the target address

## Current Implementation Status

The emulator currently implements:

- Basic CPU structure with registers (A, X, Y, PC, SP)
- Status flags (C, Z, I, D, B, V, N)
- Memory access with a 64KB address space
- A limited set of instructions:
  - LDA (immediate, zero page, and zero page,X)
  - JSR (jump to subroutine)
- Basic execution cycle

### Memory Implementation

The memory is implemented as a simple array of bytes with methods for reading and writing bytes and words:

- `init()`: Initializes memory by clearing it
- `write_word()`: Writes a 16-bit word to memory (little-endian)
- Operator overloads for byte-level access

### CPU Implementation

The CPU implementation includes:

- Register initialization
- Flag handling
- Instruction fetching and execution
- Stack operations

## Future Improvements

Future versions of this emulator will include:

1. Complete instruction set implementation
2. Return from subroutine (RTS) and other stack operations
3. Interrupt handling
4. More addressing modes
5. Cycle-accurate timing
6. Debugging tools and memory inspection
7. Integration with I/O devices to simulate complete systems
