#include "cpu.h"
#include "memory.h"
#include "op_codes.h"
#include "test_utils.h"

using namespace colors;

namespace testing {

void inline_jmp_absolute_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Inline test for JMP Absolute instruction
    mem[0xFFFC] = op(Op::JMP);  // Jump to absolute address
    mem[0xFFFD] = 0x00;         // Low byte of target address
    mem[0xFFFE] = 0x20;         // High byte of target address (0x2000)

    // Place instructions at the target location
    mem[0x2000] = op(Op::LDA_IM);  // Load a test value
    mem[0x2001] = 0x42;            // Test value

    bool program_completed = false;
    i32 cycles_used = cpu.execute(6, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> PC after JMP: 0x%04X, A register: 0x%02X%s\n", CYAN, cpu.PC, cpu.get(Register::A), RESET);

    // Verify with test assertion
    // PC should point to the next instruction after LDA_IM and its operand
    if (cpu.PC != 0x2002) {
        throw testing::TestFailedException(
            "JMP_ABS test failed: PC should be 0x2002 (after executing LDA_IM instruction)");
    }

    // Verify the LDA_IM instruction at the jump target was executed
    if (cpu.get(Register::A) != 0x42) {
        throw testing::TestFailedException(
            "JMP_ABS test failed: A register should be 0x42 (LDA_IM instruction was not executed)");
    }

    // JMP should not affect any processor flags
    if (cpu.FLAGS_Z != 0 || cpu.FLAGS_N != 0) {
        throw testing::TestFailedException("JMP_ABS test failed: Flags should not be affected directly by JMP");
    }
}

void inline_jmp_indirect_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Inline test for JMP Indirect instruction
    mem[0xFFFC] = op(Op::JMPI);  // Jump to indirect address
    mem[0xFFFD] = 0x20;          // Low byte of pointer address
    mem[0xFFFE] = 0x20;          // High byte of pointer address (0x2020)

    // Set up the indirect address
    mem[0x2020] = 0x40;  // Low byte of target address
    mem[0x2021] = 0x30;  // High byte of target address (0x3040)

    // Place a test instruction at the target location
    mem[0x3040] = op(Op::LDA_IM);  // Load a test value
    mem[0x3041] = 0xBB;            // Test value

    bool program_completed = false;
    i32 cycles_used = cpu.execute(7, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> PC after JMPI: 0x%04X, A register: 0x%02X%s\n", CYAN, cpu.PC, cpu.get(Register::A), RESET);

    // Verify with test assertion
    if (cpu.PC != 0x3042) {
        throw testing::TestFailedException("JMPI test failed: PC should be 0x3042");
    }

    // Verify the instruction after the jump was executed
    if (cpu.get(Register::A) != 0xBB) {
        throw testing::TestFailedException("JMPI test failed: A register should be 0xBB");
    }
}

void inline_jmp_indirect_page_boundary_bug_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Inline test for JMP Indirect instruction with page boundary bug
    mem[0xFFFC] = op(Op::JMPI);  // Jump to indirect address
    mem[0xFFFD] = 0xFF;          // Low byte of pointer address
    mem[0xFFFE] = 0x20;          // High byte of pointer address (0x20FF)

    // Set up the indirect address - note this crosses a page boundary
    mem[0x20FF] = 0x40;  // Low byte of target address
    mem[0x2100] = 0x30;  // This would be the high byte in normal operation
    // But due to 6502 bug, it will wrap to the start of the page
    mem[0x2000] = 0x50;  // This will actually be used as high byte (0x5040)

    // Place a test instruction at both possible target locations
    mem[0x3040] = op(Op::LDA_IM);  // Load a test value (for correct behavior)
    mem[0x3041] = 0xBB;            // Test value

    mem[0x5040] = op(Op::LDA_IM);  // Load a different test value (for buggy behavior)
    mem[0x5041] = 0xCC;            // Different test value

    bool program_completed = false;
    i32 cycles_used = cpu.execute(7, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> PC after JMPI (page boundary): 0x%04X, A register: 0x%02X%s\n", CYAN, cpu.PC,
                cpu.get(Register::A), RESET);

    // Verify with test assertion - this should match the buggy behavior of the 6502
    if (cpu.PC != 0x5042) {
        throw testing::TestFailedException("JMPI page boundary bug test failed: PC should be 0x5042");
    }

    // Verify the instruction after the jump was executed with the buggy address
    if (cpu.get(Register::A) != 0xCC) {
        throw testing::TestFailedException("JMPI page boundary bug test failed: A register should be 0xCC");
    }
}

// Use this function to register all JMP tests with a test suite
void jmp_test_suite(Cpu& cpu, Mem& mem) {
    testing::TestSuite test_suite("JMP Op Code");

    test_suite.print_header();

    // Register and run individual tests
    test_suite.register_test("Inline JMP Absolute Test", [&]() { inline_jmp_absolute_test(cpu, mem); });
    test_suite.register_test("Inline JMP Indirect Test", [&]() { inline_jmp_indirect_test(cpu, mem); });
    test_suite.register_test("Inline JMP Indirect Page Boundary Bug Test",
                             [&]() { inline_jmp_indirect_page_boundary_bug_test(cpu, mem); });

    // Print the results
    test_suite.print_results();
}

}  // namespace testing
