#include <map>
#include <vector>

#include "cpu.h"
#include "memory.h"
#include "op_codes.h"
#include "reader.h"
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

    // Read a demo program from a in memory array
    std::map<u32, std::vector<byte> > demo = {
        {0xFFFC, {op(Op::JSR), 0x42, 0x43}}, {0x4342, {op(Op::LDA_IM), 0x84}}, {0x4344, {op(Op::RTS)}}};

    binary_reader::read_from_array(cpu, mem, demo);

    cpu.execute(8, mem);  // Execute with 100 cycles
#endif
    return 0;
}
