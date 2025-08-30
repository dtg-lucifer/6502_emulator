#include "cpu.h"
#include "memory.h"
#include "op_codes.h"
#include "test_utils.h"

using namespace colors;

namespace testing {

// CMP Tests
void inline_cmp_im_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Test case 1: A > Memory (should set carry, clear zero)
    cpu.set(Register::A, 0x50);  // A = 80

    mem[0xFFFC] = op(Op::CMP_IM);  // Compare Immediate
    mem[0xFFFD] = 0x30;            // Compare with 48

    bool program_completed = false;
    i32 cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> CMP_IM result: Accumulator=0x%02X, Value=0x%02X, Flags: Z=%d, C=%d, N=%d%s\n", CYAN,
                cpu.get(Register::A), 0x30, cpu.FLAGS_Z, cpu.FLAGS_C, cpu.FLAGS_N, RESET);

    // Verify flags are correctly set
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("CMP_IM test failed: Zero flag should not be set when A > Memory");
    }
    if (!cpu.FLAGS_C) {
        throw testing::TestFailedException("CMP_IM test failed: Carry flag should be set when A >= Memory");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("CMP_IM test failed: Negative flag should not be set");
    }

    // Test case 2: A = Memory (should set carry and zero)
    cpu.reset(mem);
    cpu.set(Register::A, 0x30);  // A = 48

    mem[0xFFFC] = op(Op::CMP_IM);
    mem[0xFFFD] = 0x30;  // Compare with 48

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify flags when equal
    if (!cpu.FLAGS_Z) {
        throw testing::TestFailedException("CMP_IM equal test failed: Zero flag should be set when A = Memory");
    }
    if (!cpu.FLAGS_C) {
        throw testing::TestFailedException("CMP_IM equal test failed: Carry flag should be set when A >= Memory");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("CMP_IM equal test failed: Negative flag should not be set");
    }

    // Test case 3: A < Memory (should clear carry and zero, may set negative)
    cpu.reset(mem);
    cpu.set(Register::A, 0x10);  // A = 16

    mem[0xFFFC] = op(Op::CMP_IM);
    mem[0xFFFD] = 0x20;  // Compare with 32

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify flags when less than
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("CMP_IM less test failed: Zero flag should not be set when A < Memory");
    }
    if (cpu.FLAGS_C) {
        throw testing::TestFailedException("CMP_IM less test failed: Carry flag should be clear when A < Memory");
    }
    // Negative flag depends on the result of A - M, which is 0x10 - 0x20 = 0xF0 (which has bit 7 set)
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("CMP_IM less test failed: Negative flag should be set");
    }
}

void inline_cmp_zp_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set up a test value in memory
    mem[0x42] = 0x30;  // Value at zero page address

    // Set a value in the accumulator that is greater than the memory value
    cpu.set(Register::A, 0x50);  // A = 80

    // Inline test for CMP Zero Page instruction
    mem[0xFFFC] = op(Op::CMP_ZP);  // CMP Zero Page
    mem[0xFFFD] = 0x42;            // Zero page address

    bool program_completed = false;
    i32 cycles_used = cpu.execute(3, mem, &program_completed, true);

    // Check flags
    if (cpu.FLAGS_Z || !cpu.FLAGS_C || cpu.FLAGS_N) {
        throw testing::TestFailedException(
            "CMP_ZP test failed: Flags not correctly set for A > Memory (Z should be 0, C should be 1, N should be 0)");
    }
}

void inline_cmp_zpx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set X register value for offset
    cpu.set(Register::X, 0x05);

    // Set up a test value in memory
    mem[0x47] = 0x30;  // Value at effective address (0x42 + 0x05 = 0x47)

    // Set a value in the accumulator that is greater than the memory value
    cpu.set(Register::A, 0x50);  // A = 80

    // Inline test for CMP Zero Page,X instruction
    mem[0xFFFC] = op(Op::CMP_ZPX);  // CMP Zero Page,X
    mem[0xFFFD] = 0x42;             // Zero page base address

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Check flags
    if (cpu.FLAGS_Z || !cpu.FLAGS_C || cpu.FLAGS_N) {
        throw testing::TestFailedException(
            "CMP_ZPX test failed: Flags not correctly set for A > Memory (Z should be 0, C should be 1, N should be "
            "0)");
    }
}

void inline_cmp_abs_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set up a test value in memory
    mem[0x4480] = 0x30;  // Value at absolute address

    // Set a value in the accumulator that is greater than the memory value
    cpu.set(Register::A, 0x50);  // A = 80

    // Inline test for CMP Absolute instruction
    mem[0xFFFC] = op(Op::CMP_AB);  // CMP Absolute
    mem[0xFFFD] = 0x80;            // Low byte of address
    mem[0xFFFE] = 0x44;            // High byte of address (0x4480)

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Check flags
    if (cpu.FLAGS_Z || !cpu.FLAGS_C || cpu.FLAGS_N) {
        throw testing::TestFailedException(
            "CMP_AB test failed: Flags not correctly set for A > Memory (Z should be 0, C should be 1, N should be 0)");
    }
}

void inline_cmp_absx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set X register value for offset
    cpu.set(Register::X, 0x05);

    // Set up a test value in memory
    mem[0x4485] = 0x30;  // Value at effective address (0x4480 + 0x05 = 0x4485)

    // Set a value in the accumulator that is greater than the memory value
    cpu.set(Register::A, 0x50);  // A = 80

    // Inline test for CMP Absolute,X instruction
    mem[0xFFFC] = op(Op::CMP_ABSX);  // CMP Absolute,X
    mem[0xFFFD] = 0x80;              // Low byte of address
    mem[0xFFFE] = 0x44;              // High byte of address (0x4480)

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Check flags
    if (cpu.FLAGS_Z || !cpu.FLAGS_C || cpu.FLAGS_N) {
        throw testing::TestFailedException(
            "CMP_ABSX test failed: Flags not correctly set for A > Memory (Z should be 0, C should be 1, N should be "
            "0)");
    }
}

void inline_cmp_absy_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set Y register value for offset
    cpu.set(Register::Y, 0x06);

    // Set up a test value in memory
    mem[0x4486] = 0x30;  // Value at effective address (0x4480 + 0x06 = 0x4486)

    // Set a value in the accumulator that is greater than the memory value
    cpu.set(Register::A, 0x50);  // A = 80

    // Inline test for CMP Absolute,Y instruction
    mem[0xFFFC] = op(Op::CMP_ABSY);  // CMP Absolute,Y
    mem[0xFFFD] = 0x80;              // Low byte of address
    mem[0xFFFE] = 0x44;              // High byte of address (0x4480)

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Check flags
    if (cpu.FLAGS_Z || !cpu.FLAGS_C || cpu.FLAGS_N) {
        throw testing::TestFailedException(
            "CMP_ABSY test failed: Flags not correctly set for A > Memory (Z should be 0, C should be 1, N should be "
            "0)");
    }
}

void inline_cmp_indx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set X register value
    cpu.set(Register::X, 0x04);

    // Set up the indirect address
    mem[0x24] = 0x74;    // Low byte of target address (0x20 + X = 0x24)
    mem[0x25] = 0x20;    // High byte of target address (0x2074)
    mem[0x2074] = 0x30;  // Value at target address

    // Set a value in the accumulator that is greater than the memory value
    cpu.set(Register::A, 0x50);  // A = 80

    // Inline test for CMP (Indirect,X) instruction
    mem[0xFFFC] = op(Op::CMP_INX);  // CMP (Indirect,X)
    mem[0xFFFD] = 0x20;             // Zero page address

    bool program_completed = false;
    i32 cycles_used = cpu.execute(6, mem, &program_completed, true);

    // Check flags
    if (cpu.FLAGS_Z || !cpu.FLAGS_C || cpu.FLAGS_N) {
        throw testing::TestFailedException(
            "CMP_INX test failed: Flags not correctly set for A > Memory (Z should be 0, C should be 1, N should be "
            "0)");
    }
}

void inline_cmp_indy_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set Y register value
    cpu.set(Register::Y, 0x10);

    // Setup the indirect address
    mem[0x30] = 0x40;    // Low byte of base address
    mem[0x31] = 0x30;    // High byte of base address (0x3040)
    mem[0x3050] = 0x30;  // Value at target address (0x3040 + 0x10 = 0x3050)

    // Set a value in the accumulator that is greater than the memory value
    cpu.set(Register::A, 0x50);  // A = 80

    // Inline test for CMP (Indirect),Y instruction
    mem[0xFFFC] = op(Op::CMP_INY);  // CMP (Indirect),Y
    mem[0xFFFD] = 0x30;             // Zero page address

    bool program_completed = false;
    i32 cycles_used = cpu.execute(5, mem, &program_completed, true);

    // Check flags
    if (cpu.FLAGS_Z || !cpu.FLAGS_C || cpu.FLAGS_N) {
        throw testing::TestFailedException(
            "CMP_INY test failed: Flags not correctly set for A > Memory (Z should be 0, C should be 1, N should be "
            "0)");
    }
}

// CPX Tests
void inline_cpx_im_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Test case 1: X > Memory (should set carry, clear zero)
    cpu.set(Register::X, 0x50);  // X = 80

    mem[0xFFFC] = op(Op::CPX_IM);  // Compare X Immediate
    mem[0xFFFD] = 0x30;            // Compare with 48

    bool program_completed = false;
    i32 cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> CPX_IM result: X=0x%02X, Value=0x%02X, Flags: Z=%d, C=%d, N=%d%s\n", CYAN, cpu.get(Register::X),
                0x30, cpu.FLAGS_Z, cpu.FLAGS_C, cpu.FLAGS_N, RESET);

    // Verify flags are correctly set
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("CPX_IM test failed: Zero flag should not be set when X > Memory");
    }
    if (!cpu.FLAGS_C) {
        throw testing::TestFailedException("CPX_IM test failed: Carry flag should be set when X >= Memory");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("CPX_IM test failed: Negative flag should not be set");
    }

    // Test case 2: X = Memory (should set carry and zero)
    cpu.reset(mem);
    cpu.set(Register::X, 0x30);  // X = 48

    mem[0xFFFC] = op(Op::CPX_IM);
    mem[0xFFFD] = 0x30;  // Compare with 48

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify flags when equal
    if (!cpu.FLAGS_Z) {
        throw testing::TestFailedException("CPX_IM equal test failed: Zero flag should be set when X = Memory");
    }
    if (!cpu.FLAGS_C) {
        throw testing::TestFailedException("CPX_IM equal test failed: Carry flag should be set when X >= Memory");
    }

    // Test case 3: X < Memory (should clear carry and zero, may set negative)
    cpu.reset(mem);
    cpu.set(Register::X, 0x10);  // X = 16

    mem[0xFFFC] = op(Op::CPX_IM);
    mem[0xFFFD] = 0x20;  // Compare with 32

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify flags when less than
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("CPX_IM less test failed: Zero flag should not be set when X < Memory");
    }
    if (cpu.FLAGS_C) {
        throw testing::TestFailedException("CPX_IM less test failed: Carry flag should be clear when X < Memory");
    }
    // Negative flag depends on the result of X - M
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("CPX_IM less test failed: Negative flag should be set");
    }
}

void inline_cpx_zp_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set up a test value in memory
    mem[0x42] = 0x30;  // Value at zero page address

    // Set a value in the X register that is greater than the memory value
    cpu.set(Register::X, 0x50);  // X = 80

    // Inline test for CPX Zero Page instruction
    mem[0xFFFC] = op(Op::CPX_ZP);  // CPX Zero Page
    mem[0xFFFD] = 0x42;            // Zero page address

    bool program_completed = false;
    i32 cycles_used = cpu.execute(3, mem, &program_completed, true);

    // Check flags
    if (cpu.FLAGS_Z || !cpu.FLAGS_C || cpu.FLAGS_N) {
        throw testing::TestFailedException(
            "CPX_ZP test failed: Flags not correctly set for X > Memory (Z should be 0, C should be 1, N should be 0)");
    }
}

void inline_cpx_abs_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set up a test value in memory
    mem[0x4480] = 0x30;  // Value at absolute address

    // Set a value in the X register that is greater than the memory value
    cpu.set(Register::X, 0x50);  // X = 80

    // Inline test for CPX Absolute instruction
    mem[0xFFFC] = op(Op::CPX_AB);  // CPX Absolute
    mem[0xFFFD] = 0x80;            // Low byte of address
    mem[0xFFFE] = 0x44;            // High byte of address (0x4480)

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Check flags
    if (cpu.FLAGS_Z || !cpu.FLAGS_C || cpu.FLAGS_N) {
        throw testing::TestFailedException(
            "CPX_AB test failed: Flags not correctly set for X > Memory (Z should be 0, C should be 1, N should be 0)");
    }
}

// CPY Tests
void inline_cpy_im_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Test case 1: Y > Memory (should set carry, clear zero)
    cpu.set(Register::Y, 0x50);  // Y = 80

    mem[0xFFFC] = op(Op::CPY_IM);  // Compare Y Immediate
    mem[0xFFFD] = 0x30;            // Compare with 48

    bool program_completed = false;
    i32 cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> CPY_IM result: Y=0x%02X, Value=0x%02X, Flags: Z=%d, C=%d, N=%d%s\n", CYAN, cpu.get(Register::Y),
                0x30, cpu.FLAGS_Z, cpu.FLAGS_C, cpu.FLAGS_N, RESET);

    // Verify flags are correctly set
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("CPY_IM test failed: Zero flag should not be set when Y > Memory");
    }
    if (!cpu.FLAGS_C) {
        throw testing::TestFailedException("CPY_IM test failed: Carry flag should be set when Y >= Memory");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("CPY_IM test failed: Negative flag should not be set");
    }

    // Test case 2: Y = Memory (should set carry and zero)
    cpu.reset(mem);
    cpu.set(Register::Y, 0x30);  // Y = 48

    mem[0xFFFC] = op(Op::CPY_IM);
    mem[0xFFFD] = 0x30;  // Compare with 48

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify flags when equal
    if (!cpu.FLAGS_Z) {
        throw testing::TestFailedException("CPY_IM equal test failed: Zero flag should be set when Y = Memory");
    }
    if (!cpu.FLAGS_C) {
        throw testing::TestFailedException("CPY_IM equal test failed: Carry flag should be set when Y >= Memory");
    }

    // Test case 3: Y < Memory (should clear carry and zero, may set negative)
    cpu.reset(mem);
    cpu.set(Register::Y, 0x10);  // Y = 16

    mem[0xFFFC] = op(Op::CPY_IM);
    mem[0xFFFD] = 0x20;  // Compare with 32

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify flags when less than
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("CPY_IM less test failed: Zero flag should not be set when Y < Memory");
    }
    if (cpu.FLAGS_C) {
        throw testing::TestFailedException("CPY_IM less test failed: Carry flag should be clear when Y < Memory");
    }
    // Negative flag depends on the result of Y - M
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("CPY_IM less test failed: Negative flag should be set");
    }
}

void inline_cpy_zp_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set up a test value in memory
    mem[0x42] = 0x30;  // Value at zero page address

    // Set a value in the Y register that is greater than the memory value
    cpu.set(Register::Y, 0x50);  // Y = 80

    // Inline test for CPY Zero Page instruction
    mem[0xFFFC] = op(Op::CPY_ZP);  // CPY Zero Page
    mem[0xFFFD] = 0x42;            // Zero page address

    bool program_completed = false;
    i32 cycles_used = cpu.execute(3, mem, &program_completed, true);

    // Check flags
    if (cpu.FLAGS_Z || !cpu.FLAGS_C || cpu.FLAGS_N) {
        throw testing::TestFailedException(
            "CPY_ZP test failed: Flags not correctly set for Y > Memory (Z should be 0, C should be 1, N should be 0)");
    }
}

void inline_cpy_abs_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set up a test value in memory
    mem[0x4480] = 0x30;  // Value at absolute address

    // Set a value in the Y register that is greater than the memory value
    cpu.set(Register::Y, 0x50);  // Y = 80

    // Inline test for CPY Absolute instruction
    mem[0xFFFC] = op(Op::CPY_AB);  // CPY Absolute
    mem[0xFFFD] = 0x80;            // Low byte of address
    mem[0xFFFE] = 0x44;            // High byte of address (0x4480)

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Check flags
    if (cpu.FLAGS_Z || !cpu.FLAGS_C || cpu.FLAGS_N) {
        throw testing::TestFailedException(
            "CPY_AB test failed: Flags not correctly set for Y > Memory (Z should be 0, C should be 1, N should be 0)");
    }
}

// Test suite function for compare operations
void compare_test_suite(Cpu& cpu, Mem& mem) {
    testing::TestSuite test_suite("Compare Operations");

    test_suite.print_header();

    // Register and run CMP tests
    test_suite.register_test("CMP Immediate", [&]() { inline_cmp_im_test(cpu, mem); });
    test_suite.register_test("CMP Zero Page", [&]() { inline_cmp_zp_test(cpu, mem); });
    test_suite.register_test("CMP Zero Page,X", [&]() { inline_cmp_zpx_test(cpu, mem); });
    test_suite.register_test("CMP Absolute", [&]() { inline_cmp_abs_test(cpu, mem); });
    test_suite.register_test("CMP Absolute,X", [&]() { inline_cmp_absx_test(cpu, mem); });
    test_suite.register_test("CMP Absolute,Y", [&]() { inline_cmp_absy_test(cpu, mem); });
    test_suite.register_test("CMP (Indirect,X)", [&]() { inline_cmp_indx_test(cpu, mem); });
    test_suite.register_test("CMP (Indirect),Y", [&]() { inline_cmp_indy_test(cpu, mem); });

    // Register and run CPX tests
    test_suite.register_test("CPX Immediate", [&]() { inline_cpx_im_test(cpu, mem); });
    test_suite.register_test("CPX Zero Page", [&]() { inline_cpx_zp_test(cpu, mem); });
    test_suite.register_test("CPX Absolute", [&]() { inline_cpx_abs_test(cpu, mem); });

    // Register and run CPY tests
    test_suite.register_test("CPY Immediate", [&]() { inline_cpy_im_test(cpu, mem); });
    test_suite.register_test("CPY Zero Page", [&]() { inline_cpy_zp_test(cpu, mem); });
    test_suite.register_test("CPY Absolute", [&]() { inline_cpy_abs_test(cpu, mem); });

    // Print the results
    test_suite.print_results();
}

}  // namespace testing
