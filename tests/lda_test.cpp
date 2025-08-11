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

    bool program_completed = false;
    i32 cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Print cycles used and completion status

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Accumulator after LDA_IM: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // // Print detailed CPU state
    // print_cpu_state(cpu);

    // Verify with test assertion
    if (cpu.get(Register::A) != 0xFF) {
        throw testing::TestFailedException("LDA_IM test failed: Accumulator should be 0xFF");
    }

    // Verify flags are correctly set
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("LDA_IM test failed: Negative flag should be set");
    }
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("LDA_IM test failed: Zero flag should not be set");
    }
}

void inline_lda_zp_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Inline test for LDA instruction
    mem[0xFFFC] = op(Op::LDA_ZP);  // Load Zero Page value
    mem[0xFFFD] = 0x42;            // Value to load into accumulator
    mem[0x0042] = 0x37;            // Value at the zero page

    bool program_completed = false;
    i32 cycles_used = cpu.execute(3, mem, &program_completed, true);

    // Print cycles used and completion status

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Accumulator after LDA_ZP: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // // Print detailed CPU state
    // print_cpu_state(cpu);

    // Verify with test assertion
    if (cpu.get(Register::A) != 0x37) {
        throw testing::TestFailedException("LDA_ZP test failed: Accumulator should be 0x37");
    }
}

void inline_lda_absolute_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Inline test for LDA instruction
    mem[0xFFFC] = op(Op::LDA_AB);  // Load Zero Page value
    mem[0xFFFD] = 0x42;            // Load the address (little endian)
    mem[0xFFFE] = 0x43;            // Load the address (little endian)

    // Should load from 0x4342
    mem[0x4342] = 0x37;  // Value at the absolute address

    bool program_completed = false;
    constexpr i32 expected_cycles = 4;  // LDA_AB takes 4 cycles
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Print cycles used and completion status

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Accumulator after LDA_AB: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // // Print detailed CPU state
    // print_cpu_state(cpu);

    // Verify with test assertion
    if (cpu.get(Register::A) != 0x37) {
        throw testing::TestFailedException("LDA_AB test failed: Accumulator should be 0x37");
    }

    // Verify the cycles used
    if (cycles_used != expected_cycles) {
        throw testing::TestFailedException("LDA_AB test failed: Expected 4 cycles, but used " +
                                           std::to_string(cycles_used));
    }
}

void inline_lda_zpx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Inline test for LDA instruction
    mem[0xFFFC] = op(Op::LDA_ZPX);  // Load Zero Page with X
    mem[0xFFFD] = 0x42;             // Zero page address
    mem[0x0047] = 0x38;             // Value at the effective address (0x42 + X)
    mem[0xFFFE] = op(Op::NOP);      // Add termination marker (BRK/NULL)

    // Set X register value
    cpu.set(Register::X, 0x05);

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Print cycles used and completion status

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Accumulator after LDA_ZPX: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

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
    mem[0xFFFE] = op(Op::NOP);      // Add termination marker (BRK/NULL)

    // Set X register value
    cpu.set(Register::X, 0xFF);

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Print cycles used and completion status

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Accumulator after LDA_ZPX (With address wrapping): 0x%02X%s\n", CYAN, cpu.get(Register::A),
                RESET);

    // Print detailed CPU state
    // print_cpu_state(cpu);

    // Verify with test assertion
    if (cpu.get(Register::A) != 0x37) {
        throw testing::TestFailedException(
            "LDA_ZPX test failed: Accumulator should be 0x37, because it should read from address 0x007F");
    }
}
void inline_lda_absx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Inline test for LDA_ABSX instruction
    mem[0xFFFC] = op(Op::LDA_ABSX);  // Load Absolute,X
    mem[0xFFFD] = 0x80;              // Low byte of address
    mem[0xFFFE] = 0x44;              // High byte of address (0x4480)

    // Set value at the target address (0x4480 + X)
    mem[0x4485] = 0x37;  // Value at the effective address (0x4480 + X)

    // Set X register value
    cpu.set(Register::X, 0x05);

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Print cycles used and completion status

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Accumulator after LDA_ABSX: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Print detailed CPU state if needed
    // print_cpu_state(cpu);

    // Verify with test assertion
    if (cpu.get(Register::A) != 0x37) {
        throw testing::TestFailedException("LDA_ABSX test failed: Accumulator should be 0x37");
    }

    // Verify flags are correctly set (non-zero, non-negative value)
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("LDA_ABSX test failed: Zero flag should not be set");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("LDA_ABSX test failed: Negative flag should not be set");
    }

    // Test with a negative value to verify N flag
    cpu.reset(mem);
    mem[0xFFFC] = op(Op::LDA_ABSX);  // Load Absolute,X
    mem[0xFFFD] = 0x80;              // Low byte of address
    mem[0xFFFE] = 0x44;              // High byte of address (0x4480)

    // Set value at the target address (0x4480 + X)
    mem[0x4485] = 0x85;  // Negative value (bit 7 set) at the effective address

    // Set X register value
    cpu.set(Register::X, 0x05);

    program_completed = false;
    cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Verify N flag is set for negative value
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("LDA_ABSX test failed: Negative flag should be set for value 0x85");
    }
}

void inline_lda_absy_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Inline test for LDA_ABSY instruction
    mem[0xFFFC] = op(Op::LDA_ABSY);  // Load Absolute,Y
    mem[0xFFFD] = 0x80;              // Low byte of address
    mem[0xFFFE] = 0x44;              // High byte of address (0x4480)

    // Set value at the target address (0x4480 + Y)
    mem[0x4486] = 0x37;  // Value at the effective address (0x4480 + Y)

    // Set Y register value
    cpu.set(Register::Y, 0x06);

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Print cycles used and completion status

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Accumulator after LDA_ABSY: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion
    if (cpu.get(Register::A) != 0x37) {
        throw testing::TestFailedException("LDA_ABSY test failed: Accumulator should be 0x37");
    }

    // Verify flags are correctly set (non-zero, non-negative value)
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("LDA_ABSY test failed: Zero flag should not be set");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("LDA_ABSY test failed: Negative flag should not be set");
    }

    // Test with a negative value to verify N flag
    cpu.reset(mem);
    mem[0xFFFC] = op(Op::LDA_ABSY);  // Load Absolute,Y
    mem[0xFFFD] = 0x80;              // Low byte of address
    mem[0xFFFE] = 0x44;              // High byte of address (0x4480)

    // Set value at the target address (0x4480 + Y)
    mem[0x4486] = 0x85;  // Negative value (bit 7 set) at the effective address

    // Set Y register value
    cpu.set(Register::Y, 0x06);

    program_completed = false;
    cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Verify N flag is set for negative value
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("LDA_ABSY test failed: Negative flag should be set for value 0x85");
    }
}

void inline_lda_indx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Inline test for LDA_INX instruction (Indexed Indirect)
    mem[0xFFFC] = op(Op::LDA_INX);  // Load (Indirect,X)
    mem[0xFFFD] = 0x20;             // Zero page address

    // Setup X register
    cpu.set(Register::X, 0x04);

    // Zero page location (0x20 + X) contains the target address
    mem[0x24] = 0x74;  // Low byte of target address
    mem[0x25] = 0x20;  // High byte of target address (0x2074)

    // Target location contains our test value
    mem[0x2074] = 0x42;  // Value to load into A

    bool program_completed = false;
    i32 cycles_used = cpu.execute(6, mem, &program_completed, true);

    // Print cycles used and completion status

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Accumulator after LDA_INX: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion
    if (cpu.get(Register::A) != 0x42) {
        throw testing::TestFailedException("LDA_INX test failed: Accumulator should be 0x42");
    }

    // Verify flags are correctly set (non-zero, non-negative value)
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("LDA_INX test failed: Zero flag should not be set");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("LDA_INX test failed: Negative flag should not be set");
    }

    // Test with zero value to verify Z flag
    cpu.reset(mem);
    mem[0xFFFC] = op(Op::LDA_INX);  // Load (Indirect,X)
    mem[0xFFFD] = 0x20;             // Zero page address
    mem[0xFFFE] = op(Op::NOP);      // NOP

    // Setup X register
    cpu.set(Register::X, 0x04);

    // Zero page location (0x20 + X) contains the target address
    mem[0x24] = 0x74;  // Low byte of target address
    mem[0x25] = 0x20;  // High byte of target address (0x2074)

    // Target location contains our test value
    mem[0x2074] = 0x00;  // Zero value to test Z flag

    program_completed = false;
    cycles_used = cpu.execute(6, mem, &program_completed, true);

    // Verify Z flag is set for zero value
    if (!cpu.FLAGS_Z) {
        throw testing::TestFailedException("LDA_INX test failed: Zero flag should be set for value 0x00");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("LDA_INX test failed: Negative flag should not be set for value 0x00");
    }
}

void inline_lda_indy_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Inline test for LDA_INY instruction (Indirect Indexed)
    mem[0xFFFC] = op(Op::LDA_INY);  // Load (Indirect),Y
    mem[0xFFFD] = 0x30;             // Zero page address

    // Zero page location contains the base address
    mem[0x30] = 0x40;  // Low byte of indirect address
    mem[0x31] = 0x30;  // High byte of indirect address (0x3040)

    // Setup Y register for offset
    cpu.set(Register::Y, 0x10);

    // Target location (indirect addr + Y) contains our test value
    mem[0x3050] = 0x99;  // Value to load into A (at 0x3040 + 0x10)

    bool program_completed = false;
    i32 cycles_used = cpu.execute(5, mem, &program_completed, true);

    // Print cycles used and completion status

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Accumulator after LDA_INY: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion
    if (cpu.get(Register::A) != 0x99) {
        throw testing::TestFailedException("LDA_INY test failed: Accumulator should be 0x99");
    }

    // Verify flags are correctly set (non-zero, negative value)
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("LDA_INY test failed: Zero flag should not be set");
    }
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("LDA_INY test failed: Negative flag should be set for value 0x99");
    }

    // Test with zero-page wraparound
    cpu.reset(mem);
    mem[0xFFFC] = op(Op::LDA_INY);  // Load (Indirect),Y
    mem[0xFFFD] = 0xFF;             // Zero page address at the end of zero page
    mem[0xFFFE] = op(Op::NOP);      // Add NOP instruction for termination

    // Zero page location wraps around
    mem[0xFF] = 0x40;  // Low byte of indirect address
    mem[0x00] = 0x30;  // High byte of indirect address (0x3040)

    // Setup Y register
    cpu.set(Register::Y, 0x0F);

    // Target location contains our test value
    mem[0x304F] = 0x42;  // Value to load into A (at 0x3040 + 0x0F)

    program_completed = false;
    cycles_used = cpu.execute(6, mem, &program_completed, true);

    // Verify correct value is loaded despite wraparound
    if (cpu.get(Register::A) != 0x42) {
        throw testing::TestFailedException(
            "LDA_INY test failed: Accumulator should be 0x42 when reading from wrapped address");
    }
}
}  // namespace testing
