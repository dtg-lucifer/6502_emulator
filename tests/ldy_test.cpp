#include "cpu.h"
#include "memory.h"
#include "op_codes.h"
#include "test_utils.h"

using namespace colors;

namespace testing {
void inline_ldy_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Inline test for LDY immediate instruction
    mem[0xFFFC] = op(Op::LDY_IM);  // Load immediate
    mem[0xFFFD] = 0xF0;            // Value to load into Y register

    bool program_completed = false;
    i32 cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Print cycles used and completion status

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Y Register after LDY_IM: 0x%02X%s\n", CYAN, cpu.get(Register::Y), RESET);

    // Verify with test assertion
    if (cpu.get(Register::Y) != 0xF0) {
        throw testing::TestFailedException("LDY_IM test failed: Y register should be 0xF0");
    }

    // Verify flags are correctly set
    if (!cpu.N) {
        throw testing::TestFailedException("LDY_IM test failed: Negative flag should be set");
    }
    if (cpu.Z) {
        throw testing::TestFailedException("LDY_IM test failed: Zero flag should not be set");
    }

    // Test with a zero value to verify Z flag
    cpu.reset(mem);
    mem[0xFFFC] = op(Op::LDY_IM);  // Load immediate
    mem[0xFFFD] = 0x00;            // Value to load into Y register

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify flags for zero value
    if (!cpu.Z) {
        throw testing::TestFailedException("LDY_IM test failed: Zero flag should be set");
    }
    if (cpu.N) {
        throw testing::TestFailedException("LDY_IM test failed: Negative flag should not be set");
    }
}

void inline_ldy_zp_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Inline test for LDY Zero Page instruction
    mem[0xFFFC] = op(Op::LDY_ZP);  // Load from Zero Page
    mem[0xFFFD] = 0x42;            // Zero page address
    mem[0x0042] = 0x37;            // Value at the zero page address

    bool program_completed = false;
    i32 cycles_used = cpu.execute(3, mem, &program_completed, true);

    // Print cycles used and completion status

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Y Register after LDY_ZP: 0x%02X%s\n", CYAN, cpu.get(Register::Y), RESET);

    // Verify with test assertion
    if (cpu.get(Register::Y) != 0x37) {
        throw testing::TestFailedException("LDY_ZP test failed: Y register should be 0x37");
    }

    // Verify flags are correctly set
    if (cpu.N) {
        throw testing::TestFailedException("LDY_ZP test failed: Negative flag should not be set");
    }
    if (cpu.Z) {
        throw testing::TestFailedException("LDY_ZP test failed: Zero flag should not be set");
    }
}

void inline_ldy_zpx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Inline test for LDY Zero Page,X instruction
    mem[0xFFFC] = op(Op::LDY_ZPX);  // Load from Zero Page,X
    mem[0xFFFD] = 0x42;             // Zero page address
    mem[0x0047] = 0x38;             // Value at the effective address (0x42 + X)
    mem[0xFFFE] = 0x00;             // Add termination marker (BRK/NULL)

    // Set X register value
    cpu.set(Register::X, 0x05);

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Print cycles used and completion status

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Y Register after LDY_ZPX: 0x%02X%s\n", CYAN, cpu.get(Register::Y), RESET);

    // Verify with test assertion
    if (cpu.get(Register::Y) != 0x38) {
        throw testing::TestFailedException("LDY_ZPX test failed: Y register should be 0x38");
    }

    // Verify flags are correctly set
    if (cpu.N) {
        throw testing::TestFailedException("LDY_ZPX test failed: Negative flag should not be set");
    }
    if (cpu.Z) {
        throw testing::TestFailedException("LDY_ZPX test failed: Zero flag should not be set");
    }
}

void inline_ldy_zpx_wrap_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Inline test for LDY Zero Page,X with wrap-around
    mem[0xFFFC] = op(Op::LDY_ZPX);  // Load from Zero Page,X with wrap
    mem[0xFFFD] = 0xFF;             // Zero page address
    mem[0x0004] = 0x39;             // Value at the wrapped address (0xFF + 0x05 = 0x104, truncated to 0x04)
    mem[0xFFFE] = 0x00;             // Add termination marker (BRK/NULL)

    // Set X register value
    cpu.set(Register::X, 0x05);

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Print cycles used and completion status

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Y Register after LDY_ZPX (With address wrapping): 0x%02X%s\n", CYAN, cpu.get(Register::Y), RESET);

    // Verify with test assertion
    if (cpu.get(Register::Y) != 0x39) {
        throw testing::TestFailedException("LDY_ZPX wrap test failed: Y register should be 0x39");
    }
}

void inline_ldy_absolute_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Inline test for LDY Absolute instruction
    mem[0xFFFC] = op(Op::LDY_AB);  // Load from Absolute
    mem[0xFFFD] = 0x80;            // Low byte of address
    mem[0xFFFE] = 0x44;            // High byte of address (0x4480)
    mem[0x4480] = 0x3A;            // Value at the absolute address

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Print cycles used and completion status

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Y Register after LDY_AB: 0x%02X%s\n", CYAN, cpu.get(Register::Y), RESET);

    // Verify with test assertion
    if (cpu.get(Register::Y) != 0x3A) {
        throw testing::TestFailedException("LDY_AB test failed: Y register should be 0x3A");
    }

    // Verify flags are correctly set
    if (cpu.N) {
        throw testing::TestFailedException("LDY_AB test failed: Negative flag should not be set");
    }
    if (cpu.Z) {
        throw testing::TestFailedException("LDY_AB test failed: Zero flag should not be set");
    }
}

void inline_ldy_absx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Inline test for LDY Absolute,X instruction
    mem[0xFFFC] = op(Op::LDY_ABSX);  // Load from Absolute,X
    mem[0xFFFD] = 0x80;              // Low byte of address
    mem[0xFFFE] = 0x44;              // High byte of address (0x4480)

    // Set value at the target address (0x4480 + X)
    mem[0x4485] = 0x3B;  // Value at the effective address (0x4480 + X)

    // Set X register value
    cpu.set(Register::X, 0x05);

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Print cycles used and completion status

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Y Register after LDY_ABSX: 0x%02X%s\n", CYAN, cpu.get(Register::Y), RESET);

    // Verify with test assertion
    if (cpu.get(Register::Y) != 0x3B) {
        throw testing::TestFailedException("LDY_ABSX test failed: Y register should be 0x3B");
    }

    // Verify flags are correctly set
    if (cpu.N) {
        throw testing::TestFailedException("LDY_ABSX test failed: Negative flag should not be set");
    }
    if (cpu.Z) {
        throw testing::TestFailedException("LDY_ABSX test failed: Zero flag should not be set");
    }

    // Test with page crossing (additional cycle)
    cpu.reset(mem);
    mem[0xFFFC] = op(Op::LDY_ABSX);  // Load from Absolute,X
    mem[0xFFFD] = 0xFF;              // Low byte of address
    mem[0xFFFE] = 0x44;              // High byte of address (0x44FF)

    // Value at the target address with page crossing (0x44FF + 0x05 = 0x4504)
    mem[0x4504] = 0x80;  // Negative value to test N flag

    // Set X register value
    cpu.set(Register::X, 0x05);

    program_completed = false;
    cycles_used = cpu.execute(5, mem, &program_completed, true);  // 5 cycles with page crossing

    // Verify with test assertion
    if (cpu.get(Register::Y) != 0x80) {
        throw testing::TestFailedException("LDY_ABSX page crossing test failed: Y register should be 0x80");
    }

    // Verify N flag is set for negative value
    if (!cpu.N) {
        throw testing::TestFailedException("LDY_ABSX test failed: Negative flag should be set for value 0x80");
    }
    if (cpu.Z) {
        throw testing::TestFailedException("LDY_ABSX test failed: Zero flag should not be set for value 0x80");
    }
}
}  // namespace testing
