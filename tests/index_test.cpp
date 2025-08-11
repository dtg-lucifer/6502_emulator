#include "cpu.h"
#include "memory.h"
#include "test.h"
#include "test_utils.h"

using namespace colors;  // Make color codes more accessible

namespace testing {
// Run all tests and return true if all tests pass
bool run_all_tests(Cpu& cpu, Mem& mem) {
    testing::TestSuite test_suite_lda("LDA Op Code");

    test_suite_lda.print_header();

    // Register and run individual tests
    test_suite_lda.register_test("Inline LDA Test", [&]() { inline_lda_test(cpu, mem); });
    test_suite_lda.register_test("Inline LDA ZP Test", [&]() { inline_lda_zp_test(cpu, mem); });
    test_suite_lda.register_test("Inline LDA ZPX Test", [&]() { inline_lda_zpx_test(cpu, mem); });
    test_suite_lda.register_test("Inline LDA ZPX (Wrapping) Test", [&]() { inline_lda_zpx_wrap_test(cpu, mem); });
    test_suite_lda.register_test("Inline LDA Absolute Test", [&]() { inline_lda_absolute_test(cpu, mem); });
    test_suite_lda.register_test("Inline LDA ABSX Test", [&]() { inline_lda_absx_test(cpu, mem); });
    test_suite_lda.register_test("Inline LDA ABSY Test", [&]() { inline_lda_absy_test(cpu, mem); });
    test_suite_lda.register_test("Inline LDA INDX Test", [&]() { inline_lda_indx_test(cpu, mem); });
    test_suite_lda.register_test("Inline LDA INDY Test", [&]() { inline_lda_indy_test(cpu, mem); });

    // Print the results
    test_suite_lda.print_results();

    testing::TestSuite test_suite_jsr_rts("JSR / RTS Op Code");

    test_suite_jsr_rts.print_header();

    // Register and run JSR/RTS tests
    test_suite_jsr_rts.register_test("Inline JSR/RTS Test", [&]() { inline_jsr_rts_test(cpu, mem); });

    // Print test results
    test_suite_jsr_rts.print_results();

    testing::TestSuite test_suite_invalid_opcode("Invalid Opcode Test");

    test_suite_invalid_opcode.print_header();

    // Register and run invalid opcode test
    test_suite_invalid_opcode.register_test("Inline Invalid Opcode Test",
                                            [&]() { inline_invalid_opcode_test(cpu, mem); });

    // Print test results
    test_suite_invalid_opcode.print_results();

    testing::TestSuite test_suite_ldx("LDX Op Code");

    test_suite_ldx.print_header();

    // Register and run individual tests
    test_suite_ldx.register_test("Inline LDX Test", [&]() { inline_ldx_test(cpu, mem); });
    test_suite_ldx.register_test("Inline LDX ZP Test", [&]() { inline_ldx_zp_test(cpu, mem); });
    test_suite_ldx.register_test("Inline LDX ZPY Test", [&]() { inline_ldx_zpy_test(cpu, mem); });
    test_suite_ldx.register_test("Inline LDX ZPY (Wrapping) Test", [&]() { inline_ldx_zpy_wrap_test(cpu, mem); });
    test_suite_ldx.register_test("Inline LDX Absolute Test", [&]() { inline_ldx_absolute_test(cpu, mem); });
    test_suite_ldx.register_test("Inline LDX ABSY Test", [&]() { inline_ldx_absy_test(cpu, mem); });

    // Print the results
    test_suite_ldx.print_results();

    testing::TestSuite test_suite_ldy("LDY Op Code");

    test_suite_ldy.print_header();

    // Register and run individual tests
    test_suite_ldy.register_test("Inline LDY Test", [&]() { inline_ldy_test(cpu, mem); });
    test_suite_ldy.register_test("Inline LDY ZP Test", [&]() { inline_ldy_zp_test(cpu, mem); });
    test_suite_ldy.register_test("Inline LDY ZPX Test", [&]() { inline_ldy_zpx_test(cpu, mem); });
    test_suite_ldy.register_test("Inline LDY ZPX (Wrapping) Test", [&]() { inline_ldy_zpx_wrap_test(cpu, mem); });
    test_suite_ldy.register_test("Inline LDY Absolute Test", [&]() { inline_ldy_absolute_test(cpu, mem); });
    test_suite_ldy.register_test("Inline LDY ABSX Test", [&]() { inline_ldy_absx_test(cpu, mem); });

    // Print the results
    test_suite_ldy.print_results();

    // Add new test suites for STA, STX, and STY
    testing::TestSuite test_suite_sta("STA Op Code");
    test_suite_sta.print_header();

    // Register STA tests
    test_suite_sta.register_test("Inline STA Zero Page Test", [&]() { inline_sta_zp_test(cpu, mem); });
    test_suite_sta.register_test("Inline STA Zero Page,X Test", [&]() { inline_sta_zpx_test(cpu, mem); });
    test_suite_sta.register_test("Inline STA Zero Page,X (Wrapping) Test",
                                 [&]() { inline_sta_zpx_wrap_test(cpu, mem); });
    test_suite_sta.register_test("Inline STA Absolute Test", [&]() { inline_sta_absolute_test(cpu, mem); });
    test_suite_sta.register_test("Inline STA Absolute,X Test", [&]() { inline_sta_absx_test(cpu, mem); });
    test_suite_sta.register_test("Inline STA Absolute,Y Test", [&]() { inline_sta_absy_test(cpu, mem); });
    test_suite_sta.register_test("Inline STA Indirect,X Test", [&]() { inline_sta_indx_test(cpu, mem); });
    test_suite_sta.register_test("Inline STA Indirect,Y Test", [&]() { inline_sta_indy_test(cpu, mem); });

    test_suite_sta.print_results();

    testing::TestSuite test_suite_stx("STX Op Code");
    test_suite_stx.print_header();

    // Register STX tests
    test_suite_stx.register_test("Inline STX Zero Page Test", [&]() { inline_stx_zp_test(cpu, mem); });
    test_suite_stx.register_test("Inline STX Zero Page,Y Test", [&]() { inline_stx_zpy_test(cpu, mem); });
    test_suite_stx.register_test("Inline STX Zero Page,Y (Wrapping) Test",
                                 [&]() { inline_stx_zpy_wrap_test(cpu, mem); });
    test_suite_stx.register_test("Inline STX Absolute Test", [&]() { inline_stx_absolute_test(cpu, mem); });

    test_suite_stx.print_results();

    testing::TestSuite test_suite_sty("STY Op Code");
    test_suite_sty.print_header();

    // Register STY tests
    test_suite_sty.register_test("Inline STY Zero Page Test", [&]() { inline_sty_zp_test(cpu, mem); });
    test_suite_sty.register_test("Inline STY Zero Page,X Test", [&]() { inline_sty_zpx_test(cpu, mem); });
    test_suite_sty.register_test("Inline STY Zero Page,X (Wrapping) Test",
                                 [&]() { inline_sty_zpx_wrap_test(cpu, mem); });
    test_suite_sty.register_test("Inline STY Absolute Test", [&]() { inline_sty_absolute_test(cpu, mem); });

    test_suite_sty.print_results();

    // Run JMP tests
    jmp_test_suite(cpu, mem);

    // Return true if all tests passed
    // Since the JMP test suite doesn't return a failed count, we're assuming it's successful
    // if the execution reaches this point (as failed tests throw exceptions)
    int failed_count = test_suite_lda.get_failed_count() + test_suite_jsr_rts.get_failed_count() +
                       test_suite_invalid_opcode.get_failed_count() + test_suite_ldx.get_failed_count() +
                       test_suite_ldy.get_failed_count() + test_suite_sta.get_failed_count() +
                       test_suite_stx.get_failed_count() + test_suite_sty.get_failed_count();

    return failed_count == 0;
}
}  // namespace testing
