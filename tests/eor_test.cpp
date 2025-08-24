#include "cpu.h"
#include "memory.h"
#include "op_codes.h"
#include "test_utils.h"

using namespace colors;

namespace testing {

void inline_eor_im_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set accumulator to a known value
    cpu.set(Register::A, 0b10101010);  // 0xAA

    // Inline test for EOR immediate instruction
    mem[0xFFFC] = op(Op::EOR_IM);  // EOR immediate
    mem[0xFFFD] = 0b11110000;      // 0xF0 - Value to EOR with accumulator

    bool program_completed = false;
    i32 cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Accumulator after EOR_IM: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion - result should be 0x5A (01011010 in binary)
    byte expected = 0b10101010 ^ 0b11110000;  // 0x5A
    if (cpu.get(Register::A) != expected) {
        throw testing::TestFailedException("EOR_IM test failed: Accumulator should be 0x5A");
    }

    // Verify flags are correctly set
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("EOR_IM test failed: Zero flag should not be set");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("EOR_IM test failed: Negative flag should not be set");
    }

    // Test with a result of zero
    cpu.reset(mem);
    cpu.set(Register::A, 0b11110000);  // 0xF0

    mem[0xFFFC] = op(Op::EOR_IM);  // EOR immediate
    mem[0xFFFD] = 0b11110000;      // 0xF0 - This will result in zero when EOR with 0xF0

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify result is zero
    if (cpu.get(Register::A) != 0) {
        throw testing::TestFailedException("EOR_IM test failed: Accumulator should be 0x00");
    }

    // Verify Zero flag is set, Negative flag is clear
    if (!cpu.FLAGS_Z) {
        throw testing::TestFailedException("EOR_IM test failed: Zero flag should be set");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("EOR_IM test failed: Negative flag should not be set");
    }

    // Test with a negative result
    cpu.reset(mem);
    cpu.set(Register::A, 0b00001111);  // 0x0F

    mem[0xFFFC] = op(Op::EOR_IM);  // EOR immediate
    mem[0xFFFD] = 0b10000000;      // 0x80 - This will result in negative when EOR with 0x0F

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify result has bit 7 set
    expected = 0b00001111 ^ 0b10000000;  // 0x8F
    if (cpu.get(Register::A) != expected) {
        throw testing::TestFailedException("EOR_IM test failed: Accumulator should be 0x8F");
    }

    // Verify Negative flag is set, Zero flag is clear
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("EOR_IM test failed: Negative flag should be set");
    }
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("EOR_IM test failed: Zero flag should not be set");
    }
}

void inline_eor_zp_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set accumulator to a known value
    cpu.set(Register::A, 0b10101010);  // 0xAA

    // Inline test for EOR Zero Page instruction
    mem[0xFFFC] = op(Op::EOR_ZP);  // EOR Zero Page
    mem[0xFFFD] = 0x42;            // Zero page address
    mem[0x0042] = 0b11110000;      // Value at the zero page address (0xF0)

    bool program_completed = false;
    i32 cycles_used = cpu.execute(3, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Accumulator after EOR_ZP: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion
    byte expected = 0b10101010 ^ 0b11110000;  // 0x5A
    if (cpu.get(Register::A) != expected) {
        throw testing::TestFailedException("EOR_ZP test failed: Accumulator should be 0x5A");
    }
}

void inline_eor_zpx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set accumulator to a known value
    cpu.set(Register::A, 0b10101010);  // 0xAA

    // Set X register value for offset
    cpu.set(Register::X, 0x05);

    // Inline test for EOR Zero Page,X instruction
    mem[0xFFFC] = op(Op::EOR_ZPX);  // EOR Zero Page,X
    mem[0xFFFD] = 0x42;             // Zero page address
    mem[0x0047] = 0b11110000;       // Value at the effective address (0x42 + X) = 0x47

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Accumulator after EOR_ZPX: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion
    byte expected = 0b10101010 ^ 0b11110000;  // 0x5A
    if (cpu.get(Register::A) != expected) {
        throw testing::TestFailedException("EOR_ZPX test failed: Accumulator should be 0x5A");
    }
}

void inline_eor_abs_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set accumulator to a known value
    cpu.set(Register::A, 0b10101010);  // 0xAA

    // Inline test for EOR Absolute instruction
    mem[0xFFFC] = op(Op::EOR_AB);  // EOR Absolute
    mem[0xFFFD] = 0x80;            // Low byte of address
    mem[0xFFFE] = 0x44;            // High byte of address (0x4480)
    mem[0x4480] = 0b11110000;      // Value at the absolute address (0xF0)

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Accumulator after EOR_AB: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion
    byte expected = 0b10101010 ^ 0b11110000;  // 0x5A
    if (cpu.get(Register::A) != expected) {
        throw testing::TestFailedException("EOR_AB test failed: Accumulator should be 0x5A");
    }
}

void inline_eor_absx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set accumulator to a known value
    cpu.set(Register::A, 0b10101010);  // 0xAA

    // Set X register value for offset
    cpu.set(Register::X, 0x05);

    // Inline test for EOR Absolute,X instruction
    mem[0xFFFC] = op(Op::EOR_ABSX);  // EOR Absolute,X
    mem[0xFFFD] = 0x80;              // Low byte of address
    mem[0xFFFE] = 0x44;              // High byte of address (0x4480)
    mem[0x4485] = 0b11110000;        // Value at the effective address (0x4480 + X) = 0x4485

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Accumulator after EOR_ABSX: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion
    byte expected = 0b10101010 ^ 0b11110000;  // 0x5A
    if (cpu.get(Register::A) != expected) {
        throw testing::TestFailedException("EOR_ABSX test failed: Accumulator should be 0x5A");
    }

    // Test with page crossing
    cpu.reset(mem);
    cpu.set(Register::A, 0b10101010);  // 0xAA
    cpu.set(Register::X, 0xFF);        // Large offset to cross page boundary

    mem[0xFFFC] = op(Op::EOR_ABSX);  // EOR Absolute,X
    mem[0xFFFD] = 0x80;              // Low byte of address
    mem[0xFFFE] = 0x44;              // High byte of address (0x4480)
    mem[0x457F] = 0b11110000;        // Value at the effective address (0x4480 + 0xFF) = 0x457F

    program_completed = false;
    cycles_used = cpu.execute(5, mem, &program_completed, true);  // +1 cycle for page boundary crossing

    // Verify with test assertion
    expected = 0b10101010 ^ 0b11110000;  // 0x5A
    if (cpu.get(Register::A) != expected) {
        throw testing::TestFailedException("EOR_ABSX page crossing test failed: Accumulator should be 0x5A");
    }
}

void inline_eor_absy_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set accumulator to a known value
    cpu.set(Register::A, 0b10101010);  // 0xAA

    // Set Y register value for offset
    cpu.set(Register::Y, 0x06);

    // Inline test for EOR Absolute,Y instruction
    mem[0xFFFC] = op(Op::EOR_ABSY);  // EOR Absolute,Y
    mem[0xFFFD] = 0x80;              // Low byte of address
    mem[0xFFFE] = 0x44;              // High byte of address (0x4480)
    mem[0x4486] = 0b11110000;        // Value at the effective address (0x4480 + Y) = 0x4486

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Accumulator after EOR_ABSY: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion
    byte expected = 0b10101010 ^ 0b11110000;  // 0x5A
    if (cpu.get(Register::A) != expected) {
        throw testing::TestFailedException("EOR_ABSY test failed: Accumulator should be 0x5A");
    }
}

void inline_eor_indx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set accumulator to a known value
    cpu.set(Register::A, 0b10101010);  // 0xAA

    // Set X register value
    cpu.set(Register::X, 0x04);

    // Inline test for EOR (Indirect,X) instruction
    mem[0xFFFC] = op(Op::EOR_INX);  // EOR (Indirect,X)
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
    std::printf("%s>> Accumulator after EOR_INX: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion
    byte expected = 0b10101010 ^ 0b11110000;  // 0x5A
    if (cpu.get(Register::A) != expected) {
        throw testing::TestFailedException("EOR_INX test failed: Accumulator should be 0x5A");
    }
}

void inline_eor_indy_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set accumulator to a known value
    cpu.set(Register::A, 0b10101010);  // 0xAA

    // Set Y register value
    cpu.set(Register::Y, 0x10);

    // Inline test for EOR (Indirect),Y instruction
    mem[0xFFFC] = op(Op::EOR_INY);  // EOR (Indirect),Y
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
    std::printf("%s>> Accumulator after EOR_INY: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion
    byte expected = 0b10101010 ^ 0b11110000;  // 0x5A
    if (cpu.get(Register::A) != expected) {
        throw testing::TestFailedException("EOR_INY test failed: Accumulator should be 0x5A");
    }

    // Test with page boundary crossing
    cpu.reset(mem);
    cpu.set(Register::A, 0b10101010);  // 0xAA
    cpu.set(Register::Y, 0xD0);        // Large offset to cross page boundary

    mem[0xFFFC] = op(Op::EOR_INY);  // EOR (Indirect),Y
    mem[0xFFFD] = 0x30;             // Zero page address
    mem[0x30] = 0x80;               // Low byte of base address
    mem[0x31] = 0x30;               // High byte of base address (0x3080)
    mem[0x3150] = 0b11110000;       // Value at target address (0x3080 + 0xD0 = 0x3150)

    program_completed = false;
    cycles_used = cpu.execute(6, mem, &program_completed, true);  // +1 cycle for page boundary crossing

    // Verify with test assertion
    expected = 0b10101010 ^ 0b11110000;  // 0x5A
    if (cpu.get(Register::A) != expected) {
        throw testing::TestFailedException("EOR_INY page crossing test failed: Accumulator should be 0x5A");
    }
}

// Test suite function for EOR operations
void eor_test_suite(Cpu& cpu, Mem& mem) {
    testing::TestSuite test_suite("EOR Operations");

    test_suite.print_header();

    // Register and run individual tests
    test_suite.register_test("EOR Immediate", [&]() { inline_eor_im_test(cpu, mem); });
    test_suite.register_test("EOR Zero Page", [&]() { inline_eor_zp_test(cpu, mem); });
    test_suite.register_test("EOR Zero Page,X", [&]() { inline_eor_zpx_test(cpu, mem); });
    test_suite.register_test("EOR Absolute", [&]() { inline_eor_abs_test(cpu, mem); });
    test_suite.register_test("EOR Absolute,X", [&]() { inline_eor_absx_test(cpu, mem); });
    test_suite.register_test("EOR Absolute,Y", [&]() { inline_eor_absy_test(cpu, mem); });
    test_suite.register_test("EOR (Indirect,X)", [&]() { inline_eor_indx_test(cpu, mem); });
    test_suite.register_test("EOR (Indirect),Y", [&]() { inline_eor_indy_test(cpu, mem); });

    // Print the results
    test_suite.print_results();
}

}  // namespace testing
