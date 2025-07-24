#include "cpu.h"
#include "memory.h"
#include "op_codes.h"
#include "test.h"
#include "types.h"

int main() {
    Cpu cpu;
    Mem mem;
    cpu.reset(mem);

#if defined(__TESTING)
    testing::run_all_tests();
#else
    // Normal operation mode (not testing)

    // Program flow:
    //  - load a C-64 assembly code
    //  - parse it
    //  - load into a binary format
    //  - feed that to the memory to read
    //  - execute the script
    //  - print the final state after halting
#endif
    return 0;
}
