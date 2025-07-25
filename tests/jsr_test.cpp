#include "cpu.h"
#include "memory.h"
#include "op_codes.h"
#include "test_utils.h"

using namespace colors;

namespace testing {
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
    std::printf("%sAccumulator after JSR and RTS: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // // Print detailed CPU state
    // print_cpu_state(cpu);

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
}  // namespace testing
