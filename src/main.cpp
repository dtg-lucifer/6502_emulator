#include <iostream>

#include "cpu.h"
#include "demo_programs.h"
#include "memory.h"
#include "reader.h"
#include "types.h"

using namespace colors;

int main() {
    Cpu cpu;
    Mem mem;

    std::cout << "\n==== RUNNING LDA INSTRUCTION DEMO ====\n";

    cpu.reset(mem);

    auto lda_demo = demo_programs::get_lda_demo();
    binary_reader::read_from_array(cpu, mem, lda_demo);

    bool program_completed = false;
    i32 cycles_used = cpu.execute(100, mem, &program_completed);

    std::cout << "\nLDA Demo completed. Final CPU state:\n";
    cpu.print_state(cycles_used, program_completed);

    // std::cout << "\n\n==== RUNNING LDX INSTRUCTION DEMO ====\n";

    // cpu.reset(mem);
    // cpu.set(Register::X, 0x05);
    // cpu.set(Register::Y, 0x02);

    // auto ldx_demo = demo_programs::get_ldx_demo();
    // binary_reader::read_from_array(cpu, mem, ldx_demo);

    // program_completed = false;
    // cycles_used = cpu.execute(100, mem, &program_completed);

    // std::cout << "\nLDX Demo completed. Final CPU state:\n";
    // cpu.print_state(cycles_used, program_completed);

    // std::cout << "\n\n==== RUNNING LDY INSTRUCTION DEMO ====\n";

    // cpu.reset(mem);
    // cpu.set(Register::X, 0x05);
    // cpu.set(Register::Y, 0x02);

    // auto ldy_demo = demo_programs::get_ldy_demo();
    // binary_reader::read_from_array(cpu, mem, ldy_demo);

    // program_completed = false;
    // cycles_used = cpu.execute(100, mem, &program_completed);

    // std::cout << "\nLDY Demo completed. Final CPU state:\n";
    // cpu.print_state(cycles_used, program_completed);

    return 0;
}
