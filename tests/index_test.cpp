#include <iostream>

#include "cpu.h"
#include "memory.h"
#include "test.h"
#include "test_utils.h"

using namespace colors;  // Make color codes more accessible

namespace testing {
// Run all tests and return true if all tests pass
bool run_all_tests(Cpu& cpu, Mem& mem) {
    testing::TestSuite test_suite("6502 CPU Tests");

    std::cout << "\n" << YELLOW << BOLD << "===== Running 6502 CPU Tests =====" << RESET << "\n" << std::endl;

    // Register and run individual tests
    test_suite.register_test("Inline LDA Test", [&]() { inline_lda_test(cpu, mem); });
    test_suite.register_test("Inline LDA ZP Test", [&]() { inline_lda_zp_test(cpu, mem); });
    test_suite.register_test("Inline LDA ZPX Test", [&]() { inline_lda_zpx_test(cpu, mem); });
    test_suite.register_test("Inline LDA ZPX (Wrapping) Test", [&]() { inline_lda_zpx_wrap_test(cpu, mem); });
    test_suite.register_test("Inline JSR/RTS Test", [&]() { inline_jsr_rts_test(cpu, mem); });

    // Print test results
    test_suite.print_results();

    // Return true if all tests passed
    return test_suite.get_failed_count() == 0;
}
}  // namespace testing
