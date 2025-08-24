#include "cpu.h"
#include "memory.h"
#include "op_codes.h"
#include "test_utils.h"

using namespace colors;

namespace testing {

void inline_and_im_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set accumulator to a known value
    cpu.set(Register::A, 0b10101010);  // 0xAA

    // Inline test for AND immediate instruction
    mem[0xFFFC] = op(Op::AND_IM);  // AND immediate
    mem[0xFFFD] = 0b11110000;      // 0xF0 - Value to AND with accumulator

    bool program_completed = false;
    i32 cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Accumulator after AND_IM: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion - result should be 0xA0 (10100000 in binary)
    byte expected = 0b10101010 & 0b11110000;  // 0xA0
    if (cpu.get(Register::A) != expected) {
        throw testing::TestFailedException("AND_IM test failed: Accumulator should be 0xA0");
    }

    // Verify flags are correctly set
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("AND_IM test failed: Zero flag should not be set");
    }
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("AND_IM test failed: Negative flag should be set");
    }

    // Test with a result of zero
    cpu.reset(mem);
    cpu.set(Register::A, 0b00001111);  // 0x0F

    mem[0xFFFC] = op(Op::AND_IM);  // AND immediate
    mem[0xFFFD] = 0b11110000;      // 0xF0 - This will result in zero when AND with 0x0F

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify result is zero
    if (cpu.get(Register::A) != 0) {
        throw testing::TestFailedException("AND_IM test failed: Accumulator should be 0x00");
    }

    // Verify Zero flag is set, Negative flag is clear
    if (!cpu.FLAGS_Z) {
        throw testing::TestFailedException("AND_IM test failed: Zero flag should be set");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("AND_IM test failed: Negative flag should not be set");
    }
}

void inline_and_zp_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set accumulator to a known value
    cpu.set(Register::A, 0b10101010);  // 0xAA

    // Inline test for AND Zero Page instruction
    mem[0xFFFC] = op(Op::AND_ZP);  // AND Zero Page
    mem[0xFFFD] = 0x42;            // Zero page address
    mem[0x0042] = 0b11110000;      // Value at the zero page address (0xF0)

    bool program_completed = false;
    i32 cycles_used = cpu.execute(3, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Accumulator after AND_ZP: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion
    byte expected = 0b10101010 & 0b11110000;  // 0xA0
    if (cpu.get(Register::A) != expected) {
        throw testing::TestFailedException("AND_ZP test failed: Accumulator should be 0xA0");
    }
}

void inline_and_zpx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set accumulator to a known value
    cpu.set(Register::A, 0b10101010);  // 0xAA

    // Set X register value for offset
    cpu.set(Register::X, 0x05);

    // Inline test for AND Zero Page,X instruction
    mem[0xFFFC] = op(Op::AND_ZPX);  // AND Zero Page,X
    mem[0xFFFD] = 0x42;             // Zero page address
    mem[0x0047] = 0b11110000;       // Value at the effective address (0x42 + X) = 0x47

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Accumulator after AND_ZPX: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion
    byte expected = 0b10101010 & 0b11110000;  // 0xA0
    if (cpu.get(Register::A) != expected) {
        throw testing::TestFailedException("AND_ZPX test failed: Accumulator should be 0xA0");
    }
}

void inline_and_abs_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set accumulator to a known value
    cpu.set(Register::A, 0b10101010);  // 0xAA

    // Inline test for AND Absolute instruction
    mem[0xFFFC] = op(Op::AND_AB);  // AND Absolute
    mem[0xFFFD] = 0x80;            // Low byte of address
    mem[0xFFFE] = 0x44;            // High byte of address (0x4480)
    mem[0x4480] = 0b11110000;      // Value at the absolute address (0xF0)

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Accumulator after AND_AB: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion
    byte expected = 0b10101010 & 0b11110000;  // 0xA0
    if (cpu.get(Register::A) != expected) {
        throw testing::TestFailedException("AND_AB test failed: Accumulator should be 0xA0");
    }
}

void inline_and_absx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set accumulator to a known value
    cpu.set(Register::A, 0b10101010);  // 0xAA

    // Set X register value for offset
    cpu.set(Register::X, 0x05);

    // Inline test for AND Absolute,X instruction
    mem[0xFFFC] = op(Op::AND_ABSX);  // AND Absolute,X
    mem[0xFFFD] = 0x80;              // Low byte of address
    mem[0xFFFE] = 0x44;              // High byte of address (0x4480)
    mem[0x4485] = 0b11110000;        // Value at the effective address (0x4480 + X) = 0x4485

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Accumulator after AND_ABSX: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion
    byte expected = 0b10101010 & 0b11110000;  // 0xA0
    if (cpu.get(Register::A) != expected) {
        throw testing::TestFailedException("AND_ABSX test failed: Accumulator should be 0xA0");
    }

    // Test with page crossing
    cpu.reset(mem);
    cpu.set(Register::A, 0b10101010);  // 0xAA
    cpu.set(Register::X, 0xFF);        // Large offset to cross page boundary

    mem[0xFFFC] = op(Op::AND_ABSX);  // AND Absolute,X
    mem[0xFFFD] = 0x80;              // Low byte of address
    mem[0xFFFE] = 0x44;              // High byte of address (0x4480)
    mem[0x457F] = 0b11110000;        // Value at the effective address (0x4480 + 0xFF) = 0x457F

    program_completed = false;
    cycles_used = cpu.execute(5, mem, &program_completed, true);  // +1 cycle for page boundary crossing

    // Verify with test assertion
    expected = 0b10101010 & 0b11110000;  // 0xA0
    if (cpu.get(Register::A) != expected) {
        throw testing::TestFailedException("AND_ABSX page crossing test failed: Accumulator should be 0xA0");
    }
}

void inline_and_absy_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set accumulator to a known value
    cpu.set(Register::A, 0b10101010);  // 0xAA

    // Set Y register value for offset
    cpu.set(Register::Y, 0x06);

    // Inline test for AND Absolute,Y instruction
    mem[0xFFFC] = op(Op::AND_ABSY);  // AND Absolute,Y
    mem[0xFFFD] = 0x80;              // Low byte of address
    mem[0xFFFE] = 0x44;              // High byte of address (0x4480)
    mem[0x4486] = 0b11110000;        // Value at the effective address (0x4480 + Y) = 0x4486

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Accumulator after AND_ABSY: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion
    byte expected = 0b10101010 & 0b11110000;  // 0xA0
    if (cpu.get(Register::A) != expected) {
        throw testing::TestFailedException("AND_ABSY test failed: Accumulator should be 0xA0");
    }
}

void inline_and_indx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set accumulator to a known value
    cpu.set(Register::A, 0b10101010);  // 0xAA

    // Set X register value
    cpu.set(Register::X, 0x04);

    // Inline test for AND (Indirect,X) instruction
    mem[0xFFFC] = op(Op::AND_INX);  // AND (Indirect,X)
    mem[0xFFFD] = 0x20;             // Zero page address

    // Setup the indirect address
    mem[0x24] = 0x74;          // Low byte of target address (0x20 + X = 0x24)
    mem[0x25] = 0x20;          // High byte of target address (0x2074)
    mem[0x2074] = 0b11110000;  // Value at target address (0xF0)

    bool program_completed = false;
    i32 cycles_used = cpu.execute(6, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Accumulator after AND_INX: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion
    byte expected = 0b10101010 & 0b11110000;  // 0xA0
    if (cpu.get(Register::A) != expected) {
        throw testing::TestFailedException("AND_INX test failed: Accumulator should be 0xA0");
    }
}

void inline_and_indy_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set accumulator to a known value
    cpu.set(Register::A, 0b10101010);  // 0xAA

    // Set Y register value
    cpu.set(Register::Y, 0x10);

    // Inline test for AND (Indirect),Y instruction
    mem[0xFFFC] = op(Op::AND_INY);  // AND (Indirect),Y
    mem[0xFFFD] = 0x30;             // Zero page address

    // Setup the indirect address
    mem[0x30] = 0x40;          // Low byte of base address
    mem[0x31] = 0x30;          // High byte of base address (0x3040)
    mem[0x3050] = 0b11110000;  // Value at target address (0x3040 + Y = 0x3050)

    bool program_completed = false;
    i32 cycles_used = cpu.execute(5, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Accumulator after AND_INY: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion
    byte expected = 0b10101010 & 0b11110000;  // 0xA0
    if (cpu.get(Register::A) != expected) {
        throw testing::TestFailedException("AND_INY test failed: Accumulator should be 0xA0");
    }

    // Test with page boundary crossing
    cpu.reset(mem);
    cpu.set(Register::A, 0b10101010);  // 0xAA
    cpu.set(Register::Y, 0xD0);        // Large offset to cross page boundary

    mem[0xFFFC] = op(Op::AND_INY);  // AND (Indirect),Y
    mem[0xFFFD] = 0x30;             // Zero page address
    mem[0x30] = 0x80;               // Low byte of base address
    mem[0x31] = 0x30;               // High byte of base address (0x3080)
    mem[0x3150] = 0b11110000;       // Value at target address (0x3080 + 0xD0 = 0x3150)

    program_completed = false;
    cycles_used = cpu.execute(6, mem, &program_completed, true);  // +1 cycle for page boundary crossing

    // Verify with test assertion
    expected = 0b10101010 & 0b11110000;  // 0xA0
    if (cpu.get(Register::A) != expected) {
        throw testing::TestFailedException("AND_INY page crossing test failed: Accumulator should be 0xA0");
    }
}

// Test suite function for AND operations
void and_test_suite(Cpu& cpu, Mem& mem) {
    testing::TestSuite test_suite("AND Operations");

    test_suite.print_header();

    // Register and run individual tests
    test_suite.register_test("AND Immediate", [&]() { inline_and_im_test(cpu, mem); });
    test_suite.register_test("AND Zero Page", [&]() { inline_and_zp_test(cpu, mem); });
    test_suite.register_test("AND Zero Page,X", [&]() { inline_and_zpx_test(cpu, mem); });
    test_suite.register_test("AND Absolute", [&]() { inline_and_abs_test(cpu, mem); });
    test_suite.register_test("AND Absolute,X", [&]() { inline_and_absx_test(cpu, mem); });
    test_suite.register_test("AND Absolute,Y", [&]() { inline_and_absy_test(cpu, mem); });
    test_suite.register_test("AND (Indirect,X)", [&]() { inline_and_indx_test(cpu, mem); });
    test_suite.register_test("AND (Indirect),Y", [&]() { inline_and_indy_test(cpu, mem); });

    // Print the results
    test_suite.print_results();
}

}  // namespace testing
