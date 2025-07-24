#include <iomanip>  // For std::hex, std::setw, etc.
#include <iostream>

#include "cpu.h"
#include "memory.h"
#include "op_codes.h"
#include "test_utils.h"
#include "types.h"

using namespace colors;  // Make color codes more accessible

namespace testing {
void inline_lda_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Inline test for LDA instruction
    mem[0xFFFC] = op(Op::LDA_IM);  // Load immediate
    mem[0xFFFD] = 0xFF;            // Value to load into accumulator

    cpu.execute(2, mem);  // Execute with 2 cycles for LDA_IM

    // Check result and print with color
    std::printf("%sAccumulator after LDA_IM: 0x%02X%s\n", colors::CYAN, cpu.get(Register::A), colors::RESET);

    // Print detailed CPU state
    print_cpu_state(cpu);

    // Verify with test assertion
    if (cpu.get(Register::A) != 0xFF) {
        throw testing::TestFailedException("LDA_IM test failed: Accumulator should be 0xFF");
    }

    // Verify flags are correctly set
    if (!cpu.N) {
        throw testing::TestFailedException("LDA_IM test failed: Negative flag should be set");
    }
    if (cpu.Z) {
        throw testing::TestFailedException("LDA_IM test failed: Zero flag should not be set");
    }
}

void inline_jsr_rts_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Save the original stack pointer
    byte original_sp = cpu.SP;

    // start - inline test
    mem[0xFFFC] = op(Op::JSR);
    mem[0xFFFD] = 0x42;
    mem[0xFFFE] = 0x43;

    // set something at 0x4342
    mem[0x4342] = op(Op::LDA_IM);
    mem[0x4343] = 0x84;

    // add RTS instruction at the end of our subroutine
    mem[0x4344] = op(Op::RTS);
    // end - inline test

    // Save the program counter before execution
    word expected_pc = 0xFFFF;  // After JSR+RTS, PC should be at 0xFFFF

    cpu.execute(8, mem);  // increased cycle count to include RTS execution

    // Print result with color
    std::printf("%sAccumulator after JSR and RTS: 0x%02X%s\n", colors::CYAN, cpu.get(Register::A), colors::RESET);

    // Print detailed CPU state
    print_cpu_state(cpu);

    // Verify results with test assertions
    if (cpu.get(Register::A) != 0x84) {
        throw testing::TestFailedException("JSR/RTS test failed: Accumulator should be 0x84");
    }

    // Verify stack pointer is restored
    if (cpu.SP != original_sp) {
        throw testing::TestFailedException("JSR/RTS test failed: Stack pointer not correctly restored");
    }

    // Verify program counter is at the expected location
    if (cpu.PC != expected_pc) {
        std::stringstream ss;
        ss << "JSR/RTS test failed: PC should be 0x" << std::hex << expected_pc << " but was 0x" << cpu.PC;
        throw testing::TestFailedException(ss.str());
    }
}

// Run all tests and return true if all tests pass
bool run_all_tests(Cpu& cpu, Mem& mem) {
    testing::TestSuite test_suite("6502 CPU Tests");

    std::cout << "\n"
              << colors::YELLOW << colors::BOLD << "===== Running 6502 CPU Tests =====" << colors::RESET << "\n"
              << std::endl;

    // Register and run individual tests
    test_suite.register_test("Inline LDA Test", [&]() { inline_lda_test(cpu, mem); });

    test_suite.register_test("Inline JSR/RTS Test", [&]() { inline_jsr_rts_test(cpu, mem); });

    // Print test results
    test_suite.print_results();

    // Return true if all tests passed
    return test_suite.get_failed_count() == 0;
}
}  // namespace testing
