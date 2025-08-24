#include "cpu.h"
#include "memory.h"
#include "op_codes.h"
#include "test_utils.h"

using namespace colors;

namespace testing {

// Test BCC (Branch if Carry Clear)
void inline_bcc_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Test case 1: Branch taken (carry clear)
    cpu.FLAGS_C = 0;  // Clear carry flag

    // Store PC before execution
    word pc_before = cpu.PC;

    // Set up branch instruction - branch forward by 10 bytes
    mem[0xFFFC] = op(Op::BCC);
    mem[0xFFFD] = 0x0A;  // Branch offset
    mem[0xFFFE] = op(Op::NOP);

    // Put a marker instruction at the branch target
    // Make sure we don't branch out of bounds
    {
        word branch_target = (pc_before + 2 + 0x0A) & 0xFFFF;
        mem[branch_target] = op(Op::NOP);  // This is where we should end up
    }

    bool program_completed = false;
    i32 cycles_used = cpu.execute(3, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check that we branched to the correct location
    {
        word expected_pc = (pc_before + 2 + 0x0A) & 0xFFFF;
        if (cpu.PC != expected_pc) {
            throw testing::TestFailedException("BCC test failed: Branch should be taken when carry flag is clear");
        }
    }

    // Test case 2: Branch not taken (carry set)
    cpu.reset(mem);
    cpu.FLAGS_C = 1;  // Set carry flag

    // PC before execution
    pc_before = cpu.PC;
    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Check that we did not branch
    {
        word expected_pc = (pc_before + 2) & 0xFFFF;
        if (cpu.PC != expected_pc) {
            throw testing::TestFailedException("BCC test failed: Branch should not be taken when carry flag is set");
        }
    }

    // Test case 3: Branch backward
    cpu.reset(mem);
    cpu.FLAGS_C = 0;  // Clear carry flag

    // Move PC forward so we can branch backward
    cpu.PC = 0x5000;

    // Set up branch instruction - branch backward by 10 bytes
    mem[0x5000] = op(Op::BCC);
    mem[0x5001] = 0xF6;  // Branch offset (-10 in two's complement)
    mem[0x5002] = op(Op::NOP);

    // Put a marker instruction at the branch target
    {
        word branch_target = (0x5000 + 2 - 10) & 0xFFFF;
        mem[branch_target] = op(Op::NOP);  // This is where we should end up
    }

    pc_before = cpu.PC;
    program_completed = false;
    cycles_used = cpu.execute(3, mem, &program_completed, true);

    // Check that we branched to the correct location
    {
        word expected_pc = (pc_before + 2 - 10) & 0xFFFF;
        if (cpu.PC != expected_pc) {
            throw testing::TestFailedException("BCC test failed: Branch backward should work correctly");
        }
    }

    // Test case 4: Branch crosses page boundary (adds an extra cycle)
    cpu.reset(mem);
    cpu.FLAGS_C = 0;  // Clear carry flag

    // Set PC to end of a page
    cpu.PC = 0x50FE;

    // Set up branch instruction - branch forward to next page
    mem[0x50FE] = op(Op::BCC);
    mem[0x50FF] = 0x10;  // Branch offset (+16)

    // Make sure we don't access memory out of bounds
    {
        word branch_target = (0x50FE + 2 + 0x10) & 0xFFFF;
        mem[branch_target] = op(Op::NOP);
    }

    pc_before = cpu.PC;
    program_completed = false;
    cycles_used = cpu.execute(4, mem, &program_completed, true);  // Should take 4 cycles (cross page)

    // Check that we branched to the correct location
    {
        word expected_pc = (pc_before + 2 + 0x10) & 0xFFFF;
        if (cpu.PC != expected_pc) {
            throw testing::TestFailedException("BCC test failed: Branch across page boundary should work correctly");
        }
    }
}

// Test BCS (Branch if Carry Set)
void inline_bcs_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Test case 1: Branch taken (carry set)
    cpu.FLAGS_C = 1;  // Set carry flag

    // Store PC before execution
    word pc_before = cpu.PC;

    // Set up branch instruction - branch forward by 10 bytes
    mem[0xFFFC] = op(Op::BCS);
    mem[0xFFFD] = 0x0A;  // Branch offset
    mem[0xFFFE] = op(Op::NOP);

    // Put a marker instruction at the branch target
    {
        word branch_target = (pc_before + 2 + 0x0A) & 0xFFFF;
        mem[branch_target] = op(Op::NOP);  // This is where we should end up
    }

    bool program_completed = false;
    i32 cycles_used = cpu.execute(3, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check that we branched to the correct location
    {
        word expected_pc = (pc_before + 2 + 0x0A) & 0xFFFF;
        if (cpu.PC != expected_pc) {
            throw testing::TestFailedException("BCS test failed: Branch should be taken when carry flag is set");
        }
    }

    // Test case 2: Branch not taken (carry clear)
    cpu.reset(mem);
    cpu.FLAGS_C = 0;  // Clear carry flag

    // PC before execution
    pc_before = cpu.PC;
    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Check that we did not branch
    {
        word expected_pc = (pc_before + 2) & 0xFFFF;
        if (cpu.PC != expected_pc) {
            throw testing::TestFailedException("BCS test failed: Branch should not be taken when carry flag is clear");
        }
    }
}

// Test BEQ (Branch if Equal/Zero)
void inline_beq_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Test case 1: Branch taken (zero flag set)
    cpu.FLAGS_Z = 1;  // Set zero flag

    // Store PC before execution
    word pc_before = cpu.PC;

    // Set up branch instruction - branch forward by 10 bytes
    mem[0xFFFC] = op(Op::BEQ);
    mem[0xFFFD] = 0x0A;  // Branch offset
    mem[0xFFFE] = op(Op::NOP);

    // Put a marker instruction at the branch target
    {
        word branch_target = (pc_before + 2 + 0x0A) & 0xFFFF;
        mem[branch_target] = op(Op::NOP);  // This is where we should end up
    }

    bool program_completed = false;
    i32 cycles_used = cpu.execute(3, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check that we branched to the correct location
    {
        word expected_pc = (pc_before + 2 + 0x0A) & 0xFFFF;
        if (cpu.PC != expected_pc) {
            throw testing::TestFailedException("BEQ test failed: Branch should be taken when zero flag is set");
        }
    }

    // Test case 2: Branch not taken (zero flag clear)
    cpu.reset(mem);
    cpu.FLAGS_Z = 0;  // Clear zero flag

    // PC before execution
    pc_before = cpu.PC;
    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Check that we did not branch
    {
        word expected_pc = (pc_before + 2) & 0xFFFF;
        if (cpu.PC != expected_pc) {
            throw testing::TestFailedException("BEQ test failed: Branch should not be taken when zero flag is clear");
        }
    }
}

// Test BMI (Branch if Minus/Negative)
void inline_bmi_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Test case 1: Branch taken (negative flag set)
    cpu.FLAGS_N = 1;  // Set negative flag

    // Store PC before execution
    word pc_before = cpu.PC;

    // Set up branch instruction - branch forward by 10 bytes
    mem[0xFFFC] = op(Op::BMI);
    mem[0xFFFD] = 0x0A;  // Branch offset
    mem[0xFFFE] = op(Op::NOP);

    // Put a marker instruction at the branch target
    {
        word branch_target = (pc_before + 2 + 0x0A) & 0xFFFF;
        mem[branch_target] = op(Op::NOP);  // This is where we should end up
    }

    bool program_completed = false;
    i32 cycles_used = cpu.execute(3, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check that we branched to the correct location
    {
        word expected_pc = (pc_before + 2 + 0x0A) & 0xFFFF;
        if (cpu.PC != expected_pc) {
            throw testing::TestFailedException("BMI test failed: Branch should be taken when negative flag is set");
        }
    }

    // Test case 2: Branch not taken (negative flag clear)
    cpu.reset(mem);
    cpu.FLAGS_N = 0;  // Clear negative flag

    // PC before execution
    pc_before = cpu.PC;
    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Check that we did not branch
    {
        word expected_pc = (pc_before + 2) & 0xFFFF;
        if (cpu.PC != expected_pc) {
            throw testing::TestFailedException(
                "BMI test failed: Branch should not be taken when negative flag is clear");
        }
    }
}

// Test BNE (Branch if Not Equal/Zero Clear)
void inline_bne_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Test case 1: Branch taken (zero flag clear)
    cpu.FLAGS_Z = 0;  // Clear zero flag

    // Store PC before execution
    word pc_before = cpu.PC;

    // Set up branch instruction - branch forward by 10 bytes
    mem[0xFFFC] = op(Op::BNE);
    mem[0xFFFD] = 0x0A;  // Branch offset
    mem[0xFFFE] = op(Op::NOP);

    // Put a marker instruction at the branch target
    {
        word branch_target = (pc_before + 2 + 0x0A) & 0xFFFF;
        mem[branch_target] = op(Op::NOP);  // This is where we should end up
    }

    bool program_completed = false;
    i32 cycles_used = cpu.execute(3, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check that we branched to the correct location
    {
        word expected_pc = (pc_before + 2 + 0x0A) & 0xFFFF;
        if (cpu.PC != expected_pc) {
            throw testing::TestFailedException("BNE test failed: Branch should be taken when zero flag is clear");
        }
    }

    // Test case 2: Branch not taken (zero flag set)
    cpu.reset(mem);
    cpu.FLAGS_Z = 1;  // Set zero flag

    // PC before execution
    pc_before = cpu.PC;
    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Check that we did not branch
    {
        word expected_pc = (pc_before + 2) & 0xFFFF;
        if (cpu.PC != expected_pc) {
            throw testing::TestFailedException("BNE test failed: Branch should not be taken when zero flag is set");
        }
    }
}

// Test BPL (Branch if Plus/Positive)
void inline_bpl_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Test case 1: Branch taken (negative flag clear)
    cpu.FLAGS_N = 0;  // Clear negative flag

    // Store PC before execution
    word pc_before = cpu.PC;

    // Set up branch instruction - branch forward by 10 bytes
    mem[0xFFFC] = op(Op::BPL);
    mem[0xFFFD] = 0x0A;  // Branch offset
    mem[0xFFFE] = op(Op::NOP);

    // Put a marker instruction at the branch target
    {
        word branch_target = (pc_before + 2 + 0x0A) & 0xFFFF;
        mem[branch_target] = op(Op::NOP);  // This is where we should end up
    }

    bool program_completed = false;
    i32 cycles_used = cpu.execute(3, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check that we branched to the correct location
    {
        word expected_pc = (pc_before + 2 + 0x0A) & 0xFFFF;
        if (cpu.PC != expected_pc) {
            throw testing::TestFailedException("BPL test failed: Branch should be taken when negative flag is clear");
        }
    }

    // Test case 2: Branch not taken (negative flag set)
    cpu.reset(mem);
    cpu.FLAGS_N = 1;  // Set negative flag

    // PC before execution
    pc_before = cpu.PC;
    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Check that we did not branch
    {
        word expected_pc = (pc_before + 2) & 0xFFFF;
        if (cpu.PC != expected_pc) {
            throw testing::TestFailedException("BPL test failed: Branch should not be taken when negative flag is set");
        }
    }
}

// Test BVC (Branch if Overflow Clear)
void inline_bvc_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Test case 1: Branch taken (overflow flag clear)
    cpu.FLAGS_V = 0;  // Clear overflow flag

    // Store PC before execution
    word pc_before = cpu.PC;

    // Set up branch instruction - branch forward by 10 bytes
    mem[0xFFFC] = op(Op::BVC);
    mem[0xFFFD] = 0x0A;  // Branch offset
    mem[0xFFFE] = op(Op::NOP);

    // Put a marker instruction at the branch target
    {
        word branch_target = (pc_before + 2 + 0x0A) & 0xFFFF;
        mem[branch_target] = op(Op::NOP);  // This is where we should end up
    }

    bool program_completed = false;
    i32 cycles_used = cpu.execute(3, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check that we branched to the correct location
    {
        word expected_pc = (pc_before + 2 + 0x0A) & 0xFFFF;
        if (cpu.PC != expected_pc) {
            throw testing::TestFailedException("BVC test failed: Branch should be taken when overflow flag is clear");
        }
    }

    // Test case 2: Branch not taken (overflow flag set)
    cpu.reset(mem);
    cpu.FLAGS_V = 1;  // Set overflow flag

    // PC before execution
    pc_before = cpu.PC;
    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Check that we did not branch
    {
        word expected_pc = (pc_before + 2) & 0xFFFF;
        if (cpu.PC != expected_pc) {
            throw testing::TestFailedException("BVC test failed: Branch should not be taken when overflow flag is set");
        }
    }
}

// Test BVS (Branch if Overflow Set)
void inline_bvs_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    // Test case 1: Branch taken (overflow flag set)
    cpu.FLAGS_V = 1;  // Set overflow flag

    // Store PC before execution
    word pc_before = cpu.PC;

    // Set up branch instruction - branch forward by 10 bytes
    mem[0xFFFC] = op(Op::BVS);
    mem[0xFFFD] = 0x0A;  // Branch offset
    mem[0xFFFE] = op(Op::NOP);

    // Put a marker instruction at the branch target
    {
        word branch_target = (pc_before + 2 + 0x0A) & 0xFFFF;
        mem[branch_target] = op(Op::NOP);  // This is where we should end up
    }

    bool program_completed = false;
    i32 cycles_used = cpu.execute(3, mem, &program_completed, true);

    std::printf("%sExecution %scompleted in %d cycles%s\n", CYAN, program_completed ? "successfully " : "in",
                cycles_used, RESET);

    // Check that we branched to the correct location
    {
        word expected_pc = (pc_before + 2 + 0x0A) & 0xFFFF;
        if (cpu.PC != expected_pc) {
            throw testing::TestFailedException("BVS test failed: Branch should be taken when overflow flag is set");
        }
    }

    // Test case 2: Branch not taken (overflow flag clear)
    cpu.reset(mem);
    cpu.FLAGS_V = 0;  // Clear overflow flag

    // PC before execution
    pc_before = cpu.PC;
    program_completed = false;
    cycles_used = cpu.execute(2, mem, &program_completed, true);

    // Check that we did not branch
    {
        word expected_pc = (pc_before + 2) & 0xFFFF;
        if (cpu.PC != expected_pc) {
            throw testing::TestFailedException(
                "BVS test failed: Branch should not be taken when overflow flag is clear");
        }
    }
}

// Test suite function for branch operations
void branch_test_suite(Cpu& cpu, Mem& mem) {
    testing::TestSuite test_suite("Branch Operations");

    test_suite.print_header();

    // Register and run individual tests
    test_suite.register_test("Branch if Carry Clear (BCC)", [&]() { inline_bcc_test(cpu, mem); });
    test_suite.register_test("Branch if Carry Set (BCS)", [&]() { inline_bcs_test(cpu, mem); });
    test_suite.register_test("Branch if Equal/Zero (BEQ)", [&]() { inline_beq_test(cpu, mem); });
    test_suite.register_test("Branch if Minus/Negative (BMI)", [&]() { inline_bmi_test(cpu, mem); });
    test_suite.register_test("Branch if Not Equal/Zero Clear (BNE)", [&]() { inline_bne_test(cpu, mem); });
    test_suite.register_test("Branch if Plus/Positive (BPL)", [&]() { inline_bpl_test(cpu, mem); });
    test_suite.register_test("Branch if Overflow Clear (BVC)", [&]() { inline_bvc_test(cpu, mem); });
    test_suite.register_test("Branch if Overflow Set (BVS)", [&]() { inline_bvs_test(cpu, mem); });

    // Print the results
    test_suite.print_results();
}

}  // namespace testing
