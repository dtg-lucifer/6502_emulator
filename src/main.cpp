#include <iostream>

#include "cpu.h"
#include "demo_programs.h"
#include "memory.h"
#include "op_codes.h"
#include "reader.h"
#include "types.h"

using namespace colors;

int main() {
    Cpu cpu;
    Mem mem;

    std::cout << "\n==== RUNNING LDA INSTRUCTION DEMO ====\n";

    // First initialize memory
    mem.init();

    // Load demo program into memory before reset
    auto lda_demo = demo_programs::get_lda_demo();
    binary_reader::read_from_array(cpu, mem, lda_demo);

    // Reset CPU - this will now automatically load the PC from reset vector
    cpu.reset(mem);

    // Set initial register values after reset
    cpu.set(Register::X, 0x05);
    cpu.set(Register::Y, 0x02);

    std::cout << "CPU Program Counter after reset: 0x" << std::hex << cpu.PC << std::dec << std::endl;

    bool program_completed = false;
    i32 cycles_used = cpu.execute(0, mem, &program_completed);  // The cycle parameter is ignored, runs to completion

    std::cout << "\nLDA Demo completed in " << cycles_used << " cycles. Final CPU state:\n";
    cpu.print_state(cycles_used, program_completed);

    return 0;
}
