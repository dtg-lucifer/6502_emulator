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

// STA Tests
void inline_sta_zp_test(Cpu& cpu, Mem& mem);
void inline_sta_zpx_test(Cpu& cpu, Mem& mem);
void inline_sta_zpx_wrap_test(Cpu& cpu, Mem& mem);
void inline_sta_absolute_test(Cpu& cpu, Mem& mem);
void inline_sta_absx_test(Cpu& cpu, Mem& mem);
void inline_sta_absy_test(Cpu& cpu, Mem& mem);
void inline_sta_indx_test(Cpu& cpu, Mem& mem);
void inline_sta_indy_test(Cpu& cpu, Mem& mem);

// LDX Tests
void inline_ldx_test(Cpu& cpu, Mem& mem);
void inline_ldx_zp_test(Cpu& cpu, Mem& mem);
void inline_ldx_zpy_test(Cpu& cpu, Mem& mem);
void inline_ldx_zpy_wrap_test(Cpu& cpu, Mem& mem);
void inline_ldx_absolute_test(Cpu& cpu, Mem& mem);
void inline_ldx_absy_test(Cpu& cpu, Mem& mem);

// LDY Tests
void inline_ldy_test(Cpu& cpu, Mem& mem);
void inline_ldy_zp_test(Cpu& cpu, Mem& mem);
void inline_ldy_zpx_test(Cpu& cpu, Mem& mem);
void inline_ldy_zpx_wrap_test(Cpu& cpu, Mem& mem);
void inline_ldy_absolute_test(Cpu& cpu, Mem& mem);
void inline_ldy_absx_test(Cpu& cpu, Mem& mem);

// STX Tests
void inline_stx_zp_test(Cpu& cpu, Mem& mem);
void inline_stx_zpy_test(Cpu& cpu, Mem& mem);
void inline_stx_zpy_wrap_test(Cpu& cpu, Mem& mem);
void inline_stx_absolute_test(Cpu& cpu, Mem& mem);

// STY Tests
void inline_sty_zp_test(Cpu& cpu, Mem& mem);
void inline_sty_zpx_test(Cpu& cpu, Mem& mem);
void inline_sty_zpx_wrap_test(Cpu& cpu, Mem& mem);
void inline_sty_absolute_test(Cpu& cpu, Mem& mem);

// JSR/RTS Tests
void inline_jsr_rts_test(Cpu& cpu, Mem& mem);

// Invalid Opcode Test
void inline_invalid_opcode_test(Cpu& cpu, Mem& mem);

// Test suite functions
void sta_test_suite(Cpu& cpu, Mem& mem);
void stx_test_suite(Cpu& cpu, Mem& mem);
void sty_test_suite(Cpu& cpu, Mem& mem);

bool run_all_tests(Cpu& cpu, Mem& mem);
}  // namespace testing

#endif  // TEST_H
