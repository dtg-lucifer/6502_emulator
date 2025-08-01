#include <iostream>
#include <map>
#include <vector>

#include "cpu.h"
#include "memory.h"
#include "op_codes.h"
#include "reader.h"
#include "test_utils.h"
#include "types.h"

using namespace colors;

int main() {
    Cpu cpu;
    Mem mem;
    cpu.reset(mem);

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
        {0xFFFC, {op(Op::JSR), 0x42, 0x43}},
        {0x4342, {op(Op::LDA_IM), 0x84}},
        {0x4344, {op(Op::RTS)}},
    };

    binary_reader::read_from_array(cpu, mem, demo);

    bool program_completed = false;
    i32 cycles_used = cpu.execute(5, mem, &program_completed);
    std::cout << "Execution " << BOLD << (program_completed ? GREEN : RED)
              << (program_completed ? "COMPLETED" : "INCOMPLETE") << RESET << " using " << cycles_used << " cycles"
              << std::endl;

    // Reset CPU and memory
    cpu.reset(mem);
    binary_reader::read_from_array(cpu, mem, demo);

    program_completed = false;
    cycles_used = cpu.execute(15, mem, &program_completed);
    std::cout << "Execution " << BOLD << (program_completed ? GREEN : RED)
              << (program_completed ? "COMPLETED" : "INCOMPLETE") << RESET << " using " << cycles_used << " cycles"
              << std::endl;

    return 0;
}
