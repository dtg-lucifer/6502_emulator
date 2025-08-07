#include "cpu.h"
#include "memory.h"
#include "op_codes.h"
#include "test_utils.h"

using namespace colors;

namespace testing {

void inline_sty_zp_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set a known value in the Y register
    cpu.set(Register::Y, 0x42);

    // Inline test for STY Zero Page instruction
    mem[0xFFFC] = op(Op::STY_ZP);  // Store Y in Zero Page
    mem[0xFFFD] = 0x42;            // Zero page address
    mem[0xFFFE] = op(Op::NOP);     // No operation to terminate

    bool program_completed = false;
    i32 cycles_used = cpu.execute(3, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Memory at zero page 0x42: 0x%02X%s\n", CYAN, mem[0x0042], RESET);

    // Verify with test assertion
    if (mem[0x0042] != 0x42) {
        throw testing::TestFailedException("STY_ZP test failed: Memory at 0x0042 should be 0x42");
    }

    // STY should not affect any processor flags
    if (cpu.Z != 0 || cpu.N != 0) {
        throw testing::TestFailedException("STY_ZP test failed: Flags should not be affected");
    }
}

void inline_sty_zpx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set a known value in the Y register
    cpu.set(Register::Y, 0x37);

    // Set X register value for offset
    cpu.set(Register::X, 0x05);

    // Inline test for STY Zero Page,X instruction
    mem[0xFFFC] = op(Op::STY_ZPX);  // Store Y in Zero Page,X
    mem[0xFFFD] = 0x42;             // Zero page base address
    mem[0xFFFE] = op(Op::NOP);      // No operation to terminate

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Memory at zero page 0x47: 0x%02X%s\n", CYAN, mem[0x0047], RESET);

    // Verify with test assertion
    if (mem[0x0047] != 0x37) {  // 0x42 + 0x05 = 0x47
        throw testing::TestFailedException("STY_ZPX test failed: Memory at 0x0047 should be 0x37");
    }
}

void inline_sty_zpx_wrap_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set a known value in the Y register
    cpu.set(Register::Y, 0x39);

    // Set X register value for offset that will cause wrap
    cpu.set(Register::X, 0x05);

    // Inline test for STY Zero Page,X instruction with wrap
    mem[0xFFFC] = op(Op::STY_ZPX);  // Store Y in Zero Page,X
    mem[0xFFFD] = 0xFE;             // Zero page base address
    mem[0xFFFE] = op(Op::NOP);      // No operation to terminate

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Memory at zero page 0x03: 0x%02X%s\n", CYAN, mem[0x0003], RESET);

    // Verify with test assertion - should wrap around (0xFE + 0x05 = 0x103, which wraps to 0x03)
    if (mem[0x0003] != 0x39) {
        throw testing::TestFailedException("STY_ZPX wrap test failed: Memory at 0x0003 should be 0x39");
    }
}

void inline_sty_absolute_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set a known value in the Y register
    cpu.set(Register::Y, 0x72);

    // Inline test for STY Absolute instruction
    mem[0xFFFC] = op(Op::STY_ABS);  // Store Y at Absolute address
    mem[0xFFFD] = 0x80;             // Low byte of address
    mem[0xFFFE] = 0x44;             // High byte of address (0x4480)
    mem[0xFFFF] = op(Op::NOP);      // No operation to terminate

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Memory at absolute 0x4480: 0x%02X%s\n", CYAN, mem[0x4480], RESET);

    // Verify with test assertion
    if (mem[0x4480] != 0x72) {
        throw testing::TestFailedException("STY_ABS test failed: Memory at 0x4480 should be 0x72");
    }
}

// Test suite function
void sty_test_suite(testing::TestSuite& test_suite, Cpu& cpu, Mem& mem) {
    test_suite.register_test("Inline STY Zero Page Test", [&]() { inline_sty_zp_test(cpu, mem); });
    test_suite.register_test("Inline STY Zero Page,X Test", [&]() { inline_sty_zpx_test(cpu, mem); });
    test_suite.register_test("Inline STY Zero Page,X (Wrapping) Test", [&]() { inline_sty_zpx_wrap_test(cpu, mem); });
    test_suite.register_test("Inline STY Absolute Test", [&]() { inline_sty_absolute_test(cpu, mem); });
}

}  // namespace testing
