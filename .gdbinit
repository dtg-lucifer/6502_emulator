set debuginfod enabled on

# Automatically break at main
break main

# Define a helper to show CPU registers
define show_registers
    echo \n--- CPU Registers ---\n
    print cpu.registers[0]
    print cpu.registers[1]
    print cpu.registers[2]
end

# Define a helper to show flags
define show_flags
    echo \n--- CPU Flags ---\n
    print cpu.C
    print cpu.Z
    print cpu.I
    print cpu.D
    print cpu.B
    print cpu.V
    print cpu.N
    print cpu.U
end

# Define a helper to show memory
define memdump
    echo \n--- Memory Dump (first 16 bytes) ---\n
    x/16xb mem.data
end
