#include "cpu.h"
#include "memory.h"
#include "op_codes.h"
#include "test_utils.h"

using namespace colors;

namespace testing {
void inline_ldx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Inline test for LDX immediate instruction
    mem[0xFFFC] = op(Op::LDX_IM);  // Load immediate
    mem[0xFFFD] = 0xF0;            // Value to load into X register

    bool program_completed = false;
    i32 cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> X Register after LDX_IM: 0x%02X%s\n", CYAN, cpu.get(Register::X), RESET);

    // Verify with test assertion
    if (cpu.get(Register::X) != 0xF0) {
        throw testing::TestFailedException("LDX_IM test failed: X register should be 0xF0");
    }

    // Verify flags are correctly set
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("LDX_IM test failed: Negative flag should be set");
    }
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("LDX_IM test failed: Zero flag should not be set");
    }

    // Test with a zero value to verify Z flag
    cpu.reset(mem);
    mem[0xFFFC] = op(Op::LDX_IM);  // Load immediate
    mem[0xFFFD] = 0x00;            // Value to load into X register

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify flags for zero value
    if (!cpu.FLAGS_Z) {
        throw testing::TestFailedException("LDX_IM test failed: Zero flag should be set");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("LDX_IM test failed: Negative flag should not be set");
    }
}

void inline_ldx_zp_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Inline test for LDX Zero Page instruction
    mem[0xFFFC] = op(Op::LDX_ZP);  // Load from Zero Page
    mem[0xFFFD] = 0x42;            // Zero page address
    mem[0x0042] = 0x37;            // Value at the zero page address

    bool program_completed = false;
    i32 cycles_used = cpu.execute(3, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> X Register after LDX_ZP: 0x%02X%s\n", CYAN, cpu.get(Register::X), RESET);

    // Verify with test assertion
    if (cpu.get(Register::X) != 0x37) {
        throw testing::TestFailedException("LDX_ZP test failed: X register should be 0x37");
    }

    // Verify flags are correctly set
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("LDX_ZP test failed: Negative flag should not be set");
    }
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("LDX_ZP test failed: Zero flag should not be set");
    }
}

void inline_ldx_zpy_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Inline test for LDX Zero Page,Y instruction
    mem[0xFFFC] = op(Op::LDX_ZPY);  // Load from Zero Page,Y
    mem[0xFFFD] = 0x42;             // Zero page address
    mem[0x0048] = 0x38;             // Value at the effective address (0x42 + Y)
    mem[0xFFFE] = 0x00;             // Add termination marker (BRK/NULL)

    // Set Y register value
    cpu.set(Register::Y, 0x06);

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> X Register after LDX_ZPY: 0x%02X%s\n", CYAN, cpu.get(Register::X), RESET);

    // Verify with test assertion
    if (cpu.get(Register::X) != 0x38) {
        throw testing::TestFailedException("LDX_ZPY test failed: X register should be 0x38");
    }

    // Verify flags are correctly set
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("LDX_ZPY test failed: Negative flag should not be set");
    }
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("LDX_ZPY test failed: Zero flag should not be set");
    }
}

void inline_ldx_zpy_wrap_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Inline test for LDX Zero Page,Y with wrap-around
    mem[0xFFFC] = op(Op::LDX_ZPY);  // Load from Zero Page,Y with wrap
    mem[0xFFFD] = 0xFF;             // Zero page address
    mem[0x0005] = 0x39;             // Value at the wrapped address (0xFF + 0x06 = 0x105, truncated to 0x05)
    mem[0xFFFE] = 0x00;             // Add termination marker (BRK/NULL)

    // Set Y register value
    cpu.set(Register::Y, 0x06);

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> X Register after LDX_ZPY (With address wrapping): 0x%02X%s\n", CYAN, cpu.get(Register::X), RESET);

    // Verify with test assertion
    if (cpu.get(Register::X) != 0x39) {
        throw testing::TestFailedException("LDX_ZPY wrap test failed: X register should be 0x39");
    }
}

void inline_ldx_absolute_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Inline test for LDX Absolute instruction
    mem[0xFFFC] = op(Op::LDX_AB);  // Load from Absolute
    mem[0xFFFD] = 0x80;            // Low byte of address
    mem[0xFFFE] = 0x44;            // High byte of address (0x4480)
    mem[0x4480] = 0x3A;            // Value at the absolute address

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Print cycles used and completion status

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> X Register after LDX_AB: 0x%02X%s\n", CYAN, cpu.get(Register::X), RESET);

    // Verify with test assertion
    if (cpu.get(Register::X) != 0x3A) {
        throw testing::TestFailedException("LDX_AB test failed: X register should be 0x3A");
    }

    // Verify flags are correctly set
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("LDX_AB test failed: Negative flag should not be set");
    }
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("LDX_AB test failed: Zero flag should not be set");
    }
}

void inline_ldx_absy_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Inline test for LDX Absolute,Y instruction
    mem[0xFFFC] = op(Op::LDX_ABSY);  // Load from Absolute,Y
    mem[0xFFFD] = 0x80;              // Low byte of address
    mem[0xFFFE] = 0x44;              // High byte of address (0x4480)

    // Set value at the target address (0x4480 + Y)
    mem[0x4486] = 0x3B;  // Value at the effective address (0x4480 + Y)

    // Set Y register value
    cpu.set(Register::Y, 0x06);

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Print cycles used and completion status

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> X Register after LDX_ABSY: 0x%02X%s\n", CYAN, cpu.get(Register::X), RESET);

    // Verify with test assertion
    if (cpu.get(Register::X) != 0x3B) {
        throw testing::TestFailedException("LDX_ABSY test failed: X register should be 0x3B");
    }

    // Verify flags are correctly set
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("LDX_ABSY test failed: Negative flag should not be set");
    }
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("LDX_ABSY test failed: Zero flag should not be set");
    }

    // Test with page crossing (additional cycle)
    cpu.reset(mem);
    mem[0xFFFC] = op(Op::LDX_ABSY);  // Load from Absolute,Y
    mem[0xFFFD] = 0xFF;              // Low byte of address
    mem[0xFFFE] = 0x44;              // High byte of address (0x44FF)

    // Value at the target address with page crossing (0x44FF + 0x06 = 0x4505)
    mem[0x4505] = 0x80;  // Negative value to test N flag

    // Set Y register value
    cpu.set(Register::Y, 0x06);

    program_completed = false;
    cycles_used = cpu.execute(5, mem, &program_completed, true);  // 5 cycles with page crossing

    // Verify with test assertion
    if (cpu.get(Register::X) != 0x80) {
        throw testing::TestFailedException("LDX_ABSY page crossing test failed: X register should be 0x80");
    }

    // Verify N flag is set for negative value
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("LDX_ABSY test failed: Negative flag should be set for value 0x80");
    }
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("LDX_ABSY test failed: Zero flag should not be set for value 0x80");
    }
}
}  // namespace testing
