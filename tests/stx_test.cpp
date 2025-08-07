#include "cpu.h"
#include "memory.h"
#include "op_codes.h"
#include "test_utils.h"

using namespace colors;

namespace testing {

void inline_stx_zp_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set a known value in the X register
    cpu.set(Register::X, 0x42);

    // Inline test for STX Zero Page instruction
    mem[0xFFFC] = op(Op::STX_ZP);  // Store X in Zero Page
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
        throw testing::TestFailedException("STX_ZP test failed: Memory at 0x0042 should be 0x42");
    }

    // STX should not affect any processor flags
    if (cpu.Z != 0 || cpu.N != 0) {
        throw testing::TestFailedException("STX_ZP test failed: Flags should not be affected");
    }
}

void inline_stx_zpy_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set a known value in the X register
    cpu.set(Register::X, 0x37);

    // Set Y register value for offset
    cpu.set(Register::Y, 0x05);

    // Inline test for STX Zero Page,Y instruction
    mem[0xFFFC] = op(Op::STX_ZPY);  // Store X in Zero Page,Y
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
        throw testing::TestFailedException("STX_ZPY test failed: Memory at 0x0047 should be 0x37");
    }
}

void inline_stx_zpy_wrap_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set a known value in the X register
    cpu.set(Register::X, 0x39);

    // Set Y register value for offset that will cause wrap
    cpu.set(Register::Y, 0x05);

    // Inline test for STX Zero Page,Y instruction with wrap
    mem[0xFFFC] = op(Op::STX_ZPY);  // Store X in Zero Page,Y
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
        throw testing::TestFailedException("STX_ZPY wrap test failed: Memory at 0x0003 should be 0x39");
    }
}

void inline_stx_absolute_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set a known value in the X register
    cpu.set(Register::X, 0x72);

    // Inline test for STX Absolute instruction
    mem[0xFFFC] = op(Op::STX_ABS);  // Store X at Absolute address
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
        throw testing::TestFailedException("STX_ABS test failed: Memory at 0x4480 should be 0x72");
    }
}

// Test suite function
void stx_test_suite(testing::TestSuite& test_suite, Cpu& cpu, Mem& mem) {
    test_suite.register_test("Inline STX Zero Page Test", [&]() { inline_stx_zp_test(cpu, mem); });
    test_suite.register_test("Inline STX Zero Page,Y Test", [&]() { inline_stx_zpy_test(cpu, mem); });
    test_suite.register_test("Inline STX Zero Page,Y (Wrapping) Test", [&]() { inline_stx_zpy_wrap_test(cpu, mem); });
    test_suite.register_test("Inline STX Absolute Test", [&]() { inline_stx_absolute_test(cpu, mem); });
}

}  // namespace testing
