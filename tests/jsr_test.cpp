#include "cpu.h"
#include "memory.h"
#include "op_codes.h"
#include "test_utils.h"

using namespace colors;

namespace testing {
void inline_jsr_rts_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // After reset, SP should be 0xFF
    byte initial_sp = cpu.SP;

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

    // Track exact number of cycles required for JSR + LDA + RTS
    bool program_completed = false;
    i32 cycles_used = cpu.execute(13, mem, &program_completed, true);  // Updated cycle count

    // Print cycles used (only outside of testing environment)
    std::printf("%s>> Execution completed in %d cycles%s\n", CYAN, cycles_used, RESET);

    // Print result with color
    std::printf("%s>> Accumulator after JSR and RTS: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // // Print detailed CPU state
    // print_cpu_state(cpu);

    // Verify results with test assertions
    if (cpu.get(Register::A) != 0x84) {
        throw testing::TestFailedException("JSR/RTS test failed: Accumulator should be 0x84");
    }

    // Verify stack pointer is restored to the initial value after JSR+RTS
    // Each JSR pushes 2 bytes (return address) and RTS pulls those 2 bytes
    if (cpu.SP != initial_sp) {
        std::stringstream ss;
        ss << "JSR/RTS test failed: Stack pointer should be restored to 0x" << std::hex << static_cast<int>(initial_sp)
           << " but was 0x" << static_cast<int>(cpu.SP);
        throw testing::TestFailedException(ss.str());
    }

    // Verify program counter is at the expected location
    if (cpu.PC != expected_pc) {
        std::stringstream ss;
        ss << "JSR/RTS test failed: PC should be 0x" << std::hex << expected_pc << " but was 0x" << cpu.PC;
        throw testing::TestFailedException(ss.str());
    }

    // Print the cycles used and completion status for diagnostics (only outside of testing environment)
    std::cout << "JSR+LDA+RTS execution took " << cycles_used
              << " cycles (Completed: " << (program_completed ? "Yes" : "No") << ")" << std::endl;

    // Verify the program completed successfully
    if (!program_completed) {
        throw testing::TestFailedException("JSR/RTS test failed: Program did not complete successfully");
    }
}
}  // namespace testing
