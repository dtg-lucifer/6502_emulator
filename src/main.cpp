#include <iostream>

#include "cpu.h"
#include "memory.h"
#include "op_codes.h"
#include "types.h"

int main() {
    Cpu cpu;
    Mem mem;

    cpu.reset(mem);

    // start - inline test
    mem[0xFFFC] = op(Op::JSR);
    mem[0xFFFD] = 0x42;
    mem[0xFFFE] = 0x42;

    // set something at 0x4242
    mem[0x4242] = op(Op::LDA_IM);
    mem[0x4243] = 0x84;
    // add RTS instruction at the end of our subroutine
    // mem[0x4244] = op(Op::RTS);
    // end - inline test

    cpu.execute(9, mem);  // increased cycle count to include RTS execution

    return 0;
}
