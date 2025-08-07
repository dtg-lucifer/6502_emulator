# 6502 CPU Emulator Demo Program Explanation

This document explains the demo program used in the 6502 CPU emulator and the syntax for defining memory contents in the codebase.

## Memory Map Structure Syntax

The demo program is defined using a C++ `std::map` that associates memory addresses with byte vectors:

```cpp
std::map<u32, std::vector<byte>> demo = {
    {0xFFFC, {op(Op::JSR), 0x00, 0x20}},
    {0x2000, {op(Op::LDA_IM), 0x42, /* more bytes... */}},
    // ...
};
```

This structure is used for several reasons:

1. **Memory Sparsity**: The 6502 has a 64KB address space (0x0000 to 0xFFFF), but most programs only use small portions of it. Using a map allows us to define only the used memory regions without wasting space on empty areas.

2. **Logical Organization**: This approach enables us to group related code and data at specific memory locations, making the program structure clearer.

3. **Initialization Flexibility**: We can easily place code and data at any address without requiring sequential memory layouts.

Each map entry has:
- A key representing the starting memory address (e.g., `0xFFFC`)
- A vector of bytes to place at that address

## Program Structure

The demo program consists of several memory regions:

### 1. Program Entry Point (0xFFFC)
```cpp
{0xFFFC, {op(Op::JSR), 0x00, 0x20}}
```
- This represents a `JSR` (Jump to Subroutine) instruction that jumps to address 0x2000
- 0xFFFC is the CPU reset vector on the 6502, where execution begins after reset
- The bytes stored are: opcode for JSR (0x20), followed by the address 0x2000 in little-endian format (0x00, 0x20)

### 2. Main Program (0x2000)
This section contains the main program with various instructions that demonstrate different addressing modes.

### 3. Subroutine (0x2050)
A simple subroutine that loads a value into the accumulator and returns.

### 4. Data Regions
Various memory locations that store data values to be accessed by the instructions.

## Instruction Types and Addressing Modes

The demo program showcases multiple instruction types with various addressing modes:

### LDA (Load Accumulator) Instructions
- **Immediate**: `LDA #$42` - Load the immediate value 0x42 into the accumulator
- **Zero Page**: `LDA $80` - Load the value at zero page address 0x80 into the accumulator
- **Zero Page,X**: `LDA $70,X` - Add X register to 0x70 to get the effective address, then load that value
- **Absolute**: `LDA $3000` - Load the value at absolute address 0x3000
- **Absolute,X**: `LDA $3010,X` - Add X register to 0x3010 for the effective address
- **Absolute,Y**: `LDA $3020,Y` - Add Y register to 0x3020 for the effective address
- **Indirect,X**: `LDA ($90,X)` - Add X to 0x90, use that as pointer to get target address
- **Indirect,Y**: `LDA ($92),Y` - Get address from location 0x92, then add Y to it

### LDX (Load X Register) Instructions
- **Immediate**: `LDX #$AA` - Load the immediate value 0xAA into X
- **Zero Page**: `LDX $A0` - Load the value at zero page address 0xA0 into X
- **Zero Page,Y**: `LDX $A5,Y` - Add Y register to 0xA5 to get the effective address
- **Absolute**: `LDX $3050` - Load the value at absolute address 0x3050 into X
- **Absolute,Y**: `LDX $3060,Y` - Add Y register to 0x3060 for the effective address

### LDY (Load Y Register) Instructions
- **Immediate**: `LDY #$BB` - Load the immediate value 0xBB into Y
- **Zero Page**: `LDY $B0` - Load the value at zero page address 0xB0 into Y
- **Zero Page,X**: `LDY $B5,X` - Add X register to 0xB5 to get the effective address
- **Absolute**: `LDY $3070` - Load the value at absolute address 0x3070 into Y
- **Absolute,X**: `LDY $3080,X` - Add X register to 0x3080 for the effective address

### Control Instructions
- **JSR**: Jump to Subroutine - Pushes return address to stack and jumps to target
- **RTS**: Return from Subroutine - Pulls return address from stack and continues execution
- **NOP**: No Operation - Does nothing but consumes a cycle

## Data Layout in Memory

Each instruction needs its operands to be available in the correct memory locations. For example:

1. For `LDA $80` to work, we need to place data at address 0x80:
   ```cpp
   {0x0080, {0x55}}  // Zero page data for LDA_ZP
   ```

2. For indirect addressing modes, we need both the pointers and the target data:
   ```cpp
   {0x0090, {0x00}},        // Zero page for indirect X
   {0x0095, {0x00, 0x40}},  // Indirect address pointer after X offset (0x90+X where X=5)
   {0x4000, {0xAA}},        // Target for Indirect,X
   ```
   Here, the instruction `LDA ($90,X)` with X=5 looks at address 0x95, finds pointer 0x4000, and loads the value 0xAA.

## Program Execution Flow

1. The CPU starts execution at the reset vector (0xFFFC)
2. It executes JSR to jump to the main routine at 0x2000
3. The main routine executes various load instructions to test different addressing modes
4. It then jumps to another subroutine at 0x2050
5. The subroutine loads 0xFF into the accumulator and returns
6. The main routine executes a couple of NOP instructions
7. Finally, it executes RTS to return from the subroutine
8. Program execution completes

## Binary Format vs Assembly Code

The demo code represents compiled machine code rather than assembly language. For example, the sequence:
```cpp
op(Op::LDA_IM), 0x42
```
is the binary equivalent of the assembly instruction:
```assembly
LDA #$42
```

The 6502 uses little-endian format, which means multi-byte values are stored with the least significant byte first. For example, the 16-bit address 0x2000 is stored as 0x00, 0x20 in memory.
