#include "cpu.h"
#include "memory.h"
#include "test.h"

int main() {
    Cpu cpu;
    Mem mem;
    cpu.reset(mem, false);  // Don't use reset vector in tests

    return testing::run_all_tests(cpu, mem) ? 0 : 1;
}
