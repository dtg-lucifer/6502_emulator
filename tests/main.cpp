#include "cpu.h"
#include "memory.h"
#include "test.h"

int main() {
    Cpu cpu;
    Mem mem;
    cpu.reset(mem);

    return testing::run_all_tests(cpu, mem) ? 0 : 1;
}
