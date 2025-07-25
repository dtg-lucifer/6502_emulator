#ifndef TEST_H
#define TEST_H

#include "cpu.h"
#include "memory.h"

namespace testing {
// LDA Tests
void inline_lda_test(Cpu& cpu, Mem& mem);
void inline_lda_zp_test(Cpu& cpu, Mem& mem);
void inline_lda_zpx_test(Cpu& cpu, Mem& mem);
void inline_lda_zpx_wrap_test(Cpu& cpu, Mem& mem);

void inline_jsr_rts_test(Cpu& cpu, Mem& mem);
bool run_all_tests(Cpu& cpu, Mem& mem);
}  // namespace testing

#endif  // TEST_H
