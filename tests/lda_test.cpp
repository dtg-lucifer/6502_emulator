#include "cpu.h"
#include "memory.h"
#include "op_codes.h"
#include "test_utils.h"

using namespace colors;

namespace testing {
void inline_lda_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Inline test for LDA instruction
    mem[0xFFFC] = op(Op::LDA_IM);  // Load immediate
    mem[0xFFFD] = 0xFF;            // Value to load into accumulator

    cpu.execute(2, mem);  // Execute with 2 cycles for LDA_IM

    // Check result and print with color
    std::printf("%sAccumulator after LDA_IM: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // // Print detailed CPU state
    // print_cpu_state(cpu);

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

void inline_lda_zp_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Inline test for LDA instruction
    mem[0xFFFC] = op(Op::LDA_ZP);  // Load Zero Page value
    mem[0xFFFD] = 0x42;            // Value to load into accumulator
    mem[0x0042] = 0x37;            // Value at the zero page

    cpu.execute(3, mem);  // Execute with 3 cycles for LDA_ZP

    // Check result and print with color
    std::printf("%sAccumulator after LDA_ZP: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // // Print detailed CPU state
    // print_cpu_state(cpu);

    // Verify with test assertion
    if (cpu.get(Register::A) != 0x37) {
        throw testing::TestFailedException("LDA_ZP test failed: Accumulator should be 0xFF");
    }
}

void inline_lda_zpx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Inline test for LDA instruction
    mem[0xFFFC] = op(Op::LDA_ZPX);  // Load Zero Page with X
    mem[0xFFFD] = 0x42;             // Zero page address
    mem[0x0047] = 0x38;             // Value at the effective address (0x42 + X)

    // Set X register value
    cpu.set(Register::X, 0x05);

    cpu.execute(4, mem);  // Execute with 4 cycles for LDA_ZPX

    // Check result and print with color
    std::printf("%sAccumulator after LDA_ZPX: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Print detailed CPU state
    // print_cpu_state(cpu);

    // Verify with test assertion
    if (cpu.get(Register::A) != 0x38) {
        throw testing::TestFailedException("LDA_ZPX test failed: Accumulator should be 0x38");
    }
}

void inline_lda_zpx_wrap_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Inline test for LDA instruction
    mem[0xFFFC] = op(Op::LDA_ZPX);  // Load Zero Page with X
    mem[0xFFFD] = 0x80;             // Zero page address
    mem[0x007F] = 0x37;             // Value at the effective address (0x80 + X)

    // Set X register value
    cpu.set(Register::X, 0xFF);

    cpu.execute(4, mem);  // Execute with 4 cycles for LDA_ZPX

    // Check result and print with color
    std::printf("%sAccumulator after LDA_ZPX (With address wrapping): 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Print detailed CPU state
    // print_cpu_state(cpu);

    // Verify with test assertion
    if (cpu.get(Register::A) != 0x37) {
        throw testing::TestFailedException(
            "LDA_ZPX test failed: Accumulator should be 0x37, because it should read from address 0x007F");
    }
}
}  // namespace testing
