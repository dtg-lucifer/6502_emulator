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
void inline_lda_absolute_test(Cpu& cpu, Mem& mem);
void inline_lda_absx_test(Cpu& cpu, Mem& mem);
void inline_lda_absy_test(Cpu& cpu, Mem& mem);
void inline_lda_indx_test(Cpu& cpu, Mem& mem);
void inline_lda_indy_test(Cpu& cpu, Mem& mem);

// JSR/RTS Tests
void inline_jsr_rts_test(Cpu& cpu, Mem& mem);

// Invalid Opcode Test
void inline_invalid_opcode_test(Cpu& cpu, Mem& mem);

bool run_all_tests(Cpu& cpu, Mem& mem);
}  // namespace testing

#endif  // TEST_H
