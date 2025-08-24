#include "cpu.h"
#include "memory.h"
#include "op_codes.h"
#include "test_utils.h"

using namespace colors;

namespace testing {

// INC Tests
void inline_inc_zp_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set up memory with a test value
    mem[0x42] = 0x34;  // Value to increment

    // Inline test for INC Zero Page instruction
    mem[0xFFFC] = op(Op::INC_ZP);  // INC Zero Page
    mem[0xFFFD] = 0x42;            // Zero page address
    mem[0xFFFE] = op(Op::NOP);     // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(5, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Memory at zero page 0x42 after INC_ZP: 0x%02X%s\n", CYAN, mem[0x42], RESET);

    // Verify with test assertion - result should be 0x35 (53)
    if (mem[0x42] != 0x35) {
        throw testing::TestFailedException("INC_ZP test failed: Memory at 0x42 should be 0x35");
    }

    // Verify flags are correctly set
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("INC_ZP test failed: Zero flag should not be set");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("INC_ZP test failed: Negative flag should not be set");
    }

    // Test increment of 0xFF (should roll over to 0x00 and set zero flag)
    cpu.reset(mem);
    mem[0x42] = 0xFF;

    mem[0xFFFC] = op(Op::INC_ZP);
    mem[0xFFFD] = 0x42;

    program_completed = false;
    cycles_used = cpu.execute(5, mem, &program_completed, true);

    // Verify result is 0x00
    if (mem[0x42] != 0x00) {
        throw testing::TestFailedException("INC_ZP rollover test failed: Memory at 0x42 should be 0x00");
    }

    // Verify Zero flag is set
    if (!cpu.FLAGS_Z) {
        throw testing::TestFailedException("INC_ZP rollover test failed: Zero flag should be set");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("INC_ZP rollover test failed: Negative flag should not be set");
    }

    // Test increment to a negative value (bit 7 set)
    cpu.reset(mem);
    mem[0x42] = 0x7F;

    mem[0xFFFC] = op(Op::INC_ZP);
    mem[0xFFFD] = 0x42;

    program_completed = false;
    cycles_used = cpu.execute(5, mem, &program_completed, true);

    // Verify result is 0x80
    if (mem[0x42] != 0x80) {
        throw testing::TestFailedException("INC_ZP negative test failed: Memory at 0x42 should be 0x80");
    }

    // Verify Negative flag is set
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("INC_ZP negative test failed: Negative flag should be set");
    }
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("INC_ZP negative test failed: Zero flag should not be set");
    }
}

void inline_inc_zpx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set X register value for offset
    cpu.set(Register::X, 0x05);

    // Set up memory with a test value
    mem[0x47] = 0x34;  // Value to increment at effective address (0x42 + 0x05 = 0x47)

    // Inline test for INC Zero Page,X instruction
    mem[0xFFFC] = op(Op::INC_ZPX);  // INC Zero Page,X
    mem[0xFFFD] = 0x42;             // Zero page base address
    mem[0xFFFE] = op(Op::NOP);      // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(6, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Memory at zero page 0x47 after INC_ZPX: 0x%02X%s\n", CYAN, mem[0x47], RESET);

    // Verify with test assertion - result should be 0x35 (53)
    if (mem[0x47] != 0x35) {
        throw testing::TestFailedException("INC_ZPX test failed: Memory at 0x47 should be 0x35");
    }
}

void inline_inc_abs_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set up memory with a test value
    mem[0x4480] = 0x34;  // Value to increment

    // Inline test for INC Absolute instruction
    mem[0xFFFC] = op(Op::INC_AB);  // INC Absolute
    mem[0xFFFD] = 0x80;            // Low byte of address
    mem[0xFFFE] = 0x44;            // High byte of address (0x4480)
    mem[0xFFFF] = op(Op::NOP);     // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(6, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Memory at absolute 0x4480 after INC_AB: 0x%02X%s\n", CYAN, mem[0x4480], RESET);

    // Verify with test assertion - result should be 0x35 (53)
    if (mem[0x4480] != 0x35) {
        throw testing::TestFailedException("INC_AB test failed: Memory at 0x4480 should be 0x35");
    }
}

void inline_inc_absx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set X register value for offset
    cpu.set(Register::X, 0x05);

    // Set up memory with a test value
    mem[0x4485] = 0x34;  // Value to increment at effective address (0x4480 + 0x05 = 0x4485)

    // Inline test for INC Absolute,X instruction
    mem[0xFFFC] = op(Op::INC_ABSX);  // INC Absolute,X
    mem[0xFFFD] = 0x80;              // Low byte of address
    mem[0xFFFE] = 0x44;              // High byte of address (0x4480)
    mem[0xFFFF] = op(Op::NOP);       // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(7, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Memory at absolute 0x4485 after INC_ABSX: 0x%02X%s\n", CYAN, mem[0x4485], RESET);

    // Verify with test assertion - result should be 0x35 (53)
    if (mem[0x4485] != 0x35) {
        throw testing::TestFailedException("INC_ABSX test failed: Memory at 0x4485 should be 0x35");
    }
}

void inline_inx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set a known value in X register
    cpu.set(Register::X, 0x42);

    // Inline test for INX instruction
    mem[0xFFFC] = op(Op::INX);  // INX (Increment X)
    mem[0xFFFD] = op(Op::NOP);  // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> X register after INX: 0x%02X%s\n", CYAN, cpu.get(Register::X), RESET);

    // Verify with test assertion - result should be 0x43 (67)
    if (cpu.get(Register::X) != 0x43) {
        throw testing::TestFailedException("INX test failed: X register should be 0x43");
    }

    // Test increment of 0xFF (should roll over to 0x00 and set zero flag)
    cpu.reset(mem);
    cpu.set(Register::X, 0xFF);

    mem[0xFFFC] = op(Op::INX);

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify result is 0x00
    if (cpu.get(Register::X) != 0x00) {
        throw testing::TestFailedException("INX rollover test failed: X register should be 0x00");
    }

    // Verify Zero flag is set
    if (!cpu.FLAGS_Z) {
        throw testing::TestFailedException("INX rollover test failed: Zero flag should be set");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("INX rollover test failed: Negative flag should not be set");
    }

    // Test increment to a negative value (bit 7 set)
    cpu.reset(mem);
    cpu.set(Register::X, 0x7F);

    mem[0xFFFC] = op(Op::INX);

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify result is 0x80
    if (cpu.get(Register::X) != 0x80) {
        throw testing::TestFailedException("INX negative test failed: X register should be 0x80");
    }

    // Verify Negative flag is set
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("INX negative test failed: Negative flag should be set");
    }
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("INX negative test failed: Zero flag should not be set");
    }
}

void inline_iny_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set a known value in Y register
    cpu.set(Register::Y, 0x42);

    // Inline test for INY instruction
    mem[0xFFFC] = op(Op::INY);  // INY (Increment Y)
    mem[0xFFFD] = op(Op::NOP);  // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Y register after INY: 0x%02X%s\n", CYAN, cpu.get(Register::Y), RESET);

    // Verify with test assertion - result should be 0x43 (67)
    if (cpu.get(Register::Y) != 0x43) {
        throw testing::TestFailedException("INY test failed: Y register should be 0x43");
    }

    // Test increment of 0xFF (should roll over to 0x00 and set zero flag)
    cpu.reset(mem);
    cpu.set(Register::Y, 0xFF);

    mem[0xFFFC] = op(Op::INY);

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify result is 0x00
    if (cpu.get(Register::Y) != 0x00) {
        throw testing::TestFailedException("INY rollover test failed: Y register should be 0x00");
    }

    // Verify Zero flag is set
    if (!cpu.FLAGS_Z) {
        throw testing::TestFailedException("INY rollover test failed: Zero flag should be set");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("INY rollover test failed: Negative flag should not be set");
    }

    // Test increment to a negative value (bit 7 set)
    cpu.reset(mem);
    cpu.set(Register::Y, 0x7F);

    mem[0xFFFC] = op(Op::INY);

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify result is 0x80
    if (cpu.get(Register::Y) != 0x80) {
        throw testing::TestFailedException("INY negative test failed: Y register should be 0x80");
    }

    // Verify Negative flag is set
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("INY negative test failed: Negative flag should be set");
    }
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("INY negative test failed: Zero flag should not be set");
    }
}

// DEC Tests
void inline_dec_zp_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set up memory with a test value
    mem[0x42] = 0x34;  // Value to decrement

    // Inline test for DEC Zero Page instruction
    mem[0xFFFC] = op(Op::DEC_ZP);  // DEC Zero Page
    mem[0xFFFD] = 0x42;            // Zero page address
    mem[0xFFFE] = op(Op::NOP);     // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(5, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Memory at zero page 0x42 after DEC_ZP: 0x%02X%s\n", CYAN, mem[0x42], RESET);

    // Verify with test assertion - result should be 0x33 (51)
    if (mem[0x42] != 0x33) {
        throw testing::TestFailedException("DEC_ZP test failed: Memory at 0x42 should be 0x33");
    }

    // Verify flags are correctly set
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("DEC_ZP test failed: Zero flag should not be set");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("DEC_ZP test failed: Negative flag should not be set");
    }

    // Test decrement of 0x00 (should roll over to 0xFF and set negative flag)
    cpu.reset(mem);
    mem[0x42] = 0x00;

    mem[0xFFFC] = op(Op::DEC_ZP);
    mem[0xFFFD] = 0x42;

    program_completed = false;
    cycles_used = cpu.execute(5, mem, &program_completed, true);

    // Verify result is 0xFF
    if (mem[0x42] != 0xFF) {
        throw testing::TestFailedException("DEC_ZP rollover test failed: Memory at 0x42 should be 0xFF");
    }

    // Verify Negative flag is set
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("DEC_ZP rollover test failed: Negative flag should be set");
    }
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("DEC_ZP rollover test failed: Zero flag should not be set");
    }

    // Test decrement to zero
    cpu.reset(mem);
    mem[0x42] = 0x01;

    mem[0xFFFC] = op(Op::DEC_ZP);
    mem[0xFFFD] = 0x42;

    program_completed = false;
    cycles_used = cpu.execute(5, mem, &program_completed, true);

    // Verify result is 0x00
    if (mem[0x42] != 0x00) {
        throw testing::TestFailedException("DEC_ZP zero test failed: Memory at 0x42 should be 0x00");
    }

    // Verify Zero flag is set
    if (!cpu.FLAGS_Z) {
        throw testing::TestFailedException("DEC_ZP zero test failed: Zero flag should be set");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("DEC_ZP zero test failed: Negative flag should not be set");
    }
}

void inline_dec_zpx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set X register value for offset
    cpu.set(Register::X, 0x05);

    // Set up memory with a test value
    mem[0x47] = 0x34;  // Value to decrement at effective address (0x42 + 0x05 = 0x47)

    // Inline test for DEC Zero Page,X instruction
    mem[0xFFFC] = op(Op::DEC_ZPX);  // DEC Zero Page,X
    mem[0xFFFD] = 0x42;             // Zero page base address
    mem[0xFFFE] = op(Op::NOP);      // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(6, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Memory at zero page 0x47 after DEC_ZPX: 0x%02X%s\n", CYAN, mem[0x47], RESET);

    // Verify with test assertion - result should be 0x33 (51)
    if (mem[0x47] != 0x33) {
        throw testing::TestFailedException("DEC_ZPX test failed: Memory at 0x47 should be 0x33");
    }
}

void inline_dec_abs_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set up memory with a test value
    mem[0x4480] = 0x34;  // Value to decrement

    // Inline test for DEC Absolute instruction
    mem[0xFFFC] = op(Op::DEC_AB);  // DEC Absolute
    mem[0xFFFD] = 0x80;            // Low byte of address
    mem[0xFFFE] = 0x44;            // High byte of address (0x4480)
    mem[0xFFFF] = op(Op::NOP);     // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(6, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Memory at absolute 0x4480 after DEC_AB: 0x%02X%s\n", CYAN, mem[0x4480], RESET);

    // Verify with test assertion - result should be 0x33 (51)
    if (mem[0x4480] != 0x33) {
        throw testing::TestFailedException("DEC_AB test failed: Memory at 0x4480 should be 0x33");
    }
}

void inline_dec_absx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set X register value for offset
    cpu.set(Register::X, 0x05);

    // Set up memory with a test value
    mem[0x4485] = 0x34;  // Value to decrement at effective address (0x4480 + 0x05 = 0x4485)

    // Inline test for DEC Absolute,X instruction
    mem[0xFFFC] = op(Op::DEC_ABSX);  // DEC Absolute,X
    mem[0xFFFD] = 0x80;              // Low byte of address
    mem[0xFFFE] = 0x44;              // High byte of address (0x4480)
    mem[0xFFFF] = op(Op::NOP);       // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(7, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Memory at absolute 0x4485 after DEC_ABSX: 0x%02X%s\n", CYAN, mem[0x4485], RESET);

    // Verify with test assertion - result should be 0x33 (51)
    if (mem[0x4485] != 0x33) {
        throw testing::TestFailedException("DEC_ABSX test failed: Memory at 0x4485 should be 0x33");
    }
}

void inline_dex_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set a known value in X register
    cpu.set(Register::X, 0x42);

    // Inline test for DEX instruction
    mem[0xFFFC] = op(Op::DEX);  // DEX (Decrement X)
    mem[0xFFFD] = op(Op::NOP);  // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> X register after DEX: 0x%02X%s\n", CYAN, cpu.get(Register::X), RESET);

    // Verify with test assertion - result should be 0x41 (65)
    if (cpu.get(Register::X) != 0x41) {
        throw testing::TestFailedException("DEX test failed: X register should be 0x41");
    }

    // Test decrement of 0x00 (should roll over to 0xFF and set negative flag)
    cpu.reset(mem);
    cpu.set(Register::X, 0x00);

    mem[0xFFFC] = op(Op::DEX);

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify result is 0xFF
    if (cpu.get(Register::X) != 0xFF) {
        throw testing::TestFailedException("DEX rollover test failed: X register should be 0xFF");
    }

    // Verify Negative flag is set
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("DEX rollover test failed: Negative flag should be set");
    }
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("DEX rollover test failed: Zero flag should not be set");
    }

    // Test decrement to zero
    cpu.reset(mem);
    cpu.set(Register::X, 0x01);

    mem[0xFFFC] = op(Op::DEX);

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify result is 0x00
    if (cpu.get(Register::X) != 0x00) {
        throw testing::TestFailedException("DEX zero test failed: X register should be 0x00");
    }

    // Verify Zero flag is set
    if (!cpu.FLAGS_Z) {
        throw testing::TestFailedException("DEX zero test failed: Zero flag should be set");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("DEX zero test failed: Negative flag should not be set");
    }
}

void inline_dey_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set a known value in Y register
    cpu.set(Register::Y, 0x42);

    // Inline test for DEY instruction
    mem[0xFFFC] = op(Op::DEY);  // DEY (Decrement Y)
    mem[0xFFFD] = op(Op::NOP);  // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Y register after DEY: 0x%02X%s\n", CYAN, cpu.get(Register::Y), RESET);

    // Verify with test assertion - result should be 0x41 (65)
    if (cpu.get(Register::Y) != 0x41) {
        throw testing::TestFailedException("DEY test failed: Y register should be 0x41");
    }

    // Test decrement of 0x00 (should roll over to 0xFF and set negative flag)
    cpu.reset(mem);
    cpu.set(Register::Y, 0x00);

    mem[0xFFFC] = op(Op::DEY);

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify result is 0xFF
    if (cpu.get(Register::Y) != 0xFF) {
        throw testing::TestFailedException("DEY rollover test failed: Y register should be 0xFF");
    }

    // Verify Negative flag is set
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("DEY rollover test failed: Negative flag should be set");
    }
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("DEY rollover test failed: Zero flag should not be set");
    }

    // Test decrement to zero
    cpu.reset(mem);
    cpu.set(Register::Y, 0x01);

    mem[0xFFFC] = op(Op::DEY);

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify result is 0x00
    if (cpu.get(Register::Y) != 0x00) {
        throw testing::TestFailedException("DEY zero test failed: Y register should be 0x00");
    }

    // Verify Zero flag is set
    if (!cpu.FLAGS_Z) {
        throw testing::TestFailedException("DEY zero test failed: Zero flag should be set");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("DEY zero test failed: Negative flag should not be set");
    }
}

// Test suite function for increment and decrement operations
void increment_decrement_test_suite(Cpu& cpu, Mem& mem) {
    testing::TestSuite test_suite("Increment and Decrement Operations");

    test_suite.print_header();

    // Register and run Increment tests
    test_suite.register_test("INC Zero Page", [&]() { inline_inc_zp_test(cpu, mem); });
    test_suite.register_test("INC Zero Page,X", [&]() { inline_inc_zpx_test(cpu, mem); });
    test_suite.register_test("INC Absolute", [&]() { inline_inc_abs_test(cpu, mem); });
    test_suite.register_test("INC Absolute,X", [&]() { inline_inc_absx_test(cpu, mem); });
    test_suite.register_test("INX", [&]() { inline_inx_test(cpu, mem); });
    test_suite.register_test("INY", [&]() { inline_iny_test(cpu, mem); });

    // Register and run Decrement tests
    test_suite.register_test("DEC Zero Page", [&]() { inline_dec_zp_test(cpu, mem); });
    test_suite.register_test("DEC Zero Page,X", [&]() { inline_dec_zpx_test(cpu, mem); });
    test_suite.register_test("DEC Absolute", [&]() { inline_dec_abs_test(cpu, mem); });
    test_suite.register_test("DEC Absolute,X", [&]() { inline_dec_absx_test(cpu, mem); });
    test_suite.register_test("DEX", [&]() { inline_dex_test(cpu, mem); });
    test_suite.register_test("DEY", [&]() { inline_dey_test(cpu, mem); });

    // Print the results
    test_suite.print_results();
}

}  // namespace testing
