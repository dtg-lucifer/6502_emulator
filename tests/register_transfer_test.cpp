#include "cpu.h"
#include "memory.h"
#include "op_codes.h"
#include "test_utils.h"

using namespace colors;

namespace testing {

void inline_tax_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set a known value in accumulator
    cpu.set(Register::A, 0x42);
    // Set X to a different value to ensure it gets overwritten
    cpu.set(Register::X, 0x00);

    // Inline test for TAX instruction (Transfer A to X)
    mem[0xFFFC] = op(Op::TAX);  // TAX instruction
    mem[0xFFFD] = op(Op::NOP);  // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> X register after TAX: 0x%02X (A: 0x%02X)%s\n", CYAN, cpu.get(Register::X), cpu.get(Register::A),
                RESET);

    // Verify with test assertion
    if (cpu.get(Register::X) != 0x42) {
        throw testing::TestFailedException("TAX test failed: X register should be 0x42");
    }

    // Verify flags are correctly set
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("TAX test failed: Zero flag should not be set for non-zero value");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("TAX test failed: Negative flag should not be set for value 0x42");
    }

    // Test with a zero value
    cpu.reset(mem);
    cpu.set(Register::A, 0x00);
    cpu.set(Register::X, 0x42);  // Set X to ensure it changes

    mem[0xFFFC] = op(Op::TAX);

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify X is zero
    if (cpu.get(Register::X) != 0x00) {
        throw testing::TestFailedException("TAX test failed: X register should be 0x00");
    }

    // Verify Zero flag is set
    if (!cpu.FLAGS_Z) {
        throw testing::TestFailedException("TAX test failed: Zero flag should be set for zero value");
    }

    // Test with a negative value
    cpu.reset(mem);
    cpu.set(Register::A, 0x80);  // Negative value (bit 7 set)
    cpu.set(Register::X, 0x00);  // Set X to ensure it changes

    mem[0xFFFC] = op(Op::TAX);

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify X has negative value
    if (cpu.get(Register::X) != 0x80) {
        throw testing::TestFailedException("TAX test failed: X register should be 0x80");
    }

    // Verify Negative flag is set
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("TAX test failed: Negative flag should be set for negative value");
    }
}

void inline_tay_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set a known value in accumulator
    cpu.set(Register::A, 0x42);
    // Set Y to a different value to ensure it gets overwritten
    cpu.set(Register::Y, 0x00);

    // Inline test for TAY instruction (Transfer A to Y)
    mem[0xFFFC] = op(Op::TAY);  // TAY instruction
    mem[0xFFFD] = op(Op::NOP);  // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Y register after TAY: 0x%02X (A: 0x%02X)%s\n", CYAN, cpu.get(Register::Y), cpu.get(Register::A),
                RESET);

    // Verify with test assertion
    if (cpu.get(Register::Y) != 0x42) {
        throw testing::TestFailedException("TAY test failed: Y register should be 0x42");
    }

    // Verify flags are correctly set
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("TAY test failed: Zero flag should not be set for non-zero value");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("TAY test failed: Negative flag should not be set for value 0x42");
    }

    // Test with a zero value
    cpu.reset(mem);
    cpu.set(Register::A, 0x00);
    cpu.set(Register::Y, 0x42);  // Set Y to ensure it changes

    mem[0xFFFC] = op(Op::TAY);

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify Y is zero
    if (cpu.get(Register::Y) != 0x00) {
        throw testing::TestFailedException("TAY test failed: Y register should be 0x00");
    }

    // Verify Zero flag is set
    if (!cpu.FLAGS_Z) {
        throw testing::TestFailedException("TAY test failed: Zero flag should be set for zero value");
    }

    // Test with a negative value
    cpu.reset(mem);
    cpu.set(Register::A, 0x80);  // Negative value (bit 7 set)
    cpu.set(Register::Y, 0x00);  // Set Y to ensure it changes

    mem[0xFFFC] = op(Op::TAY);

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify Y has negative value
    if (cpu.get(Register::Y) != 0x80) {
        throw testing::TestFailedException("TAY test failed: Y register should be 0x80");
    }

    // Verify Negative flag is set
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("TAY test failed: Negative flag should be set for negative value");
    }
}

void inline_txa_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set a known value in X register
    cpu.set(Register::X, 0x42);
    // Set A to a different value to ensure it gets overwritten
    cpu.set(Register::A, 0x00);

    // Inline test for TXA instruction (Transfer X to A)
    mem[0xFFFC] = op(Op::TXA);  // TXA instruction
    mem[0xFFFD] = op(Op::NOP);  // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> A register after TXA: 0x%02X (X: 0x%02X)%s\n", CYAN, cpu.get(Register::A), cpu.get(Register::X),
                RESET);

    // Verify with test assertion
    if (cpu.get(Register::A) != 0x42) {
        throw testing::TestFailedException("TXA test failed: A register should be 0x42");
    }

    // Verify flags are correctly set
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("TXA test failed: Zero flag should not be set for non-zero value");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("TXA test failed: Negative flag should not be set for value 0x42");
    }

    // Test with a zero value
    cpu.reset(mem);
    cpu.set(Register::X, 0x00);
    cpu.set(Register::A, 0x42);  // Set A to ensure it changes

    mem[0xFFFC] = op(Op::TXA);

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify A is zero
    if (cpu.get(Register::A) != 0x00) {
        throw testing::TestFailedException("TXA test failed: A register should be 0x00");
    }

    // Verify Zero flag is set
    if (!cpu.FLAGS_Z) {
        throw testing::TestFailedException("TXA test failed: Zero flag should be set for zero value");
    }

    // Test with a negative value
    cpu.reset(mem);
    cpu.set(Register::X, 0x80);  // Negative value (bit 7 set)
    cpu.set(Register::A, 0x00);  // Set A to ensure it changes

    mem[0xFFFC] = op(Op::TXA);

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify A has negative value
    if (cpu.get(Register::A) != 0x80) {
        throw testing::TestFailedException("TXA test failed: A register should be 0x80");
    }

    // Verify Negative flag is set
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("TXA test failed: Negative flag should be set for negative value");
    }
}

void inline_tya_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set a known value in Y register
    cpu.set(Register::Y, 0x42);
    // Set A to a different value to ensure it gets overwritten
    cpu.set(Register::A, 0x00);

    // Inline test for TYA instruction (Transfer Y to A)
    mem[0xFFFC] = op(Op::TYA);  // TYA instruction
    mem[0xFFFD] = op(Op::NOP);  // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> A register after TYA: 0x%02X (Y: 0x%02X)%s\n", CYAN, cpu.get(Register::A), cpu.get(Register::Y),
                RESET);

    // Verify with test assertion
    if (cpu.get(Register::A) != 0x42) {
        throw testing::TestFailedException("TYA test failed: A register should be 0x42");
    }

    // Verify flags are correctly set
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("TYA test failed: Zero flag should not be set for non-zero value");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("TYA test failed: Negative flag should not be set for value 0x42");
    }

    // Test with a zero value
    cpu.reset(mem);
    cpu.set(Register::Y, 0x00);
    cpu.set(Register::A, 0x42);  // Set A to ensure it changes

    mem[0xFFFC] = op(Op::TYA);

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify A is zero
    if (cpu.get(Register::A) != 0x00) {
        throw testing::TestFailedException("TYA test failed: A register should be 0x00");
    }

    // Verify Zero flag is set
    if (!cpu.FLAGS_Z) {
        throw testing::TestFailedException("TYA test failed: Zero flag should be set for zero value");
    }

    // Test with a negative value
    cpu.reset(mem);
    cpu.set(Register::Y, 0x80);  // Negative value (bit 7 set)
    cpu.set(Register::A, 0x00);  // Set A to ensure it changes

    mem[0xFFFC] = op(Op::TYA);

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify A has negative value
    if (cpu.get(Register::A) != 0x80) {
        throw testing::TestFailedException("TYA test failed: A register should be 0x80");
    }

    // Verify Negative flag is set
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("TYA test failed: Negative flag should be set for negative value");
    }
}

// Test suite function for register transfer operations
void register_transfer_test_suite(Cpu& cpu, Mem& mem) {
    testing::TestSuite test_suite("Register Transfer Operations");

    test_suite.print_header();

    // Register and run individual tests
    test_suite.register_test("Transfer A to X (TAX)", [&]() { inline_tax_test(cpu, mem); });
    test_suite.register_test("Transfer A to Y (TAY)", [&]() { inline_tay_test(cpu, mem); });
    test_suite.register_test("Transfer X to A (TXA)", [&]() { inline_txa_test(cpu, mem); });
    test_suite.register_test("Transfer Y to A (TYA)", [&]() { inline_tya_test(cpu, mem); });

    // Print the results
    test_suite.print_results();
}

}  // namespace testing
