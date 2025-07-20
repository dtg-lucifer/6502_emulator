set debuginfod enabled on

# Automatically break at main
break main

# Define a helper to show CPU registers
define show_registers
    echo \n--- CPU Registers ---\n
    echo A (Accumulator):
    print cpu.registers[0]
    echo X (Index X):
    print cpu.registers[1]
    echo Y (Index Y):
    print cpu.registers[2]
    echo \n--- CPU Registers (Hex) ---\n
    echo A (Accumulator):
    printf "0x%02x\n", cpu.registers[0]
    echo X (Index X):
    printf "0x%02x\n", cpu.registers[1]
    echo Y (Index Y):
    printf "0x%02x\n", cpu.registers[2]
    echo \n--- Program Counter (PC) ---\n
    printf "0x%04x\n", cpu.PC
    echo \n--- Stack Pointer (SP) ---\n
    printf "0x%02x (Stack Address: 0x%04x)\n", cpu.SP, (cpu.stack_base + cpu.SP)
end

# Define a helper to show flags
define show_flags
    echo \n--- CPU Flags ---\n
    echo C (Carry):
    print cpu.C
    echo Z (Zero):
    print cpu.Z
    echo I (Interrupt Disable):
    print cpu.I
    echo D (Decimal Mode):
    print cpu.D
    echo B (Break):
    print cpu.B
    echo V (Overflow):
    print cpu.V
    echo N (Negative):
    print cpu.N
    echo U (Unused):
    print cpu.U
    echo \n--- CPU Flags (Binary) ---\n
    echo Format: 0bNVBDIZCU (Negative|Overflow|Break|Decimal|Interrupt|Zero|Carry|Unused)\n
    printf "0b%d%d%d%d%d%d%d%d (N V B D I Z C U)\n", cpu.N, cpu.V, cpu.B, cpu.D, cpu.I, cpu.Z, cpu.C, cpu.U
end

# Define a helper to show memory
define memdump
    echo \n--- Memory Dump (first 64 bytes) ---\n
    x/64xb mem.data
    echo \n--- Memory Dump (last 64 bytes) ---\n
    x/64xb mem.data + (mem.MAX_MEM - 64)
end

layout src
