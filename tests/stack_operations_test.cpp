#include "cpu.h"
#include "memory.h"
#include "op_codes.h"
#include "test_utils.h"

using namespace colors;

namespace testing {

void inline_pha_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set a known value in the accumulator
    cpu.set(Register::A, 0x42);

    // Record initial stack pointer value
    byte initial_sp = cpu.SP;

    // Test PHA instruction (Push Accumulator onto stack)
    mem[0xFFFC] = op(Op::PHA);  // PHA instruction
    mem[0xFFFD] = op(Op::NOP);  // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(3, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check that the stack pointer was decremented
    std::printf("%s>> Stack pointer after PHA: 0x%02X (Initial: 0x%02X)%s\n", CYAN, static_cast<int>(cpu.SP),
                static_cast<int>(initial_sp), RESET);

    // Check that the value was pushed to the stack
    std::printf("%s>> Value on stack at 0x01%02X: 0x%02X%s\n", CYAN, static_cast<int>(initial_sp),
                mem[0x0100 + initial_sp], RESET);

    // Verify with test assertions
    if (cpu.SP != initial_sp - 1) {
        throw testing::TestFailedException("PHA test failed: Stack pointer should be decremented by 1");
    }

    // Verify that accumulator value was pushed onto the stack
    if (mem[0x0100 + initial_sp] != 0x42) {
        throw testing::TestFailedException("PHA test failed: Accumulator value was not correctly pushed onto stack");
    }
}

void inline_php_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set up a specific processor status
    cpu.FLAGS_N = 1;  // Set negative flag
    cpu.FLAGS_Z = 0;  // Clear zero flag
    cpu.FLAGS_C = 1;  // Set carry flag
    cpu.FLAGS_V = 0;  // Clear overflow flag

    // Record expected status byte and initial stack pointer
    byte expected_status = (1 << 7) | (1 << 0) | (1 << 5) | (1 << 4);  // N=1, C=1, B=1, U=1
    byte initial_sp = cpu.SP;

    // Test PHP instruction (Push Processor Status onto stack)
    mem[0xFFFC] = op(Op::PHP);  // PHP instruction
    mem[0xFFFD] = op(Op::NOP);  // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(3, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check that the stack pointer was decremented
    std::printf("%s>> Stack pointer after PHP: 0x%02X (Initial: 0x%02X)%s\n", CYAN, static_cast<int>(cpu.SP),
                static_cast<int>(initial_sp), RESET);

    // Check that the status was pushed to the stack
    std::printf("%s>> Status on stack at 0x01%02X: 0x%02X%s\n", CYAN, static_cast<int>(initial_sp),
                mem[0x0100 + initial_sp], RESET);

    // Verify with test assertions
    if (cpu.SP != initial_sp - 1) {
        throw testing::TestFailedException("PHP test failed: Stack pointer should be decremented by 1");
    }

    // Note: The B flag and unused flag are set when pushing to stack with PHP
    if ((mem[0x0100 + initial_sp] & 0xCB) != (expected_status & 0xCB)) {
        std::stringstream ss;
        ss << "PHP test failed: Status was not correctly pushed onto stack. "
           << "Expected: 0x" << std::hex << static_cast<int>(expected_status & 0xCB) << " but got: 0x"
           << static_cast<int>(mem[0x0100 + initial_sp] & 0xCB);
        throw testing::TestFailedException(ss.str());
    }
}

void inline_pla_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Push a test value onto the stack
    byte initial_sp = cpu.SP;
    mem[0x0100 + initial_sp] = 0x37;  // Value to be pulled from stack
    cpu.SP--;                         // Manually update the stack pointer

    // Test PLA instruction (Pull Accumulator from stack)
    mem[0xFFFC] = op(Op::PLA);  // PLA instruction
    mem[0xFFFD] = op(Op::NOP);  // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check results
    std::printf("%s>> Stack pointer after PLA: 0x%02X (Initial: 0x%02X)%s\n", CYAN, static_cast<int>(cpu.SP),
                static_cast<int>(initial_sp - 1), RESET);
    std::printf("%s>> Accumulator after PLA: 0x%02X%s\n", CYAN, cpu.get(Register::A), RESET);

    // Verify with test assertions
    if (cpu.SP != initial_sp) {
        throw testing::TestFailedException(
            "PLA test failed: Stack pointer should be incremented back to initial value");
    }

    // Verify accumulator contains the value from the stack
    if (cpu.get(Register::A) != 0x37) {
        throw testing::TestFailedException("PLA test failed: Accumulator should contain 0x37");
    }

    // Verify flags are set correctly (should set N and Z flags based on value)
    if (cpu.FLAGS_N != 0) {
        throw testing::TestFailedException("PLA test failed: Negative flag should be clear for value 0x37");
    }
    if (cpu.FLAGS_Z != 0) {
        throw testing::TestFailedException("PLA test failed: Zero flag should be clear for non-zero value");
    }

    // Test with a negative value (bit 7 set)
    cpu.reset(mem);
    initial_sp = cpu.SP;
    mem[0x0100 + initial_sp] = 0x85;  // Negative value (bit 7 set)
    cpu.SP--;

    mem[0xFFFC] = op(Op::PLA);

    program_completed = false;
    cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Verify flags for negative value
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("PLA test failed: Negative flag should be set for value 0x85");
    }

    // Test with a zero value
    cpu.reset(mem);
    initial_sp = cpu.SP;
    mem[0x0100 + initial_sp] = 0x00;  // Zero value
    cpu.SP--;

    mem[0xFFFC] = op(Op::PLA);

    program_completed = false;
    cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Verify flags for zero value
    if (!cpu.FLAGS_Z) {
        throw testing::TestFailedException("PLA test failed: Zero flag should be set for value 0x00");
    }
}

void inline_plp_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set all flags to a known state first (all clear except Interrupt and Unused)
    cpu.FLAGS_N = 0;
    cpu.FLAGS_V = 0;
    cpu.FLAGS_B = 0;
    cpu.FLAGS_D = 0;
    cpu.FLAGS_I = 1;  // Set to 1 by reset
    cpu.FLAGS_Z = 0;
    cpu.FLAGS_C = 0;
    cpu.FLAGS_U = 1;  // Unused flag is set by default

    // Push a test status value onto the stack
    byte initial_sp = cpu.SP;
    // Status with N, D, Z, C set (10001011)
    byte test_status = 0x8B;
    mem[0x0100 + initial_sp] = test_status;
    cpu.SP--;  // Manually update the stack pointer

    // Test PLP instruction (Pull Processor Status from stack)
    mem[0xFFFC] = op(Op::PLP);  // PLP instruction
    mem[0xFFFD] = op(Op::NOP);  // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(4, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check results
    std::printf("%s>> Stack pointer after PLP: 0x%02X (Initial: 0x%02X)%s\n", CYAN, static_cast<int>(cpu.SP),
                static_cast<int>(initial_sp - 1), RESET);

    // Verify with test assertions
    if (cpu.SP != initial_sp) {
        throw testing::TestFailedException(
            "PLP test failed: Stack pointer should be incremented back to initial value");
    }

    // Verify status flags match what was pulled from the stack
    // Note: The B flag is not affected by PLP in actual hardware
    if (cpu.FLAGS != test_status) {
        std::stringstream ss;
        ss << "PLP test failed: Status flags incorrect. "
           << "Expected: N=1, Z=1, C=1, D=1 but got: "
           << "N=" << static_cast<int>(cpu.FLAGS_N) << ", Z=" << static_cast<int>(cpu.FLAGS_Z)
           << ", C=" << static_cast<int>(cpu.FLAGS_C) << ", D=" << static_cast<int>(cpu.FLAGS_D) << std::endl;
        throw testing::TestFailedException(ss.str());
    }
}

void inline_tsx_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set X to a different value to ensure it's overwritten
    cpu.set(Register::X, 0x00);

    // Remember the current SP value
    byte initial_sp = cpu.SP;

    // Test TSX instruction (Transfer Stack Pointer to X)
    mem[0xFFFC] = op(Op::TSX);  // TSX instruction
    mem[0xFFFD] = op(Op::NOP);  // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result
    std::printf("%s>> X register after TSX: 0x%02X (SP: 0x%02X)%s\n", CYAN, static_cast<int>(cpu.get(Register::X)),
                static_cast<int>(initial_sp), RESET);

    // Verify with test assertions
    if (cpu.get(Register::X) != initial_sp) {
        throw testing::TestFailedException("TSX test failed: X register should contain stack pointer value");
    }

    // Verify stack pointer hasn't changed
    if (cpu.SP != initial_sp) {
        throw testing::TestFailedException("TSX test failed: Stack pointer shouldn't change");
    }

    // Verify flags are set correctly
    // SP is typically 0xFF after reset, which should set N flag (bit 7 set)
    if (!cpu.FLAGS_N) {
        throw testing::TestFailedException("TSX test failed: Negative flag should be set when bit 7 is set");
    }

    if (cpu.FLAGS_Z) {
        throw testing::TestFailedException("TSX test failed: Zero flag should be clear for non-zero value");
    }

    // Test with a zero value for SP
    cpu.reset(mem);
    cpu.SP = 0x00;  // Set SP to zero

    mem[0xFFFC] = op(Op::TSX);

    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Verify flags for zero value
    if (!cpu.FLAGS_Z) {
        throw testing::TestFailedException("TSX test failed: Zero flag should be set when SP is zero");
    }

    if (cpu.FLAGS_N) {
        throw testing::TestFailedException("TSX test failed: Negative flag should be clear when SP bit 7 is clear");
    }
}

void inline_txs_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Set SP to a known initial value
    cpu.SP = 0xAA;

    byte initial_flags = cpu.FLAGS;  // Save initial flags for verification

    // Set X to a specific value
    cpu.set(Register::X, 0x42);

    // Test TXS instruction (Transfer X to Stack Pointer)
    mem[0xFFFC] = op(Op::TXS);  // TXS instruction
    mem[0xFFFD] = op(Op::NOP);  // NOP to terminate test

    bool program_completed = false;
    i32 cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Print cycles used and completion status
    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check result
    std::printf("%s>> Stack pointer after TXS: 0x%02X (X: 0x%02X)%s\n", CYAN, static_cast<int>(cpu.SP),
                static_cast<int>(cpu.get(Register::X)), RESET);

    // Verify with test assertions
    if (cpu.SP != 0x42) {
        throw testing::TestFailedException("TXS test failed: Stack pointer should contain X register value (0x42)");
    }

    // Verify X hasn't changed
    if (cpu.get(Register::X) != 0x42) {
        throw testing::TestFailedException("TXS test failed: X register shouldn't change");
    }

    // Verify flags haven't changed (TXS doesn't affect flags)
    // Keep the test simple: TXS is documented to not affect any flags
    // Note: We're using the flags set in cpu.reset() as reference
    if (cpu.FLAGS != initial_flags) {
        throw testing::TestFailedException("TXS test failed: Status flags shouldn't be affected");
    }
}

// Test suite function for stack operations
void stack_operations_test_suite(Cpu& cpu, Mem& mem) {
    testing::TestSuite test_suite("Stack Operations");

    test_suite.print_header();

    // Register and run individual tests
    test_suite.register_test("Push Accumulator (PHA)", [&]() { inline_pha_test(cpu, mem); });
    test_suite.register_test("Push Processor Status (PHP)", [&]() { inline_php_test(cpu, mem); });
    test_suite.register_test("Pull Accumulator (PLA)", [&]() { inline_pla_test(cpu, mem); });
    test_suite.register_test("Pull Processor Status (PLP)", [&]() { inline_plp_test(cpu, mem); });
    test_suite.register_test("Transfer Stack Pointer to X (TSX)", [&]() { inline_tsx_test(cpu, mem); });
    test_suite.register_test("Transfer X to Stack Pointer (TXS)", [&]() { inline_txs_test(cpu, mem); });

    // Print the results
    test_suite.print_results();
}

}  // namespace testing
