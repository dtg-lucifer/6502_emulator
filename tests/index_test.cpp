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

    // Print the results
    test_suite_lda.print_results();

    testing::TestSuite test_suite_jsr_rts("JSR / RTS Op Code");

    test_suite_jsr_rts.print_header();

    // Register and run JSR/RTS tests
    test_suite_jsr_rts.register_test("Inline JSR/RTS Test", [&]() { inline_jsr_rts_test(cpu, mem); });

    // Print test results
    test_suite_jsr_rts.print_results();

    // Return true if all tests passed
    int failed_count = test_suite_lda.get_failed_count() + test_suite_jsr_rts.get_failed_count();
    return failed_count == 0;
}
}  // namespace testing
