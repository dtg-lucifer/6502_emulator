#include "cpu.h"
#include "memory.h"
#include "op_codes.h"
#include "test_utils.h"

using namespace colors;

namespace testing {

// ASL (Arithmetic Shift Left) Tests

void inline_asl_a_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Test case 1: Normal shift with no overflow
    cpu.set(Register::A, 0b01101001);  // 105 decimal

    // Load ASL A instruction
    mem[0xFFFC] = op(Op::ASL_A);
    mem[0xFFFD] = op(Op::NOP);  // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(2, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result: 01101001 << 1 = 11010010 (210 decimal)
    if (cpu.get(Register::A) != 0b11010010) {
        throw testing::TestFailedException("ASL_A test failed: Accumulator should be 0xD2 (210 decimal)");
    }

    // Check carry flag is clear (MSB of original value was 0)
    if (cpu.FLAGS_C) {
        throw testing::TestFailedException("ASL_A test failed: Carry flag should be clear");
    }

    // Check negative flag is set (bit 7 of result is 1)
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("ASL_A test failed: Negative flag should be set");
    }

    // Check zero flag is clear (result is non-zero)
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("ASL_A test failed: Zero flag should be clear");
    }

    // Test case 2: Shift with bit 7 set (testing carry flag)
    cpu.reset(mem);
    cpu.set(Register::A, 0b10000001);  // 129 decimal

    // Reset PC and execute again
    cpu.PC = 0xFFFC;
    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Check result: 10000001 << 1 = 00000010 (2 decimal)
    if (cpu.get(Register::A) != 0b00000010) {
        throw testing::TestFailedException("ASL_A test failed: Accumulator should be 0x02 (2 decimal)");
    }

    // Check carry flag is set (MSB of original value was 1)
    if (!cpu.FLAGS_C) {
        throw testing::TestFailedException("ASL_A test failed: Carry flag should be set");
    }

    // Check negative flag is clear (bit 7 of result is 0)
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("ASL_A test failed: Negative flag should be clear");
    }

    // Test case 3: Shift resulting in zero
    cpu.reset(mem);
    cpu.set(Register::A, 0b10000000);  // 128 decimal

    // Reset PC and execute again
    cpu.PC = 0xFFFC;
    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Check result: 10000000 << 1 = 00000000 (0 decimal)
    if (cpu.get(Register::A) != 0) {
        throw testing::TestFailedException("ASL_A test failed: Accumulator should be 0x00 (0 decimal)");
    }

    // Check carry flag is set (MSB of original value was 1)
    if (!cpu.FLAGS_C) {
        throw testing::TestFailedException("ASL_A test failed: Carry flag should be set");
    }

    // Check zero flag is set (result is zero)
    if (!cpu.FLAGS_Z) {
        throw testing::TestFailedException("ASL_A test failed: Zero flag should be set");
    }
}

void inline_asl_zp_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set up memory
    byte zp_address = 0x42;
    mem[zp_address] = 0b01101001;  // 105 decimal

    // Load ASL Zero Page instruction
    mem[0xFFFC] = op(Op::ASL_ZP);
    mem[0xFFFD] = zp_address;
    mem[0xFFFE] = op(Op::NOP);  // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(5, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result: 01101001 << 1 = 11010010 (210 decimal)
    if (mem[zp_address] != 0b11010010) {
        throw testing::TestFailedException("ASL_ZP test failed: Memory at ZP should be 0xD2 (210 decimal)");
    }

    // Check flags (same as ASL_A test)
    if (cpu.FLAGS_C) {
        throw testing::TestFailedException("ASL_ZP test failed: Carry flag should be clear");
    }
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("ASL_ZP test failed: Negative flag should be set");
    }
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("ASL_ZP test failed: Zero flag should be clear");
    }
}

void inline_asl_zpx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set up memory and X register
    byte zp_base = 0x40;
    byte x_offset = 0x02;
    byte zp_address = zp_base + x_offset;  // 0x42
    cpu.set(Register::X, x_offset);
    mem[zp_address] = 0b10101010;  // 170 decimal

    // Load ASL Zero Page,X instruction
    mem[0xFFFC] = op(Op::ASL_ZPX);
    mem[0xFFFD] = zp_base;
    mem[0xFFFE] = op(Op::NOP);  // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(6, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result: 10101010 << 1 = 01010100 (84 decimal)
    if (mem[zp_address] != 0b01010100) {
        throw testing::TestFailedException("ASL_ZPX test failed: Memory at ZP+X should be 0x54 (84 decimal)");
    }

    // Check flags
    if (!cpu.FLAGS_C) {
        throw testing::TestFailedException("ASL_ZPX test failed: Carry flag should be set (MSB was 1)");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("ASL_ZPX test failed: Negative flag should be clear");
    }
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("ASL_ZPX test failed: Zero flag should be clear");
    }
}

void inline_asl_abs_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set up memory
    word abs_address = 0x4242;
    mem[abs_address] = 0b01101001;  // 105 decimal

    // Load ASL Absolute instruction
    mem[0xFFFC] = op(Op::ASL_AB);
    mem[0xFFFD] = abs_address & 0xFF;         // Low byte
    mem[0xFFFE] = (abs_address >> 8) & 0xFF;  // High byte
    mem[0xFFFF] = op(Op::NOP);                // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(6, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result: 01101001 << 1 = 11010010 (210 decimal)
    if (mem[abs_address] != 0b11010010) {
        throw testing::TestFailedException("ASL_AB test failed: Memory at absolute address should be 0xD2");
    }

    // Check flags
    if (cpu.FLAGS_C) {
        throw testing::TestFailedException("ASL_AB test failed: Carry flag should be clear");
    }
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("ASL_AB test failed: Negative flag should be set");
    }
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("ASL_AB test failed: Zero flag should be clear");
    }
}

void inline_asl_absx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set up memory and X register
    word abs_base = 0x4240;
    byte x_offset = 0x02;
    word abs_address = abs_base + x_offset;  // 0x4242
    cpu.set(Register::X, x_offset);
    mem[abs_address] = 0b10000000;  // 128 decimal

    // Load ASL Absolute,X instruction
    mem[0xFFFC] = op(Op::ASL_ABSX);
    mem[0xFFFD] = abs_base & 0xFF;         // Low byte
    mem[0xFFFE] = (abs_base >> 8) & 0xFF;  // High byte
    mem[0xFFFF] = op(Op::NOP);             // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(7, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result: 10000000 << 1 = 00000000
    if (mem[abs_address] != 0) {
        throw testing::TestFailedException("ASL_ABSX test failed: Memory at absolute address+X should be 0x00");
    }

    // Check flags
    if (!cpu.FLAGS_C) {
        throw testing::TestFailedException("ASL_ABSX test failed: Carry flag should be set (MSB was 1)");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("ASL_ABSX test failed: Negative flag should be clear");
    }
    if (!cpu.FLAGS_Z) {
        throw testing::TestFailedException("ASL_ABSX test failed: Zero flag should be set (result is 0)");
    }
}

// LSR (Logical Shift Right) Tests

void inline_lsr_a_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Test case 1: Normal shift with no carry
    cpu.set(Register::A, 0b01101000);  // 104 decimal

    // Load LSR A instruction
    mem[0xFFFC] = op(Op::LSR_A);
    mem[0xFFFD] = op(Op::NOP);  // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(2, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result: 01101000 >> 1 = 00110100 (52 decimal)
    if (cpu.get(Register::A) != 0b00110100) {
        throw testing::TestFailedException("LSR_A test failed: Accumulator should be 0x34 (52 decimal)");
    }

    // Check carry flag is clear (LSB of original value was 0)
    if (cpu.FLAGS_C) {
        throw testing::TestFailedException("LSR_A test failed: Carry flag should be clear");
    }

    // Check negative flag is clear (bit 7 of result is always 0)
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("LSR_A test failed: Negative flag should always be clear for LSR");
    }

    // Check zero flag is clear (result is non-zero)
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("LSR_A test failed: Zero flag should be clear");
    }

    // Test case 2: Shift with LSB set (testing carry flag)
    cpu.reset(mem);
    cpu.set(Register::A, 0b10000001);  // 129 decimal

    // Reset PC and execute again
    cpu.PC = 0xFFFC;
    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Check result: 10000001 >> 1 = 01000000 (64 decimal)
    if (cpu.get(Register::A) != 0b01000000) {
        throw testing::TestFailedException("LSR_A test failed: Accumulator should be 0x40 (64 decimal)");
    }

    // Check carry flag is set (LSB of original value was 1)
    if (!cpu.FLAGS_C) {
        throw testing::TestFailedException("LSR_A test failed: Carry flag should be set");
    }

    // Test case 3: Shift resulting in zero
    cpu.reset(mem);
    cpu.set(Register::A, 0b00000001);  // 1 decimal

    // Reset PC and execute again
    cpu.PC = 0xFFFC;
    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Check result: 00000001 >> 1 = 00000000 (0 decimal)
    if (cpu.get(Register::A) != 0) {
        throw testing::TestFailedException("LSR_A test failed: Accumulator should be 0x00 (0 decimal)");
    }

    // Check carry flag is set (LSB of original value was 1)
    if (!cpu.FLAGS_C) {
        throw testing::TestFailedException("LSR_A test failed: Carry flag should be set");
    }

    // Check zero flag is set (result is zero)
    if (!cpu.FLAGS_Z) {
        throw testing::TestFailedException("LSR_A test failed: Zero flag should be set");
    }
}

void inline_lsr_zp_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set up memory
    byte zp_address = 0x42;
    mem[zp_address] = 0b01101001;  // 105 decimal (odd number to test LSB)

    // Load LSR Zero Page instruction
    mem[0xFFFC] = op(Op::LSR_ZP);
    mem[0xFFFD] = zp_address;
    mem[0xFFFE] = op(Op::NOP);  // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(5, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result: 01101001 >> 1 = 00110100 (52 decimal)
    if (mem[zp_address] != 0b00110100) {
        throw testing::TestFailedException("LSR_ZP test failed: Memory at ZP should be 0x34 (52 decimal)");
    }

    // Check flags
    if (!cpu.FLAGS_C) {
        throw testing::TestFailedException("LSR_ZP test failed: Carry flag should be set (LSB was 1)");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("LSR_ZP test failed: Negative flag should always be clear for LSR");
    }
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("LSR_ZP test failed: Zero flag should be clear");
    }
}

void inline_lsr_zpx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set up memory and X register
    byte zp_base = 0x40;
    byte x_offset = 0x02;
    byte zp_address = zp_base + x_offset;  // 0x42
    cpu.set(Register::X, x_offset);
    mem[zp_address] = 0b00000010;  // 2 decimal

    // Load LSR Zero Page,X instruction
    mem[0xFFFC] = op(Op::LSR_ZPX);
    mem[0xFFFD] = zp_base;
    mem[0xFFFE] = op(Op::NOP);  // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(6, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result: 00000010 >> 1 = 00000001 (1 decimal)
    if (mem[zp_address] != 0b00000001) {
        throw testing::TestFailedException("LSR_ZPX test failed: Memory at ZP+X should be 0x01 (1 decimal)");
    }

    // Check flags
    if (cpu.FLAGS_C) {
        throw testing::TestFailedException("LSR_ZPX test failed: Carry flag should be clear (LSB was 0)");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("LSR_ZPX test failed: Negative flag should always be clear for LSR");
    }
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("LSR_ZPX test failed: Zero flag should be clear");
    }
}

void inline_lsr_abs_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set up memory
    word abs_address = 0x4242;
    mem[abs_address] = 0b00000001;  // 1 decimal

    // Load LSR Absolute instruction
    mem[0xFFFC] = op(Op::LSR_AB);
    mem[0xFFFD] = abs_address & 0xFF;         // Low byte
    mem[0xFFFE] = (abs_address >> 8) & 0xFF;  // High byte
    mem[0xFFFF] = op(Op::NOP);                // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(6, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result: 00000001 >> 1 = 00000000 (0 decimal)
    if (mem[abs_address] != 0) {
        throw testing::TestFailedException("LSR_AB test failed: Memory at absolute address should be 0x00");
    }

    // Check flags
    if (!cpu.FLAGS_C) {
        throw testing::TestFailedException("LSR_AB test failed: Carry flag should be set (LSB was 1)");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("LSR_AB test failed: Negative flag should always be clear for LSR");
    }
    if (!cpu.FLAGS_Z) {
        throw testing::TestFailedException("LSR_AB test failed: Zero flag should be set (result is 0)");
    }
}

void inline_lsr_absx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set up memory and X register
    word abs_base = 0x4240;
    byte x_offset = 0x02;
    word abs_address = abs_base + x_offset;  // 0x4242
    cpu.set(Register::X, x_offset);
    mem[abs_address] = 0b10101010;  // 170 decimal

    // Load LSR Absolute,X instruction
    mem[0xFFFC] = op(Op::LSR_ABSX);
    mem[0xFFFD] = abs_base & 0xFF;         // Low byte
    mem[0xFFFE] = (abs_base >> 8) & 0xFF;  // High byte
    mem[0xFFFF] = op(Op::NOP);             // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(7, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result: 10101010 >> 1 = 01010101 (85 decimal)
    if (mem[abs_address] != 0b01010101) {
        throw testing::TestFailedException("LSR_ABSX test failed: Memory at absolute address+X should be 0x55");
    }

    // Check flags
    if (cpu.FLAGS_C) {
        throw testing::TestFailedException("LSR_ABSX test failed: Carry flag should be clear (LSB was 0)");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("LSR_ABSX test failed: Negative flag should always be clear for LSR");
    }
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("LSR_ABSX test failed: Zero flag should be clear");
    }
}

// ROL (Rotate Left) Tests

void inline_rol_a_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Test case 1: Rotate left with carry clear initially
    cpu.set(Register::A, 0b01101001);  // 105 decimal
    cpu.FLAGS_C = 0;                   // Clear carry initially

    // Load ROL A instruction
    mem[0xFFFC] = op(Op::ROL_A);
    mem[0xFFFD] = op(Op::NOP);  // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(2, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result: 01101001 << 1 + carry(0) = 11010010 (210 decimal)
    if (cpu.get(Register::A) != 0b11010010) {
        throw testing::TestFailedException("ROL_A test failed: Accumulator should be 0xD2 (210 decimal)");
    }

    // Check carry flag is clear (MSB of original value was 0)
    if (cpu.FLAGS_C) {
        throw testing::TestFailedException("ROL_A test failed: Carry flag should be clear");
    }

    // Check negative flag is set (bit 7 of result is 1)
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("ROL_A test failed: Negative flag should be set");
    }

    // Test case 2: Rotate left with carry set initially
    cpu.reset(mem);
    cpu.set(Register::A, 0b10000000);  // 128 decimal
    cpu.FLAGS_C = 1;                   // Set carry initially

    // Reset PC and execute again
    cpu.PC = 0xFFFC;
    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Check result: 10000000 << 1 + carry(1) = 00000001 (1 decimal)
    if (cpu.get(Register::A) != 0b00000001) {
        throw testing::TestFailedException("ROL_A test failed: Accumulator should be 0x01 (1 decimal)");
    }

    // Check carry flag is set (MSB of original value was 1)
    if (!cpu.FLAGS_C) {
        throw testing::TestFailedException("ROL_A test failed: Carry flag should be set");
    }

    // Check negative flag is clear (bit 7 of result is 0)
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("ROL_A test failed: Negative flag should be clear");
    }

    // Test case 3: Rotate resulting in zero
    cpu.reset(mem);
    cpu.set(Register::A, 0b10000000);  // 128 decimal
    cpu.FLAGS_C = 0;                   // Clear carry initially

    // Reset PC and execute again
    cpu.PC = 0xFFFC;
    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Check result: 10000000 << 1 + carry(0) = 00000000 (0 decimal)
    if (cpu.get(Register::A) != 0) {
        throw testing::TestFailedException("ROL_A test failed: Accumulator should be 0x00 (0 decimal)");
    }

    // Check zero flag is set (result is zero)
    if (!cpu.FLAGS_Z) {
        throw testing::TestFailedException("ROL_A test failed: Zero flag should be set");
    }
}

void inline_rol_zp_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set up memory and initial carry flag
    byte zp_address = 0x42;
    mem[zp_address] = 0b01101001;  // 105 decimal
    cpu.FLAGS_C = 0;               // Clear carry initially

    // Load ROL Zero Page instruction
    mem[0xFFFC] = op(Op::ROL_ZP);
    mem[0xFFFD] = zp_address;
    mem[0xFFFE] = op(Op::NOP);  // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(5, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result: 01101001 << 1 + carry(0) = 11010010 (210 decimal)
    if (mem[zp_address] != 0b11010010) {
        throw testing::TestFailedException("ROL_ZP test failed: Memory at ZP should be 0xD2 (210 decimal)");
    }

    // Check flags
    if (cpu.FLAGS_C) {
        throw testing::TestFailedException("ROL_ZP test failed: Carry flag should be clear");
    }
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("ROL_ZP test failed: Negative flag should be set");
    }
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("ROL_ZP test failed: Zero flag should be clear");
    }

    // Test with carry set initially
    cpu.reset(mem);
    mem[zp_address] = 0b10000000;  // 128 decimal
    cpu.FLAGS_C = 1;               // Set carry initially

    cpu.PC = 0xFFFC;  // Reset PC
    program_completed = false;
    cycles_used = cpu.execute(5, mem, &program_completed, true);

    // Check result: 10000000 << 1 + carry(1) = 00000001 (1 decimal)
    if (mem[zp_address] != 0b00000001) {
        throw testing::TestFailedException("ROL_ZP test failed: Memory at ZP should be 0x01 (1 decimal)");
    }

    // Check carry flag is set (MSB of original value was 1)
    if (!cpu.FLAGS_C) {
        throw testing::TestFailedException("ROL_ZP test failed: Carry flag should be set");
    }
}

void inline_rol_zpx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set up memory, X register, and initial carry flag
    byte zp_base = 0x40;
    byte x_offset = 0x02;
    byte zp_address = zp_base + x_offset;  // 0x42
    cpu.set(Register::X, x_offset);
    mem[zp_address] = 0b01101001;  // 105 decimal
    cpu.FLAGS_C = 1;               // Set carry initially

    // Load ROL Zero Page,X instruction
    mem[0xFFFC] = op(Op::ROL_ZPX);
    mem[0xFFFD] = zp_base;
    mem[0xFFFE] = op(Op::NOP);  // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(6, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result: 01101001 << 1 + carry(1) = 11010011 (211 decimal)
    if (mem[zp_address] != 0b11010011) {
        throw testing::TestFailedException("ROL_ZPX test failed: Memory at ZP+X should be 0xD3 (211 decimal)");
    }

    // Check flags
    if (cpu.FLAGS_C) {
        throw testing::TestFailedException("ROL_ZPX test failed: Carry flag should be clear");
    }
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("ROL_ZPX test failed: Negative flag should be set");
    }
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("ROL_ZPX test failed: Zero flag should be clear");
    }
}

void inline_rol_abs_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set up memory and initial carry flag
    word abs_address = 0x4242;
    mem[abs_address] = 0b10000000;  // 128 decimal
    cpu.FLAGS_C = 0;                // Clear carry initially

    // Load ROL Absolute instruction
    mem[0xFFFC] = op(Op::ROL_AB);
    mem[0xFFFD] = abs_address & 0xFF;         // Low byte
    mem[0xFFFE] = (abs_address >> 8) & 0xFF;  // High byte
    mem[0xFFFF] = op(Op::NOP);                // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(6, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result: 10000000 << 1 + carry(0) = 00000000 (0 decimal)
    if (mem[abs_address] != 0) {
        throw testing::TestFailedException("ROL_AB test failed: Memory at absolute address should be 0x00");
    }

    // Check flags
    if (!cpu.FLAGS_C) {
        throw testing::TestFailedException("ROL_AB test failed: Carry flag should be set (MSB was 1)");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("ROL_AB test failed: Negative flag should be clear");
    }
    if (!cpu.FLAGS_Z) {
        throw testing::TestFailedException("ROL_AB test failed: Zero flag should be set (result is 0)");
    }
}

void inline_rol_absx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set up memory, X register, and initial carry flag
    word abs_base = 0x4240;
    byte x_offset = 0x02;
    word abs_address = abs_base + x_offset;  // 0x4242
    cpu.set(Register::X, x_offset);
    mem[abs_address] = 0b01010101;  // 85 decimal
    cpu.FLAGS_C = 1;                // Set carry initially

    // Load ROL Absolute,X instruction
    mem[0xFFFC] = op(Op::ROL_ABSX);
    mem[0xFFFD] = abs_base & 0xFF;         // Low byte
    mem[0xFFFE] = (abs_base >> 8) & 0xFF;  // High byte
    mem[0xFFFF] = op(Op::NOP);             // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(7, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result: 01010101 << 1 + carry(1) = 10101011 (171 decimal)
    if (mem[abs_address] != 0b10101011) {
        throw testing::TestFailedException("ROL_ABSX test failed: Memory at absolute address+X should be 0xAB");
    }

    // Check flags
    if (cpu.FLAGS_C) {
        throw testing::TestFailedException("ROL_ABSX test failed: Carry flag should be clear (MSB was 0)");
    }
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("ROL_ABSX test failed: Negative flag should be set (bit 7 is 1)");
    }
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("ROL_ABSX test failed: Zero flag should be clear");
    }
}

// ROR (Rotate Right) Tests

void inline_ror_a_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Test case 1: Rotate right with carry clear initially
    cpu.set(Register::A, 0b01101001);  // 105 decimal
    cpu.FLAGS_C = 0;                   // Clear carry initially

    // Load ROR A instruction
    mem[0xFFFC] = op(Op::ROR_A);
    mem[0xFFFD] = op(Op::NOP);  // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(2, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result: carry(0) << 7 + 01101001 >> 1 = 00110100 (52 decimal)
    if (cpu.get(Register::A) != 0b00110100) {
        throw testing::TestFailedException("ROR_A test failed: Accumulator should be 0x34 (52 decimal)");
    }

    // Check carry flag is set (LSB of original value was 1)
    if (!cpu.FLAGS_C) {
        throw testing::TestFailedException("ROR_A test failed: Carry flag should be set");
    }

    // Test case 2: Rotate right with carry set initially
    cpu.reset(mem);
    cpu.set(Register::A, 0b00000100);  // 4 decimal
    cpu.FLAGS_C = 1;                   // Set carry initially

    // Reset PC and execute again
    cpu.PC = 0xFFFC;
    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Check result: carry(1) << 7 + 00000100 >> 1 = 10000010 (130 decimal)
    if (cpu.get(Register::A) != 0b10000010) {
        throw testing::TestFailedException("ROR_A test failed: Accumulator should be 0x82 (130 decimal)");
    }

    // Check negative flag is set (bit 7 of result is 1)
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("ROR_A test failed: Negative flag should be set");
    }

    // Check carry flag is clear (LSB of original value was 0)
    if (cpu.FLAGS_C) {
        throw testing::TestFailedException("ROR_A test failed: Carry flag should be clear");
    }
}

void inline_ror_zp_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set up memory and initial carry flag
    byte zp_address = 0x42;
    mem[zp_address] = 0b01101001;  // 105 decimal
    cpu.FLAGS_C = 0;               // Clear carry initially

    // Load ROR Zero Page instruction
    mem[0xFFFC] = op(Op::ROR_ZP);
    mem[0xFFFD] = zp_address;
    mem[0xFFFE] = op(Op::NOP);  // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(5, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result: carry(0) << 7 + 01101001 >> 1 = 00110100 (52 decimal)
    if (mem[zp_address] != 0b00110100) {
        throw testing::TestFailedException("ROR_ZP test failed: Memory at ZP should be 0x34 (52 decimal)");
    }

    // Check flags
    if (!cpu.FLAGS_C) {
        throw testing::TestFailedException("ROR_ZP test failed: Carry flag should be set (LSB was 1)");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("ROR_ZP test failed: Negative flag should be clear");
    }
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("ROR_ZP test failed: Zero flag should be clear");
    }

    // Test with carry set initially
    cpu.reset(mem);
    mem[zp_address] = 0b00000000;  // 0 decimal
    cpu.FLAGS_C = 1;               // Set carry initially

    cpu.PC = 0xFFFC;  // Reset PC
    program_completed = false;
    cycles_used = cpu.execute(5, mem, &program_completed, true);

    // Check result: carry(1) << 7 + 00000000 >> 1 = 10000000 (128 decimal)
    if (mem[zp_address] != 0b10000000) {
        throw testing::TestFailedException("ROR_ZP test failed: Memory at ZP should be 0x80 (128 decimal)");
    }

    // Check carry flag is clear (LSB of original value was 0)
    if (cpu.FLAGS_C) {
        throw testing::TestFailedException("ROR_ZP test failed: Carry flag should be clear");
    }
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("ROR_ZP test failed: Negative flag should be set (bit 7 is 1)");
    }
}

void inline_ror_zpx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set up memory, X register, and initial carry flag
    byte zp_base = 0x40;
    byte x_offset = 0x02;
    byte zp_address = zp_base + x_offset;  // 0x42
    cpu.set(Register::X, x_offset);
    mem[zp_address] = 0b00000001;  // 1 decimal
    cpu.FLAGS_C = 1;               // Set carry initially

    // Load ROR Zero Page,X instruction
    mem[0xFFFC] = op(Op::ROR_ZPX);
    mem[0xFFFD] = zp_base;
    mem[0xFFFE] = op(Op::NOP);  // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(6, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result: carry(1) << 7 + 00000001 >> 1 = 10000000 (128 decimal)
    if (mem[zp_address] != 0b10000000) {
        throw testing::TestFailedException("ROR_ZPX test failed: Memory at ZP+X should be 0x80 (128 decimal)");
    }

    // Check flags
    if (!cpu.FLAGS_C) {
        throw testing::TestFailedException("ROR_ZPX test failed: Carry flag should be set (LSB was 1)");
    }
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("ROR_ZPX test failed: Negative flag should be set");
    }
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("ROR_ZPX test failed: Zero flag should be clear");
    }
}

void inline_ror_abs_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set up memory and initial carry flag
    word abs_address = 0x4242;
    mem[abs_address] = 0b00000010;  // 2 decimal
    cpu.FLAGS_C = 0;                // Clear carry initially

    // Load ROR Absolute instruction
    mem[0xFFFC] = op(Op::ROR_AB);
    mem[0xFFFD] = abs_address & 0xFF;         // Low byte
    mem[0xFFFE] = (abs_address >> 8) & 0xFF;  // High byte
    mem[0xFFFF] = op(Op::NOP);                // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(6, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result: carry(0) << 7 + 00000010 >> 1 = 00000001 (1 decimal)
    if (mem[abs_address] != 0b00000001) {
        throw testing::TestFailedException("ROR_AB test failed: Memory at absolute address should be 0x01");
    }

    // Check flags
    if (cpu.FLAGS_C) {
        throw testing::TestFailedException("ROR_AB test failed: Carry flag should be clear (LSB was 0)");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("ROR_AB test failed: Negative flag should be clear");
    }
    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("ROR_AB test failed: Zero flag should be clear");
    }
}

void inline_ror_absx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set up memory, X register, and initial carry flag
    word abs_base = 0x4240;
    byte x_offset = 0x02;
    word abs_address = abs_base + x_offset;  // 0x4242
    cpu.set(Register::X, x_offset);
    mem[abs_address] = 0b00000001;  // 1 decimal
    cpu.FLAGS_C = 0;                // Clear carry initially

    // Load ROR Absolute,X instruction
    mem[0xFFFC] = op(Op::ROR_ABSX);
    mem[0xFFFD] = abs_base & 0xFF;         // Low byte
    mem[0xFFFE] = (abs_base >> 8) & 0xFF;  // High byte
    mem[0xFFFF] = op(Op::NOP);             // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(7, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result: carry(0) << 7 + 00000001 >> 1 = 00000000 (0 decimal)
    if (mem[abs_address] != 0b00000000) {
        throw testing::TestFailedException("ROR_ABSX test failed: Memory at absolute address+X should be 0x00");
    }

    // Check flags
    if (!cpu.FLAGS_C) {
        throw testing::TestFailedException("ROR_ABSX test failed: Carry flag should be set (LSB was 1)");
    }
    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("ROR_ABSX test failed: Negative flag should be clear");
    }
    if (!cpu.FLAGS_Z) {
        throw testing::TestFailedException("ROR_ABSX test failed: Zero flag should be set (result is 0)");
    }
}

// Test suite function for shifts and rotates operations
void shifts_rotates_test_suite(Cpu& cpu, Mem& mem) {
    testing::TestSuite test_suite("Shifts and Rotates Operations");

    test_suite.print_header();

    // Register and run individual tests
    test_suite.register_test("ASL Accumulator (ASL A)", [&]() { inline_asl_a_test(cpu, mem); });
    test_suite.register_test("ASL Zero Page (ASL ZP)", [&]() { inline_asl_zp_test(cpu, mem); });
    test_suite.register_test("ASL Zero Page,X (ASL ZPX)", [&]() { inline_asl_zpx_test(cpu, mem); });
    test_suite.register_test("ASL Absolute (ASL AB)", [&]() { inline_asl_abs_test(cpu, mem); });
    test_suite.register_test("ASL Absolute,X (ASL ABSX)", [&]() { inline_asl_absx_test(cpu, mem); });

    test_suite.register_test("LSR Accumulator (LSR A)", [&]() { inline_lsr_a_test(cpu, mem); });
    test_suite.register_test("LSR Zero Page (LSR ZP)", [&]() { inline_lsr_zp_test(cpu, mem); });
    test_suite.register_test("LSR Zero Page,X (LSR ZPX)", [&]() { inline_lsr_zpx_test(cpu, mem); });
    test_suite.register_test("LSR Absolute (LSR AB)", [&]() { inline_lsr_abs_test(cpu, mem); });
    test_suite.register_test("LSR Absolute,X (LSR ABSX)", [&]() { inline_lsr_absx_test(cpu, mem); });

    test_suite.register_test("ROL Accumulator (ROL A)", [&]() { inline_rol_a_test(cpu, mem); });
    test_suite.register_test("ROL Zero Page (ROL ZP)", [&]() { inline_rol_zp_test(cpu, mem); });
    test_suite.register_test("ROL Zero Page,X (ROL ZPX)", [&]() { inline_rol_zpx_test(cpu, mem); });
    test_suite.register_test("ROL Absolute (ROL AB)", [&]() { inline_rol_abs_test(cpu, mem); });
    test_suite.register_test("ROL Absolute,X (ROL ABSX)", [&]() { inline_rol_absx_test(cpu, mem); });

    test_suite.register_test("ROR Accumulator (ROR A)", [&]() { inline_ror_a_test(cpu, mem); });
    test_suite.register_test("ROR Zero Page (ROR ZP)", [&]() { inline_ror_zp_test(cpu, mem); });
    test_suite.register_test("ROR Zero Page,X (ROR ZPX)", [&]() { inline_ror_zpx_test(cpu, mem); });
    test_suite.register_test("ROR Absolute (ROR AB)", [&]() { inline_ror_abs_test(cpu, mem); });
    test_suite.register_test("ROR Absolute,X (ROR ABSX)", [&]() { inline_ror_absx_test(cpu, mem); });

    // Print the results
    test_suite.print_results();
}

}  // namespace testing
