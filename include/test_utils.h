#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "types.h"

namespace testing {

// Custom exception for test failures
class TestFailedException : public std::exception {
   private:
    std::string message;

   public:
    TestFailedException(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override { return message.c_str(); }
};

// Test case result tracking
struct TestResult {
    std::string name;
    bool passed;
    std::string message;

    TestResult(const std::string& test_name, bool test_passed, const std::string& test_message = "") : name(test_name), passed(test_passed), message(test_message) {}
};

class TestSuite {
   private:
    std::vector<TestResult> results;
    std::string suite_name;
    int passed_count = 0;
    int failed_count = 0;

   public:
    TestSuite(const std::string& name) : suite_name(name) {}

    void add_result(const TestResult& result) {
        results.push_back(result);
        if (result.passed) {
            passed_count++;
        } else {
            failed_count++;
        }
    }

    int get_passed_count() const { return passed_count; }
    int get_failed_count() const { return failed_count; }

    // Register a test function to run
    template <typename Func>
    void register_test(const std::string& name, Func test_func) {
        try {
            test_func();
            add_result(TestResult(name, true));
        } catch (const TestFailedException& e) {
            add_result(TestResult(name, false, e.what()));
        } catch (const std::exception& e) {
            std::string msg = "Unexpected exception: ";
            msg += e.what();
            add_result(TestResult(name, false, msg));
        } catch (...) {
            add_result(TestResult(name, false, "Unknown exception occurred"));
        }
    }

    // Print test suite results
    void print_results() const {
        std::cout << "\n======================================" << std::endl;
        std::cout << "TEST SUITE: " << suite_name << std::endl;
        std::cout << "======================================" << std::endl;

        for (const auto& result : results) {
            if (result.passed) {
                std::cout << "✅ PASS: " << result.name << std::endl;
            } else {
                std::cout << "❌ FAIL: " << result.name << std::endl;
                std::cout << "   " << result.message << std::endl;
            }
        }

        std::cout << "--------------------------------------" << std::endl;
        std::cout << "SUMMARY: " << passed_count << " passed, " << failed_count << " failed" << std::endl;
        std::cout << "======================================\n" << std::endl;
    }
};
}  // namespace testing

#endif  // TEST_UTILS_H
