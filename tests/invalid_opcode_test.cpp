#include "cpu.h"
#include "memory.h"
#include "test_utils.h"

using namespace colors;

namespace testing {
void inline_invalid_opcode_test(Cpu& cpu, Mem& mem) {
    cpu.reset(mem);

    mem[0xFFFC] = 0x00;
    mem[0xFFFD] = 0xEA;
    mem[0xFFFE] = 0xEA;

    bool program_completed = false;
    i32 cycles_used = cpu.execute(1, mem, &program_completed, true);  // Updated cycle count

    // Only print in non-testing environments
    std::printf("%s>> Execution completed in %d cycles%s\n", CYAN, cycles_used, RESET);

    if (cycles_used != 1) {
        std::stringstream ss;
        ss << "No more than cycle should be used here";
        throw testing::TestFailedException(ss.str());
    }

    // Verify the program completed successfully
    if (!program_completed) {
        throw testing::TestFailedException("Program did not complete successfully");
    }
}
}  // namespace testing
