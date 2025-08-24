#include "cpu.h"
#include "memory.h"
#include "op_codes.h"
#include "test_utils.h"

using namespace colors;

namespace testing {

// Test for CLC (Clear Carry Flag)
void inline_clc_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set carry flag to ensure it gets cleared
    cpu.FLAGS_C = 1;

    // Verify carry flag is set before the operation
    if (!cpu.FLAGS_C) {
        throw testing::TestFailedException("CLC test setup failed: Carry flag should be set before testing");
    }

    // Load CLC instruction
    mem[0xFFFC] = op(Op::CLC);
    mem[0xFFFD] = op(Op::NOP);  // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(2, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Verify carry flag is cleared
    if (cpu.FLAGS_C) {
        throw testing::TestFailedException("CLC test failed: Carry flag should be cleared");
    }

    // Make sure other flags are not affected
    bool original_n = cpu.FLAGS_N;
    bool original_v = cpu.FLAGS_V;
    bool original_z = cpu.FLAGS_Z;

    // Run CLC again to ensure flags don't change
    mem[0xFFFC] = op(Op::CLC);
    cpu.PC = 0xFFFC;  // Reset PC

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify other flags are not affected
    if (cpu.FLAGS_N != original_n) {
        throw testing::TestFailedException("CLC test failed: Negative flag shouldn't be affected");
    }
    if (cpu.FLAGS_V != original_v) {
        throw testing::TestFailedException("CLC test failed: Overflow flag shouldn't be affected");
    }
    if (cpu.FLAGS_Z != original_z) {
        throw testing::TestFailedException("CLC test failed: Zero flag shouldn't be affected");
    }
}

// Test for CLD (Clear Decimal Mode Flag)
void inline_cld_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set decimal flag to ensure it gets cleared
    cpu.FLAGS_D = 1;

    // Verify decimal flag is set before the operation
    if (!cpu.FLAGS_D) {
        throw testing::TestFailedException("CLD test setup failed: Decimal flag should be set before testing");
    }

    // Load CLD instruction
    mem[0xFFFC] = op(Op::CLD);
    mem[0xFFFD] = op(Op::NOP);  // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(2, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Verify decimal flag is cleared
    if (cpu.FLAGS_D) {
        throw testing::TestFailedException("CLD test failed: Decimal flag should be cleared");
    }

    // Make sure other flags are not affected
    bool original_n = cpu.FLAGS_N;
    bool original_v = cpu.FLAGS_V;
    bool original_z = cpu.FLAGS_Z;
    bool original_c = cpu.FLAGS_C;

    // Run CLD again to ensure flags don't change
    mem[0xFFFC] = op(Op::CLD);
    cpu.PC = 0xFFFC;  // Reset PC

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify other flags are not affected
    if (cpu.FLAGS_N != original_n) {
        throw testing::TestFailedException("CLD test failed: Negative flag shouldn't be affected");
    }
    if (cpu.FLAGS_V != original_v) {
        throw testing::TestFailedException("CLD test failed: Overflow flag shouldn't be affected");
    }
    if (cpu.FLAGS_Z != original_z) {
        throw testing::TestFailedException("CLD test failed: Zero flag shouldn't be affected");
    }
    if (cpu.FLAGS_C != original_c) {
        throw testing::TestFailedException("CLD test failed: Carry flag shouldn't be affected");
    }
}

// Test for CLI (Clear Interrupt Disable Flag)
void inline_cli_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set interrupt disable flag to ensure it gets cleared
    cpu.FLAGS_I = 1;

    // Verify interrupt disable flag is set before the operation
    if (!cpu.FLAGS_I) {
        throw testing::TestFailedException(
            "CLI test setup failed: Interrupt disable flag should be set before testing");
    }

    // Load CLI instruction
    mem[0xFFFC] = op(Op::CLI);
    mem[0xFFFD] = op(Op::NOP);  // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(2, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Verify interrupt disable flag is cleared
    if (cpu.FLAGS_I) {
        throw testing::TestFailedException("CLI test failed: Interrupt disable flag should be cleared");
    }

    // Make sure other flags are not affected
    bool original_n = cpu.FLAGS_N;
    bool original_v = cpu.FLAGS_V;
    bool original_z = cpu.FLAGS_Z;
    bool original_c = cpu.FLAGS_C;
    bool original_d = cpu.FLAGS_D;

    // Run CLI again to ensure flags don't change
    mem[0xFFFC] = op(Op::CLI);
    cpu.PC = 0xFFFC;  // Reset PC

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify other flags are not affected
    if (cpu.FLAGS_N != original_n) {
        throw testing::TestFailedException("CLI test failed: Negative flag shouldn't be affected");
    }
    if (cpu.FLAGS_V != original_v) {
        throw testing::TestFailedException("CLI test failed: Overflow flag shouldn't be affected");
    }
    if (cpu.FLAGS_Z != original_z) {
        throw testing::TestFailedException("CLI test failed: Zero flag shouldn't be affected");
    }
    if (cpu.FLAGS_C != original_c) {
        throw testing::TestFailedException("CLI test failed: Carry flag shouldn't be affected");
    }
    if (cpu.FLAGS_D != original_d) {
        throw testing::TestFailedException("CLI test failed: Decimal flag shouldn't be affected");
    }
}

// Test for CLV (Clear Overflow Flag)
void inline_clv_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set overflow flag to ensure it gets cleared
    cpu.FLAGS_V = 1;

    // Verify overflow flag is set before the operation
    if (!cpu.FLAGS_V) {
        throw testing::TestFailedException("CLV test setup failed: Overflow flag should be set before testing");
    }

    // Load CLV instruction
    mem[0xFFFC] = op(Op::CLV);
    mem[0xFFFD] = op(Op::NOP);  // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(2, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Verify overflow flag is cleared
    if (cpu.FLAGS_V) {
        throw testing::TestFailedException("CLV test failed: Overflow flag should be cleared");
    }

    // Make sure other flags are not affected
    bool original_n = cpu.FLAGS_N;
    bool original_z = cpu.FLAGS_Z;
    bool original_c = cpu.FLAGS_C;
    bool original_d = cpu.FLAGS_D;
    bool original_i = cpu.FLAGS_I;

    // Run CLV again to ensure flags don't change
    mem[0xFFFC] = op(Op::CLV);
    cpu.PC = 0xFFFC;  // Reset PC

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify other flags are not affected
    if (cpu.FLAGS_N != original_n) {
        throw testing::TestFailedException("CLV test failed: Negative flag shouldn't be affected");
    }
    if (cpu.FLAGS_Z != original_z) {
        throw testing::TestFailedException("CLV test failed: Zero flag shouldn't be affected");
    }
    if (cpu.FLAGS_C != original_c) {
        throw testing::TestFailedException("CLV test failed: Carry flag shouldn't be affected");
    }
    if (cpu.FLAGS_D != original_d) {
        throw testing::TestFailedException("CLV test failed: Decimal flag shouldn't be affected");
    }
    if (cpu.FLAGS_I != original_i) {
        throw testing::TestFailedException("CLV test failed: Interrupt flag shouldn't be affected");
    }
}

// Test for SEC (Set Carry Flag)
void inline_sec_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Clear carry flag to ensure it gets set
    cpu.FLAGS_C = 0;

    // Verify carry flag is cleared before the operation
    if (cpu.FLAGS_C) {
        throw testing::TestFailedException("SEC test setup failed: Carry flag should be cleared before testing");
    }

    // Load SEC instruction
    mem[0xFFFC] = op(Op::SEC);
    mem[0xFFFD] = op(Op::NOP);  // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(2, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Verify carry flag is set
    if (!cpu.FLAGS_C) {
        throw testing::TestFailedException("SEC test failed: Carry flag should be set");
    }

    // Make sure other flags are not affected
    bool original_n = cpu.FLAGS_N;
    bool original_v = cpu.FLAGS_V;
    bool original_z = cpu.FLAGS_Z;
    bool original_d = cpu.FLAGS_D;
    bool original_i = cpu.FLAGS_I;

    // Run SEC again to ensure flags don't change (except carry)
    mem[0xFFFC] = op(Op::SEC);
    cpu.PC = 0xFFFC;  // Reset PC

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify other flags are not affected
    if (cpu.FLAGS_N != original_n) {
        throw testing::TestFailedException("SEC test failed: Negative flag shouldn't be affected");
    }
    if (cpu.FLAGS_V != original_v) {
        throw testing::TestFailedException("SEC test failed: Overflow flag shouldn't be affected");
    }
    if (cpu.FLAGS_Z != original_z) {
        throw testing::TestFailedException("SEC test failed: Zero flag shouldn't be affected");
    }
    if (cpu.FLAGS_D != original_d) {
        throw testing::TestFailedException("SEC test failed: Decimal flag shouldn't be affected");
    }
    if (cpu.FLAGS_I != original_i) {
        throw testing::TestFailedException("SEC test failed: Interrupt flag shouldn't be affected");
    }
}

// Test for SED (Set Decimal Mode Flag)
void inline_sed_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Clear decimal flag to ensure it gets set
    cpu.FLAGS_D = 0;

    // Verify decimal flag is cleared before the operation
    if (cpu.FLAGS_D) {
        throw testing::TestFailedException("SED test setup failed: Decimal flag should be cleared before testing");
    }

    // Load SED instruction
    mem[0xFFFC] = op(Op::SED);
    mem[0xFFFD] = op(Op::NOP);  // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(2, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Verify decimal flag is set
    if (!cpu.FLAGS_D) {
        throw testing::TestFailedException("SED test failed: Decimal flag should be set");
    }

    // Make sure other flags are not affected
    bool original_n = cpu.FLAGS_N;
    bool original_v = cpu.FLAGS_V;
    bool original_z = cpu.FLAGS_Z;
    bool original_c = cpu.FLAGS_C;
    bool original_i = cpu.FLAGS_I;

    // Run SED again to ensure flags don't change (except decimal)
    mem[0xFFFC] = op(Op::SED);
    cpu.PC = 0xFFFC;  // Reset PC

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify other flags are not affected
    if (cpu.FLAGS_N != original_n) {
        throw testing::TestFailedException("SED test failed: Negative flag shouldn't be affected");
    }
    if (cpu.FLAGS_V != original_v) {
        throw testing::TestFailedException("SED test failed: Overflow flag shouldn't be affected");
    }
    if (cpu.FLAGS_Z != original_z) {
        throw testing::TestFailedException("SED test failed: Zero flag shouldn't be affected");
    }
    if (cpu.FLAGS_C != original_c) {
        throw testing::TestFailedException("SED test failed: Carry flag shouldn't be affected");
    }
    if (cpu.FLAGS_I != original_i) {
        throw testing::TestFailedException("SED test failed: Interrupt flag shouldn't be affected");
    }
}

// Test for SEI (Set Interrupt Disable Flag)
void inline_sei_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Clear interrupt disable flag to ensure it gets set
    cpu.FLAGS_I = 0;

    // Verify interrupt disable flag is cleared before the operation
    if (cpu.FLAGS_I) {
        throw testing::TestFailedException(
            "SEI test setup failed: Interrupt disable flag should be cleared before testing");
    }

    // Load SEI instruction
    mem[0xFFFC] = op(Op::SEI);
    mem[0xFFFD] = op(Op::NOP);  // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(2, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Verify interrupt disable flag is set
    if (!cpu.FLAGS_I) {
        throw testing::TestFailedException("SEI test failed: Interrupt disable flag should be set");
    }

    // Make sure other flags are not affected
    bool original_n = cpu.FLAGS_N;
    bool original_v = cpu.FLAGS_V;
    bool original_z = cpu.FLAGS_Z;
    bool original_c = cpu.FLAGS_C;
    bool original_d = cpu.FLAGS_D;

    // Run SEI again to ensure flags don't change (except interrupt)
    mem[0xFFFC] = op(Op::SEI);
    cpu.PC = 0xFFFC;  // Reset PC

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify other flags are not affected
    if (cpu.FLAGS_N != original_n) {
        throw testing::TestFailedException("SEI test failed: Negative flag shouldn't be affected");
    }
    if (cpu.FLAGS_V != original_v) {
        throw testing::TestFailedException("SEI test failed: Overflow flag shouldn't be affected");
    }
    if (cpu.FLAGS_Z != original_z) {
        throw testing::TestFailedException("SEI test failed: Zero flag shouldn't be affected");
    }
    if (cpu.FLAGS_C != original_c) {
        throw testing::TestFailedException("SEI test failed: Carry flag shouldn't be affected");
    }
    if (cpu.FLAGS_D != original_d) {
        throw testing::TestFailedException("SEI test failed: Decimal flag shouldn't be affected");
    }
}

// Test suite function for status flag operations
void status_flag_test_suite(Cpu& cpu, Mem& mem) {
    testing::TestSuite test_suite("Status Flag Operations");

    test_suite.print_header();

    // Register and run individual tests
    test_suite.register_test("Clear Carry Flag (CLC)", [&]() { inline_clc_test(cpu, mem); });
    test_suite.register_test("Clear Decimal Mode Flag (CLD)", [&]() { inline_cld_test(cpu, mem); });
    test_suite.register_test("Clear Interrupt Disable Flag (CLI)", [&]() { inline_cli_test(cpu, mem); });
    test_suite.register_test("Clear Overflow Flag (CLV)", [&]() { inline_clv_test(cpu, mem); });
    test_suite.register_test("Set Carry Flag (SEC)", [&]() { inline_sec_test(cpu, mem); });
    test_suite.register_test("Set Decimal Mode Flag (SED)", [&]() { inline_sed_test(cpu, mem); });
    test_suite.register_test("Set Interrupt Disable Flag (SEI)", [&]() { inline_sei_test(cpu, mem); });

    // Print the results
    test_suite.print_results();
}

}  // namespace testing
