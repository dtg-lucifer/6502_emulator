# 6502 Assembler

The 6502 Assembler is a complete assembler for the MOS Technology 6502 processor that converts assembly language source code into binary machine code that can be executed by the 6502 CPU emulator.

## Features

- **Complete 6502 Instruction Set**: Supports all 151 opcodes defined in the emulator's `op_codes.h` file, including load/store, arithmetic, logical, branch, and control instructions
- **All Addressing Modes**: Supports all 6502 addressing modes including immediate, zero page, absolute, indexed, and indirect addressing
- **Labels and Symbols**: Full support for labels and symbolic addressing
- **Assembler Directives**: Support for `.org`, `.word`, and `.byte` directives
- **Multiple Number Formats**: Supports decimal, hexadecimal ($), and binary (%) number formats
- **Error Reporting**: Comprehensive error reporting with line numbers and context
- **Memory Map Output**: Generates detailed memory maps and disassembly listings

## Quick Start

### Building the Assembler

The assembler is built as part of the main project:

```bash
make setup
make build
```

This creates the `6502_assembler` executable in the `build/bin/` directory.

### Basic Usage

Assemble a simple program:

```bash
./build/bin/6502_assembler program.s
```

Specify output file:

```bash
./build/bin/6502_assembler -o program.bin program.s
```

Verbose output with debugging information:

```bash
./build/bin/6502_assembler -v -d program.s
```

## Assembly Language Syntax

### Comments

Comments begin with a semicolon (`;`) and continue to the end of the line:

```assembly
; This is a comment
LDA #$FF  ; Load accumulator with $FF
```

### Labels

Labels are identifiers followed by a colon (`:`) and mark memory locations:

```assembly
start:
    LDA #$00
    STA $0200

loop:
    INX
    JMP loop
```

### Numbers

The assembler supports multiple number formats:

- **Decimal**: `123`, `255`
- **Hexadecimal**: `$FF`, `$1234` (prefixed with `$`)
- **Binary**: `%11111111`, `%10101010` (prefixed with `%`)

### Instructions

Instructions follow the standard 6502 assembly syntax:

```assembly
MNEMONIC [operand]
```

Examples:

```assembly
NOP           ; No operand (implied addressing)
LDA #$42      ; Immediate addressing
LDA $80       ; Zero page addressing
LDA $1000     ; Absolute addressing
LDA $80,X     ; Zero page indexed with X
LDA $1000,X   ; Absolute indexed with X
LDA ($80,X)   ; Indirect indexed with X
LDA ($80),Y   ; Indirect indexed with Y
```

## Supported Instructions

### Load/Store Operations

All load/store operations exactly match the opcodes defined in `op_codes.h`:

| Instruction | Description       | Addressing Modes                                                                                | Opcodes                                        |
| ----------- | ----------------- | ----------------------------------------------------------------------------------------------- | ---------------------------------------------- |
| `LDA`       | Load Accumulator  | Immediate, Zero Page, Zero Page,X, Absolute, Absolute,X, Absolute,Y, (Indirect,X), (Indirect),Y | 0xA9, 0xA5, 0xB5, 0xAD, 0xBD, 0xB9, 0xA1, 0xB1 |
| `LDX`       | Load X Register   | Immediate, Zero Page, Zero Page,Y, Absolute, Absolute,Y                                         | 0xA2, 0xA6, 0xB6, 0xAE, 0xBE                   |
| `LDY`       | Load Y Register   | Immediate, Zero Page, Zero Page,X, Absolute, Absolute,X                                         | 0xA0, 0xA4, 0xB4, 0xAC, 0xBC                   |
| `STA`       | Store Accumulator | Zero Page, Zero Page,X, Absolute, Absolute,X, Absolute,Y, (Indirect,X), (Indirect),Y            | 0x85, 0x95, 0x8D, 0x9D, 0x99, 0x81, 0x91       |
| `STX`       | Store X Register  | Zero Page, Zero Page,Y, Absolute                                                                | 0x86, 0x96, 0x8E                               |
| `STY`       | Store Y Register  | Zero Page, Zero Page,X, Absolute                                                                | 0x84, 0x94, 0x8C                               |

### Register Transfer Operations

| Instruction | Description                 |
| ----------- | --------------------------- |
| `TAX`       | Transfer Accumulator to X   |
| `TAY`       | Transfer Accumulator to Y   |
| `TXA`       | Transfer X to Accumulator   |
| `TYA`       | Transfer Y to Accumulator   |
| `TSX`       | Transfer Stack Pointer to X |
| `TXS`       | Transfer X to Stack Pointer |

### Stack Operations

| Instruction | Description                      |
| ----------- | -------------------------------- |
| `PHA`       | Push Accumulator on Stack        |
| `PHP`       | Push Processor Status on Stack   |
| `PLA`       | Pull Accumulator from Stack      |
| `PLP`       | Pull Processor Status from Stack |

### Logical Operations

| Instruction | Description                   | Addressing Modes                                                                                |
| ----------- | ----------------------------- | ----------------------------------------------------------------------------------------------- |
| `AND`       | Logical AND with Accumulator  | Immediate, Zero Page, Zero Page,X, Absolute, Absolute,X, Absolute,Y, (Indirect,X), (Indirect),Y |
| `EOR`       | Exclusive OR with Accumulator | Immediate, Zero Page, Zero Page,X, Absolute, Absolute,X, Absolute,Y, (Indirect,X), (Indirect),Y |
| `ORA`       | Logical OR with Accumulator   | Immediate, Zero Page, Zero Page,X, Absolute, Absolute,X, Absolute,Y, (Indirect,X), (Indirect),Y |
| `BIT`       | Test Bits in Memory           | Zero Page, Absolute                                                                             |

### Arithmetic Operations

| Instruction | Description         | Addressing Modes                                                                                |
| ----------- | ------------------- | ----------------------------------------------------------------------------------------------- |
| `ADC`       | Add with Carry      | Immediate, Zero Page, Zero Page,X, Absolute, Absolute,X, Absolute,Y, (Indirect,X), (Indirect),Y |
| `SBC`       | Subtract with Carry | Immediate, Zero Page, Zero Page,X, Absolute, Absolute,X, Absolute,Y, (Indirect,X), (Indirect),Y |
| `CMP`       | Compare Accumulator | Immediate, Zero Page, Zero Page,X, Absolute, Absolute,X, Absolute,Y, (Indirect,X), (Indirect),Y |
| `CPX`       | Compare X Register  | Immediate, Zero Page, Absolute                                                                  |
| `CPY`       | Compare Y Register  | Immediate, Zero Page, Absolute                                                                  |

### Increment/Decrement Operations

| Instruction | Description          | Addressing Modes                             |
| ----------- | -------------------- | -------------------------------------------- |
| `INC`       | Increment Memory     | Zero Page, Zero Page,X, Absolute, Absolute,X |
| `INX`       | Increment X Register | Implied                                      |
| `INY`       | Increment Y Register | Implied                                      |
| `DEC`       | Decrement Memory     | Zero Page, Zero Page,X, Absolute, Absolute,X |
| `DEX`       | Decrement X Register | Implied                                      |
| `DEY`       | Decrement Y Register | Implied                                      |

### Shift and Rotate Operations

| Instruction | Description           | Addressing Modes                                          |
| ----------- | --------------------- | --------------------------------------------------------- |
| `ASL`       | Arithmetic Shift Left | Accumulator, Zero Page, Zero Page,X, Absolute, Absolute,X |
| `LSR`       | Logical Shift Right   | Accumulator, Zero Page, Zero Page,X, Absolute, Absolute,X |
| `ROL`       | Rotate Left           | Accumulator, Zero Page, Zero Page,X, Absolute, Absolute,X |
| `ROR`       | Rotate Right          | Accumulator, Zero Page, Zero Page,X, Absolute, Absolute,X |

### Jump and Branch Operations

| Instruction | Description            | Addressing Modes   |
| ----------- | ---------------------- | ------------------ |
| `JMP`       | Jump                   | Absolute, Indirect |
| `JSR`       | Jump to Subroutine     | Absolute           |
| `RTS`       | Return from Subroutine | Implied            |
| `BRK`       | Force Interrupt        | Implied            |

### Branch Instructions

| Instruction | Description                      |
| ----------- | -------------------------------- |
| `BCC`       | Branch if Carry Clear            |
| `BCS`       | Branch if Carry Set              |
| `BEQ`       | Branch if Equal (Zero Set)       |
| `BMI`       | Branch if Minus (Negative Set)   |
| `BNE`       | Branch if Not Equal (Zero Clear) |
| `BPL`       | Branch if Plus (Negative Clear)  |
| `BVC`       | Branch if Overflow Clear         |
| `BVS`       | Branch if Overflow Set           |

### Status Flag Operations

| Instruction | Description                  |
| ----------- | ---------------------------- |
| `CLC`       | Clear Carry Flag             |
| `CLD`       | Clear Decimal Mode Flag      |
| `CLI`       | Clear Interrupt Disable Flag |
| `CLV`       | Clear Overflow Flag          |
| `SEC`       | Set Carry Flag               |
| `SED`       | Set Decimal Mode Flag        |
| `SEI`       | Set Interrupt Disable Flag   |

### No Operation

| Instruction | Description  |
| ----------- | ------------ |
| `NOP`       | No Operation |

## Addressing Modes

### Immediate (`#value`)

The operand is a constant value:

```assembly
LDA #$42    ; Load accumulator with the value $42
```

### Zero Page (`$00-$FF`)

The operand is an address in the zero page (first 256 bytes):

```assembly
LDA $80     ; Load accumulator from address $0080
```

### Zero Page,X (`$00-$FF,X`)

Zero page address plus X register:

```assembly
LDA $80,X   ; Load accumulator from address $0080 + X
```

### Zero Page,Y (`$00-$FF,Y`)

Zero page address plus Y register:

```assembly
LDX $80,Y   ; Load X register from address $0080 + Y
```

### Absolute (`$0000-$FFFF`)

The operand is a 16-bit address:

```assembly
LDA $1234   ; Load accumulator from address $1234
```

### Absolute,X (`$0000-$FFFF,X`)

Absolute address plus X register:

```assembly
LDA $1234,X ; Load accumulator from address $1234 + X
```

### Absolute,Y (`$0000-$FFFF,Y`)

Absolute address plus Y register:

```assembly
LDA $1234,Y ; Load accumulator from address $1234 + Y
```

### Indirect (`($0000-$FFFF)`)

The operand is the address of a pointer to the target address:

```assembly
JMP ($1234) ; Jump to address stored at $1234 and $1235
```

### Indirect,X (`($00-$FF,X)`)

Zero page address plus X, then indirect:

```assembly
LDA ($80,X) ; Load from address pointed to by ($0080 + X)
```

### Indirect,Y (`($00-$FF),Y`)

Indirect from zero page address, then add Y:

```assembly
LDA ($80),Y ; Load from (address pointed to by $0080) + Y
```

### Relative (for branches)

Signed 8-bit offset from current PC:

```assembly
BEQ loop    ; Branch to 'loop' label if zero flag is set
```

## Assembler Directives

### `.org` - Set Origin Address

Sets the current assembly address:

```assembly
.org $8000  ; Set assembly address to $8000
start:
    LDA #$00
```

### `.word` - Define 16-bit Word

Defines a 16-bit word in memory:

```assembly
.org $FFFC
.word start ; Reset vector points to 'start' label
```

### `.byte` - Define 8-bit Byte

Defines an 8-bit byte in memory:

```assembly
data:
.byte $42   ; Store byte $42 at 'data' label
```

## Example Programs

### Simple Counter

This program increments the X register and stores it to memory location $0200:

```assembly
.org $8000
start:
    LDX #$00        ; Initialize X register to 0
    STX $0200       ; Store X to memory location $0200

loop:
    INX             ; Increment X register
    STX $0200       ; Store X to memory location $0200
    JMP loop        ; Jump back to loop

.org $FFFC
    .word start     ; Reset vector points to start
```

### Hello World Data

This program sets up some data bytes:

```assembly
.org $1000
message:
    .byte $48       ; 'H'
    .byte $65       ; 'e'
    .byte $6C       ; 'l'
    .byte $6C       ; 'l'
    .byte $6F       ; 'o'
    .byte $00       ; Null terminator

.org $8000
start:
    LDY #$00        ; Initialize Y register
    LDA message,Y   ; Load first character
    ; ... more code
```

### Subroutine Example

This program demonstrates subroutine calls:

```assembly
.org $8000
start:
    LDA #$42        ; Load accumulator with $42
    JSR multiply    ; Call multiply subroutine
    STA result      ; Store result
    BRK             ; Break

multiply:
    ASL A           ; Shift left (multiply by 2)
    RTS             ; Return from subroutine

.org $1000
result:
    .byte $00       ; Storage for result

.org $FFFC
    .word start     ; Reset vector
```

## Command Line Options

### Basic Usage

```bash
6502_assembler [options] <source_file>
```

### Options

| Option       | Description                                             |
| ------------ | ------------------------------------------------------- |
| `-o <file>`  | Specify output file (default: `<source>.bin`)           |
| `-v`         | Verbose output - shows assembly progress and statistics |
| `-d`         | Debug output - shows detailed internal information      |
| `-h, --help` | Show help message                                       |
| `--version`  | Show version information                                |
| `--symbols`  | Print symbol table after assembly                       |
| `--memory`   | Print memory map after assembly                         |
| `--ir`       | Print intermediate representation                       |

### Examples

Basic assembly:

```bash
6502_assembler counter.s
```

Verbose assembly with custom output:

```bash
6502_assembler -v -o counter.bin counter.s
```

Debug assembly with symbol table:

```bash
6502_assembler -d --symbols counter.s
```

Show memory map:

```bash
6502_assembler --memory counter.s
```

## Error Handling

The assembler provides comprehensive error reporting with line numbers and context:

### Syntax Errors

```
Parse error at line 5, column 10: Expected instruction mnemonic
Line 5:     XYZ #$42
                ^
```

### Undefined Labels

```
Undefined label: unknown_label at line 8
```

### Invalid Addressing Modes

```
Invalid addressing mode for instruction LDX at line 3
```

### Address Range Errors

```
Zero page address out of range at line 6
```

## Output Files

### Binary Output (`.bin`)

The assembler generates raw binary files that can be loaded directly into the 6502 emulator or burned to ROM.

### Memory Layout

The binary file contains the assembled machine code starting from the lowest address used in the program. Gaps in the address space are filled with zeros.

## Integration with Emulator

The assembled binary files can be used directly with the 6502 CPU emulator:

```cpp
#include "reader.h"
#include "cpu.h"
#include "memory.h"

// Load assembled program
auto program_data = reader::read_binary_file("program.bin");
// ... load into emulator memory and execute
```

## Opcode Compatibility

The assembler provides 100% compatibility with the 6502 CPU emulator's instruction set. All 151 opcodes defined in `op_codes.h` are supported with their exact opcode values:

- **Verified Coverage**: Every opcode from `BRK` (0x00) to `NOP` (0xEA) is implemented
- **Exact Mapping**: Assembled instructions produce the same opcodes as the emulator expects
- **Full Addressing Mode Support**: All addressing mode variants are correctly encoded
- **Binary Compatibility**: Assembled programs can be directly loaded into the emulator

## Tips and Best Practices

1. **Use Labels**: Always use meaningful labels instead of hard-coded addresses
2. **Comment Your Code**: Use comments to explain complex logic
3. **Organize with .org**: Use `.org` directives to organize code in logical sections
4. **Test Incrementally**: Assemble and test small sections of code before combining
5. **Check Address Ranges**: Be mindful of zero page vs. absolute addressing modes
6. **Use Proper Data Types**: Use `.byte` for 8-bit values and `.word` for 16-bit addresses
7. **Verify Opcodes**: Use `--memory` flag to verify that assembled opcodes match expectations

## Troubleshooting

### Common Issues

**"Unknown instruction" errors**: Check that the instruction mnemonic is spelled correctly and is a valid 6502 instruction.

**"Invalid addressing mode" errors**: Verify that the addressing mode is supported for the specific instruction.

**"Undefined label" errors**: Ensure all referenced labels are defined in the source code.

**Binary file not generated**: Check for assembly errors in the output. The binary is only generated if assembly succeeds.

### Getting Help

Use the verbose (`-v`) and debug (`-d`) flags to get more information about the assembly process:

```bash
6502_assembler -v -d --symbols --memory program.s
```

This will show:

- Assembly progress
- Symbol table
- Memory map
- Any errors or warnings
