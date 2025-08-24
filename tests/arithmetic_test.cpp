#include "cpu.h"
#include "memory.h"
#include "op_codes.h"
#include "test_utils.h"

using namespace colors;

namespace testing {

// ADC Tests
void inline_adc_im_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Test case 1: Simple addition without carry
    cpu.set(Register::A, 0x10);  // A = 16
    cpu.FLAGS_C = 0;             // Clear carry flag

    mem[0xFFFC] = op(Op::ADC_IM);  // Add with Carry Immediate
    mem[0xFFFD] = 0x15;            // Add 21

    bool program_completed = false;
    i32 cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Accumulator after ADC_IM: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion - result should be 0x25 (37)
    if (cpu.get(Register::A) != 0x25) {
        throw testing::TestFailedException("ADC_IM test failed: Accumulator should be 0x25");
    }

    // Verify flags are correctly set
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("ADC_IM test failed: Zero flag should not be set");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("ADC_IM test failed: Negative flag should not be set");
    }
    if (cpu.FLAGS_C) {
        throw testing::TestFailedException("ADC_IM test failed: Carry flag should not be set");
    }
    if (cpu.FLAGS_V) {
        throw testing::TestFailedException("ADC_IM test failed: Overflow flag should not be set");
    }

    // Test case 2: Addition with carry in
    cpu.reset(mem);
    cpu.set(Register::A, 0x10);  // A = 16
    cpu.FLAGS_C = 1;             // Set carry flag

    mem[0xFFFC] = op(Op::ADC_IM);  // Add with Carry Immediate
    mem[0xFFFD] = 0x15;            // Add 21

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify result: 16 + 21 + 1 = 38 (0x26)
    if (cpu.get(Register::A) != 0x26) {
        throw testing::TestFailedException("ADC_IM with carry in test failed: Accumulator should be 0x26");
    }

    // Test case 3: Addition with carry out
    cpu.reset(mem);
    cpu.set(Register::A, 0xF0);  // A = 240
    cpu.FLAGS_C = 0;             // Clear carry flag

    mem[0xFFFC] = op(Op::ADC_IM);  // Add with Carry Immediate
    mem[0xFFFD] = 0x20;            // Add 32

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify result: 0xF0 + 0x20 = 0x110, but truncated to 8-bit is 0x10
    if (cpu.get(Register::A) != 0x10) {
        throw testing::TestFailedException("ADC_IM carry out test failed: Accumulator should be 0x10");
    }

    // Verify carry flag is set
    if (!cpu.FLAGS_C) {
        throw testing::TestFailedException("ADC_IM carry out test failed: Carry flag should be set");
    }

    // Test case 4: Addition with overflow (signed)
    cpu.reset(mem);
    cpu.set(Register::A, 0x50);  // A = 80 (positive)
    cpu.FLAGS_C = 0;             // Clear carry flag

    mem[0xFFFC] = op(Op::ADC_IM);  // Add with Carry Immediate
    mem[0xFFFD] = 0x50;            // Add 80 (positive)

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify result: 0x50 + 0x50 = 0xA0 (negative in two's complement)
    if (cpu.get(Register::A) != 0xA0) {
        throw testing::TestFailedException("ADC_IM overflow test failed: Accumulator should be 0xA0");
    }

    // Verify overflow flag is set (pos + pos = neg indicates overflow)
    if (!cpu.FLAGS_V) {
        throw testing::TestFailedException("ADC_IM overflow test failed: Overflow flag should be set");
    }

    // Verify negative flag is set
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("ADC_IM overflow test failed: Negative flag should be set");
    }

    // Test case 5: Addition resulting in zero
    cpu.reset(mem);
    cpu.set(Register::A, 0xFF);  // A = 255
    cpu.FLAGS_C = 0;             // Clear carry flag

    mem[0xFFFC] = op(Op::ADC_IM);  // Add with Carry Immediate
    mem[0xFFFD] = 0x01;            // Add 1

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify result: 0xFF + 0x01 = 0x100, but truncated to 8-bit is 0x00
    if (cpu.get(Register::A) != 0x00) {
        throw testing::TestFailedException("ADC_IM zero result test failed: Accumulator should be 0x00");
    }

    // Verify zero flag is set
    if (!cpu.FLAGS_Z) {
        throw testing::TestFailedException("ADC_IM zero result test failed: Zero flag should be set");
    }

    // Verify carry flag is set
    if (!cpu.FLAGS_C) {
        throw testing::TestFailedException("ADC_IM zero result test failed: Carry flag should be set");
    }
}

void inline_adc_zp_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set a known value in the accumulator
    cpu.set(Register::A, 0x33);  // A = 51
    cpu.FLAGS_C = 0;             // Clear carry flag

    // Inline test for ADC Zero Page instruction
    mem[0xFFFC] = op(Op::ADC_ZP);  // ADC Zero Page
    mem[0xFFFD] = 0x42;            // Zero page address
    mem[0x0042] = 0x22;            // Value at zero page address (34)

    bool program_completed = false;
    i32 cycles_used = cpu.execute(3, mem, &program_completed, true);

    // Check result and print with color
    std::printf("%s>> Accumulator after ADC_ZP: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion - result should be 0x55 (85)
    if (cpu.get(Register::A) != 0x55) {
        throw testing::TestFailedException("ADC_ZP test failed: Accumulator should be 0x55");
    }
}

void inline_adc_zpx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set a known value in the accumulator and X register
    cpu.set(Register::A, 0x33);  // A = 51
    cpu.set(Register::X, 0x05);  // X offset
    cpu.FLAGS_C = 0;             // Clear carry flag

    // Inline test for ADC Zero Page,X instruction
    mem[0xFFFC] = op(Op::ADC_ZPX);  // ADC Zero Page,X
    mem[0xFFFD] = 0x42;             // Zero page base address
    mem[0x0047] = 0x22;             // Value at effective address (0x42 + 0x05 = 0x47)

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Check result and print with color
    std::printf("%s>> Accumulator after ADC_ZPX: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion - result should be 0x55 (85)
    if (cpu.get(Register::A) != 0x55) {
        throw testing::TestFailedException("ADC_ZPX test failed: Accumulator should be 0x55");
    }
}

void inline_adc_abs_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set a known value in the accumulator
    cpu.set(Register::A, 0x33);  // A = 51
    cpu.FLAGS_C = 0;             // Clear carry flag

    // Inline test for ADC Absolute instruction
    mem[0xFFFC] = op(Op::ADC_AB);  // ADC Absolute
    mem[0xFFFD] = 0x80;            // Low byte of address
    mem[0xFFFE] = 0x44;            // High byte of address (0x4480)
    mem[0x4480] = 0x22;            // Value at absolute address (34)

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Check result and print with color
    std::printf("%s>> Accumulator after ADC_AB: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion - result should be 0x55 (85)
    if (cpu.get(Register::A) != 0x55) {
        throw testing::TestFailedException("ADC_AB test failed: Accumulator should be 0x55");
    }
}

void inline_adc_absx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set a known value in the accumulator and X register
    cpu.set(Register::A, 0x33);  // A = 51
    cpu.set(Register::X, 0x05);  // X offset
    cpu.FLAGS_C = 0;             // Clear carry flag

    // Inline test for ADC Absolute,X instruction
    mem[0xFFFC] = op(Op::ADC_ABSX);  // ADC Absolute,X
    mem[0xFFFD] = 0x80;              // Low byte of address
    mem[0xFFFE] = 0x44;              // High byte of address (0x4480)
    mem[0x4485] = 0x22;              // Value at effective address (0x4480 + 0x05 = 0x4485)

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Check result and print with color
    std::printf("%s>> Accumulator after ADC_ABSX: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion - result should be 0x55 (85)
    if (cpu.get(Register::A) != 0x55) {
        throw testing::TestFailedException("ADC_ABSX test failed: Accumulator should be 0x55");
    }

    // Test with page crossing (additional cycle)
    cpu.reset(mem);
    cpu.set(Register::A, 0x33);  // A = 51
    cpu.set(Register::X, 0xFF);  // X offset to force page crossing
    cpu.FLAGS_C = 0;             // Clear carry flag

    mem[0xFFFC] = op(Op::ADC_ABSX);  // ADC Absolute,X
    mem[0xFFFD] = 0x80;              // Low byte of address
    mem[0xFFFE] = 0x44;              // High byte of address (0x4480)
    mem[0x457F] = 0x22;              // Value at effective address (0x4480 + 0xFF = 0x457F)

    program_completed = false;
    cycles_used = cpu.execute(5, mem, &program_completed, true);  // +1 cycle for page crossing

    // Verify result
    if (cpu.get(Register::A) != 0x55) {
        throw testing::TestFailedException("ADC_ABSX page crossing test failed: Accumulator should be 0x55");
    }
}

void inline_adc_absy_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set a known value in the accumulator and Y register
    cpu.set(Register::A, 0x33);  // A = 51
    cpu.set(Register::Y, 0x06);  // Y offset
    cpu.FLAGS_C = 0;             // Clear carry flag

    // Inline test for ADC Absolute,Y instruction
    mem[0xFFFC] = op(Op::ADC_ABSY);  // ADC Absolute,Y
    mem[0xFFFD] = 0x80;              // Low byte of address
    mem[0xFFFE] = 0x44;              // High byte of address (0x4480)
    mem[0x4486] = 0x22;              // Value at effective address (0x4480 + 0x06 = 0x4486)

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Check result and print with color
    std::printf("%s>> Accumulator after ADC_ABSY: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion - result should be 0x55 (85)
    if (cpu.get(Register::A) != 0x55) {
        throw testing::TestFailedException("ADC_ABSY test failed: Accumulator should be 0x55");
    }
}

void inline_adc_indx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set a known value in the accumulator and X register
    cpu.set(Register::A, 0x33);  // A = 51
    cpu.set(Register::X, 0x04);  // X offset
    cpu.FLAGS_C = 0;             // Clear carry flag

    // Inline test for ADC (Indirect,X) instruction
    mem[0xFFFC] = op(Op::ADC_INX);  // ADC (Indirect,X)
    mem[0xFFFD] = 0x20;             // Zero page address

    // Setup the indirect address
    mem[0x24] = 0x74;    // Low byte of target address (0x20 + X = 0x24)
    mem[0x25] = 0x20;    // High byte of target address (0x2074)
    mem[0x2074] = 0x22;  // Value at target address (34)

    bool program_completed = false;
    i32 cycles_used = cpu.execute(6, mem, &program_completed, true);

    // Check result and print with color
    std::printf("%s>> Accumulator after ADC_INX: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion - result should be 0x55 (85)
    if (cpu.get(Register::A) != 0x55) {
        throw testing::TestFailedException("ADC_INX test failed: Accumulator should be 0x55");
    }
}

void inline_adc_indy_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set a known value in the accumulator and Y register
    cpu.set(Register::A, 0x33);  // A = 51
    cpu.set(Register::Y, 0x10);  // Y offset
    cpu.FLAGS_C = 0;             // Clear carry flag

    // Inline test for ADC (Indirect),Y instruction
    mem[0xFFFC] = op(Op::ADC_INY);  // ADC (Indirect),Y
    mem[0xFFFD] = 0x30;             // Zero page address

    // Setup the indirect address
    mem[0x30] = 0x40;    // Low byte of base address
    mem[0x31] = 0x30;    // High byte of base address (0x3040)
    mem[0x3050] = 0x22;  // Value at target address (0x3040 + 0x10 = 0x3050)

    bool program_completed = false;
    i32 cycles_used = cpu.execute(5, mem, &program_completed, true);

    // Check result and print with color
    std::printf("%s>> Accumulator after ADC_INY: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion - result should be 0x55 (85)
    if (cpu.get(Register::A) != 0x55) {
        throw testing::TestFailedException("ADC_INY test failed: Accumulator should be 0x55");
    }
}

// SBC Tests
void inline_sbc_im_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Test case 1: Simple subtraction with carry set
    cpu.set(Register::A, 0x50);  // A = 80
    cpu.FLAGS_C = 1;             // Set carry flag (means no borrow in SBC)

    mem[0xFFFC] = op(Op::SBC_IM);  // Subtract with Carry Immediate
    mem[0xFFFD] = 0x30;            // Subtract 48

    bool program_completed = false;
    i32 cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result and print with color
    std::printf("%s>> Accumulator after SBC_IM: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion - result should be 0x20 (32)
    if (cpu.get(Register::A) != 0x20) {
        throw testing::TestFailedException("SBC_IM test failed: Accumulator should be 0x20");
    }

    // Verify flags are correctly set
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("SBC_IM test failed: Zero flag should not be set");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("SBC_IM test failed: Negative flag should not be set");
    }
    if (!cpu.FLAGS_C) {
        throw testing::TestFailedException("SBC_IM test failed: Carry flag should be set (no borrow)");
    }
    if (cpu.FLAGS_V) {
        throw testing::TestFailedException("SBC_IM test failed: Overflow flag should not be set");
    }

    // Test case 2: Subtraction with borrow (carry clear)
    cpu.reset(mem);
    cpu.set(Register::A, 0x50);  // A = 80
    cpu.FLAGS_C = 0;             // Clear carry flag (means borrow in SBC)

    mem[0xFFFC] = op(Op::SBC_IM);  // Subtract with Carry Immediate
    mem[0xFFFD] = 0x30;            // Subtract 48

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify result: 80 - 48 - 1 = 31 (0x1F)
    if (cpu.get(Register::A) != 0x1F) {
        throw testing::TestFailedException("SBC_IM with borrow test failed: Accumulator should be 0x1F");
    }

    // Test case 3: Subtraction resulting in zero
    cpu.reset(mem);
    cpu.set(Register::A, 0x35);  // A = 53
    cpu.FLAGS_C = 1;             // Set carry flag (no borrow)

    mem[0xFFFC] = op(Op::SBC_IM);  // Subtract with Carry Immediate
    mem[0xFFFD] = 0x35;            // Subtract 53

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify result is zero
    if (cpu.get(Register::A) != 0x00) {
        throw testing::TestFailedException("SBC_IM zero result test failed: Accumulator should be 0x00");
    }

    // Verify zero flag is set
    if (!cpu.FLAGS_Z) {
        throw testing::TestFailedException("SBC_IM zero result test failed: Zero flag should be set");
    }

    // Verify carry flag is set (no borrow)
    if (!cpu.FLAGS_C) {
        throw testing::TestFailedException("SBC_IM zero result test failed: Carry flag should be set (no borrow)");
    }

    // Test case 4: Subtraction resulting in negative value
    cpu.reset(mem);
    cpu.set(Register::A, 0x35);  // A = 53
    cpu.FLAGS_C = 1;             // Set carry flag (no borrow)

    mem[0xFFFC] = op(Op::SBC_IM);  // Subtract with Carry Immediate
    mem[0xFFFD] = 0x40;            // Subtract 64

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify result: 53 - 64 = -11 (0xF5 in two's complement)
    if (cpu.get(Register::A) != 0xF5) {
        throw testing::TestFailedException("SBC_IM negative result test failed: Accumulator should be 0xF5");
    }

    // Verify negative flag is set
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("SBC_IM negative result test failed: Negative flag should be set");
    }

    // Verify carry flag is clear (borrow occurred)
    if (cpu.FLAGS_C) {
        throw testing::TestFailedException("SBC_IM negative result test failed: Carry flag should be clear (borrow)");
    }

    // Test case 5: Subtraction with overflow
    cpu.reset(mem);
    cpu.set(Register::A, 0x80);  // A = -128 (in two's complement)
    cpu.FLAGS_C = 1;             // Set carry flag (no borrow)

    mem[0xFFFC] = op(Op::SBC_IM);  // Subtract with Carry Immediate
    mem[0xFFFD] = 0x01;            // Subtract 1

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify result: -128 - 1 = -129 or 127 (0x7F) with overflow
    if (cpu.get(Register::A) != 0x7F) {
        throw testing::TestFailedException("SBC_IM overflow test failed: Accumulator should be 0x7F");
    }

    // Verify overflow flag is set (neg - pos = pos indicates overflow)
    if (!cpu.FLAGS_V) {
        throw testing::TestFailedException("SBC_IM overflow test failed: Overflow flag should be set");
    }

    // Verify negative flag is clear
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("SBC_IM overflow test failed: Negative flag should be clear");
    }
}

void inline_sbc_zp_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set a known value in the accumulator
    cpu.set(Register::A, 0x50);  // A = 80
    cpu.FLAGS_C = 1;             // Set carry flag (no borrow)

    // Inline test for SBC Zero Page instruction
    mem[0xFFFC] = op(Op::SBC_ZP);  // SBC Zero Page
    mem[0xFFFD] = 0x42;            // Zero page address
    mem[0x0042] = 0x30;            // Value at zero page address (48)

    bool program_completed = false;
    i32 cycles_used = cpu.execute(3, mem, &program_completed, true);

    // Check result and print with color
    std::printf("%s>> Accumulator after SBC_ZP: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion - result should be 0x20 (32)
    if (cpu.get(Register::A) != 0x20) {
        throw testing::TestFailedException("SBC_ZP test failed: Accumulator should be 0x20");
    }
}

void inline_sbc_zpx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set known values in registers
    cpu.set(Register::A, 0x50);  // A = 80
    cpu.set(Register::X, 0x05);  // X offset
    cpu.FLAGS_C = 1;             // Set carry flag (no borrow)

    // Inline test for SBC Zero Page,X instruction
    mem[0xFFFC] = op(Op::SBC_ZPX);  // SBC Zero Page,X
    mem[0xFFFD] = 0x42;             // Zero page base address
    mem[0x0047] = 0x30;             // Value at effective address (0x42 + 0x05 = 0x47)

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Check result and print with color
    std::printf("%s>> Accumulator after SBC_ZPX: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion - result should be 0x20 (32)
    if (cpu.get(Register::A) != 0x20) {
        throw testing::TestFailedException("SBC_ZPX test failed: Accumulator should be 0x20");
    }
}

void inline_sbc_abs_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set a known value in the accumulator
    cpu.set(Register::A, 0x50);  // A = 80
    cpu.FLAGS_C = 1;             // Set carry flag (no borrow)

    // Inline test for SBC Absolute instruction
    mem[0xFFFC] = op(Op::SBC_AB);  // SBC Absolute
    mem[0xFFFD] = 0x80;            // Low byte of address
    mem[0xFFFE] = 0x44;            // High byte of address (0x4480)
    mem[0x4480] = 0x30;            // Value at absolute address (48)

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Check result and print with color
    std::printf("%s>> Accumulator after SBC_AB: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion - result should be 0x20 (32)
    if (cpu.get(Register::A) != 0x20) {
        throw testing::TestFailedException("SBC_AB test failed: Accumulator should be 0x20");
    }
}

void inline_sbc_absx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set known values in registers
    cpu.set(Register::A, 0x50);  // A = 80
    cpu.set(Register::X, 0x05);  // X offset
    cpu.FLAGS_C = 1;             // Set carry flag (no borrow)

    // Inline test for SBC Absolute,X instruction
    mem[0xFFFC] = op(Op::SBC_ABSX);  // SBC Absolute,X
    mem[0xFFFD] = 0x80;              // Low byte of address
    mem[0xFFFE] = 0x44;              // High byte of address (0x4480)
    mem[0x4485] = 0x30;              // Value at effective address (0x4480 + 0x05 = 0x4485)

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Check result and print with color
    std::printf("%s>> Accumulator after SBC_ABSX: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion - result should be 0x20 (32)
    if (cpu.get(Register::A) != 0x20) {
        throw testing::TestFailedException("SBC_ABSX test failed: Accumulator should be 0x20");
    }
}

void inline_sbc_absy_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set known values in registers
    cpu.set(Register::A, 0x50);  // A = 80
    cpu.set(Register::Y, 0x06);  // Y offset
    cpu.FLAGS_C = 1;             // Set carry flag (no borrow)

    // Inline test for SBC Absolute,Y instruction
    mem[0xFFFC] = op(Op::SBC_ABSY);  // SBC Absolute,Y
    mem[0xFFFD] = 0x80;              // Low byte of address
    mem[0xFFFE] = 0x44;              // High byte of address (0x4480)
    mem[0x4486] = 0x30;              // Value at effective address (0x4480 + 0x06 = 0x4486)

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Check result and print with color
    std::printf("%s>> Accumulator after SBC_ABSY: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion - result should be 0x20 (32)
    if (cpu.get(Register::A) != 0x20) {
        throw testing::TestFailedException("SBC_ABSY test failed: Accumulator should be 0x20");
    }
}

void inline_sbc_indx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set known values in registers
    cpu.set(Register::A, 0x50);  // A = 80
    cpu.set(Register::X, 0x04);  // X offset
    cpu.FLAGS_C = 1;             // Set carry flag (no borrow)

    // Inline test for SBC (Indirect,X) instruction
    mem[0xFFFC] = op(Op::SBC_INX);  // SBC (Indirect,X)
    mem[0xFFFD] = 0x20;             // Zero page address

    // Setup the indirect address
    mem[0x24] = 0x74;    // Low byte of target address (0x20 + X = 0x24)
    mem[0x25] = 0x20;    // High byte of target address (0x2074)
    mem[0x2074] = 0x30;  // Value at target address (48)

    bool program_completed = false;
    i32 cycles_used = cpu.execute(6, mem, &program_completed, true);

    // Check result and print with color
    std::printf("%s>> Accumulator after SBC_INX: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion - result should be 0x20 (32)
    if (cpu.get(Register::A) != 0x20) {
        throw testing::TestFailedException("SBC_INX test failed: Accumulator should be 0x20");
    }
}

void inline_sbc_indy_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set known values in registers
    cpu.set(Register::A, 0x50);  // A = 80
    cpu.set(Register::Y, 0x10);  // Y offset
    cpu.FLAGS_C = 1;             // Set carry flag (no borrow)

    // Inline test for SBC (Indirect),Y instruction
    mem[0xFFFC] = op(Op::SBC_INY);  // SBC (Indirect),Y
    mem[0xFFFD] = 0x30;             // Zero page address

    // Setup the indirect address
    mem[0x30] = 0x40;    // Low byte of base address
    mem[0x31] = 0x30;    // High byte of base address (0x3040)
    mem[0x3050] = 0x30;  // Value at target address (0x3040 + 0x10 = 0x3050)

    bool program_completed = false;
    i32 cycles_used = cpu.execute(5, mem, &program_completed, true);

    // Check result and print with color
    std::printf("%s>> Accumulator after SBC_INY: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertion - result should be 0x20 (32)
    if (cpu.get(Register::A) != 0x20) {
        throw testing::TestFailedException("SBC_INY test failed: Accumulator should be 0x20");
    }
}

// Test suite function for arithmetic operations
void arithmetic_test_suite(Cpu& cpu, Mem& mem) {
    testing::TestSuite test_suite("Arithmetic Operations");

    test_suite.print_header();

    // Register and run ADC tests
    test_suite.register_test("ADC Immediate", [&]() { inline_adc_im_test(cpu, mem); });
    test_suite.register_test("ADC Zero Page", [&]() { inline_adc_zp_test(cpu, mem); });
    test_suite.register_test("ADC Zero Page,X", [&]() { inline_adc_zpx_test(cpu, mem); });
    test_suite.register_test("ADC Absolute", [&]() { inline_adc_abs_test(cpu, mem); });
    test_suite.register_test("ADC Absolute,X", [&]() { inline_adc_absx_test(cpu, mem); });
    test_suite.register_test("ADC Absolute,Y", [&]() { inline_adc_absy_test(cpu, mem); });
    test_suite.register_test("ADC (Indirect,X)", [&]() { inline_adc_indx_test(cpu, mem); });
    test_suite.register_test("ADC (Indirect),Y", [&]() { inline_adc_indy_test(cpu, mem); });

    // Register and run SBC tests
    test_suite.register_test("SBC Immediate", [&]() { inline_sbc_im_test(cpu, mem); });
    test_suite.register_test("SBC Zero Page", [&]() { inline_sbc_zp_test(cpu, mem); });
    test_suite.register_test("SBC Zero Page,X", [&]() { inline_sbc_zpx_test(cpu, mem); });
    test_suite.register_test("SBC Absolute", [&]() { inline_sbc_abs_test(cpu, mem); });
    test_suite.register_test("SBC Absolute,X", [&]() { inline_sbc_absx_test(cpu, mem); });
    test_suite.register_test("SBC Absolute,Y", [&]() { inline_sbc_absy_test(cpu, mem); });
    test_suite.register_test("SBC (Indirect,X)", [&]() { inline_sbc_indx_test(cpu, mem); });
    test_suite.register_test("SBC (Indirect),Y", [&]() { inline_sbc_indy_test(cpu, mem); });

    // Print the results
    test_suite.print_results();
}

}  // namespace testing
