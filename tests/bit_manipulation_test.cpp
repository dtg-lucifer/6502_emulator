#include "cpu.h"
#include "memory.h"
#include "op_codes.h"
#include "test_utils.h"

using namespace colors;

namespace testing {

// Test BIT Zero Page instruction
void inline_bit_zp_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set up test values
    byte test_mem_value = 0b10100000;    // bit 7 is set (N flag), bit 6 is clear (V flag), bit 5-0 for testing with A
    byte test_accumulator = 0b00100000;  // bit 5 set to test Zero flag behavior

    // Store test value in memory
    byte zp_address = 0x42;
    mem[zp_address] = test_mem_value;

    // Set accumulator value
    cpu.set(Register::A, test_accumulator);

    // Load BIT Zero Page instruction
    mem[0xFFFC] = op(Op::BIT_ZP);
    mem[0xFFFD] = zp_address;
    mem[0xFFFE] = op(Op::NOP);  // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(3, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check that N flag is set (bit 7 of memory value)
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("BIT_ZP test failed: N flag should be set (bit 7 of memory)");
    }

    // Check that V flag is clear (bit 6 of memory value)
    if (cpu.FLAGS_V) {
        throw testing::TestFailedException("BIT_ZP test failed: V flag should be clear (bit 6 of memory)");
    }

    // Test case where A AND M equals zero (no bits in common)
    // In this case, Z flag should be set
    byte test_mem_value2 = 0b10000000;    // Only bit 7 set
    byte test_accumulator2 = 0b00000001;  // Only bit 0 set
    mem[zp_address] = test_mem_value2;
    cpu.set(Register::A, test_accumulator2);

    cpu.PC = 0xFFFC;  // Reset PC
    program_completed = false;
    cycles_used = cpu.execute(3, mem, &program_completed, true);

    // Check Z flag is set (A & M = 0)
    if (!cpu.FLAGS_Z) {
        throw testing::TestFailedException("BIT_ZP test failed: Z flag should be set when A & M = 0");
    }

    // Test case where A AND M is non-zero
    // Z flag should be clear
    byte test_mem_value3 = 0b11000001;    // Bits 7, 6, and 0 set
    byte test_accumulator3 = 0b00000001;  // Bit 0 set
    mem[zp_address] = test_mem_value3;
    cpu.set(Register::A, test_accumulator3);

    cpu.PC = 0xFFFC;  // Reset PC
    program_completed = false;
    cycles_used = cpu.execute(3, mem, &program_completed, true);

    // Check Z flag is clear (A & M ≠ 0)
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("BIT_ZP test failed: Z flag should be clear when A & M ≠ 0");
    }

    // Check that N and V flags match bits 7 and 6 of memory
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("BIT_ZP test failed: N flag should be set (bit 7 of memory)");
    }
    if (!cpu.FLAGS_V) {
        throw testing::TestFailedException("BIT_ZP test failed: V flag should be set (bit 6 of memory)");
    }

    // Test that accumulator is not modified
    if (cpu.get(Register::A) != test_accumulator3) {
        throw testing::TestFailedException("BIT_ZP test failed: Accumulator should not be modified");
    }
}

// Test BIT Absolute instruction
void inline_bit_abs_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set up test values
    byte test_mem_value = 0b11000000;    // bit 7 and 6 are set (N and V flags)
    byte test_accumulator = 0b00100000;  // bit 5 set to test Zero flag behavior

    // Store test value in memory
    word abs_address = 0x4242;
    mem[abs_address] = test_mem_value;

    // Set accumulator value
    cpu.set(Register::A, test_accumulator);

    // Load BIT Absolute instruction
    mem[0xFFFC] = op(Op::BIT_AB);
    mem[0xFFFD] = abs_address & 0xFF;         // Low byte
    mem[0xFFFE] = (abs_address >> 8) & 0xFF;  // High byte
    mem[0xFFFF] = op(Op::NOP);                // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check that N flag is set (bit 7 of memory value)
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("BIT_AB test failed: N flag should be set (bit 7 of memory)");
    }

    // Check that V flag is set (bit 6 of memory value)
    if (!cpu.FLAGS_V) {
        throw testing::TestFailedException("BIT_AB test failed: V flag should be set (bit 6 of memory)");
    }

    // Check Z flag is set (A & M = 0, no bits in common)
    if (!cpu.FLAGS_Z) {
        throw testing::TestFailedException("BIT_AB test failed: Z flag should be set when A & M = 0");
    }

    // Test case where A AND M is non-zero
    // Z flag should be clear
    byte test_mem_value2 = 0b00100000;    // Bit 5 set
    byte test_accumulator2 = 0b00100000;  // Bit 5 set
    mem[abs_address] = test_mem_value2;
    cpu.set(Register::A, test_accumulator2);

    cpu.PC = 0xFFFC;  // Reset PC
    program_completed = false;
    cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Check Z flag is clear (A & M ≠ 0)
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("BIT_AB test failed: Z flag should be clear when A & M ≠ 0");
    }

    // Check that N and V flags match bits 7 and 6 of memory
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("BIT_AB test failed: N flag should be clear (bit 7 of memory)");
    }
    if (cpu.FLAGS_V) {
        throw testing::TestFailedException("BIT_AB test failed: V flag should be clear (bit 6 of memory)");
    }

    // Test that accumulator is not modified
    if (cpu.get(Register::A) != test_accumulator2) {
        throw testing::TestFailedException("BIT_AB test failed: Accumulator should not be modified");
    }
}

// Test suite function for bit manipulation operations
void bit_manipulation_test_suite(Cpu& cpu, Mem& mem) {
    testing::TestSuite test_suite("Bit Manipulation Operations");

    test_suite.print_header();

    // Register and run individual tests
    test_suite.register_test("BIT Zero Page (BIT ZP)", [&]() { inline_bit_zp_test(cpu, mem); });
    test_suite.register_test("BIT Absolute (BIT AB)", [&]() { inline_bit_abs_test(cpu, mem); });

    // Print the results
    test_suite.print_results();
}

}  // namespace testing
