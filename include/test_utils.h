#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <exception>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "cpu.h"
#include "types.h"

namespace testing {

// Custom exception for test failures
class TestFailedException : public std::exception {
   private:
    std::string message;
    std::string colored_message;

   public:
    TestFailedException(const std::string& msg) : message(msg) {
        colored_message =
            std::string(colors::RED) + std::string(colors::BOLD) + ">> " + msg + std::string(colors::RESET);
    }

    const char* what() const noexcept override { return colored_message.c_str(); }

    // For when we need the raw message without colors
    const std::string& raw_message() const { return message; }
};

// Test case result tracking
struct TestResult {
    std::string name;
    bool passed;
    std::string message;

    TestResult(const std::string& test_name, bool test_passed, const std::string& test_message = "")
        : name(test_name), passed(test_passed), message(test_message) {}
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

    void print_header() const {
        std::cout << "\n"
                  << colors::YELLOW << colors::BOLD << "===== Running " << this->suite_name
                  << " Tests =====" << colors::RESET << "\n"
                  << std::endl;
    }

    // Print test suite results
    void print_results() const {
        std::cout << "\n"
                  << colors::CYAN << colors::BOLD << "======================================" << colors::RESET
                  << std::endl;
        std::cout << colors::CYAN << colors::BOLD << "TEST SUITE: " << suite_name << colors::RESET << std::endl;
        std::cout << colors::CYAN << colors::BOLD << "======================================" << colors::RESET
                  << std::endl;

        for (const auto& result : results) {
            if (result.passed) {
                std::cout << colors::GREEN << colors::BOLD << "✅ PASS: " << result.name << colors::RESET << std::endl;
            } else {
                std::cout << colors::RED << colors::BOLD << "❌ FAIL: " << result.name << colors::RESET << std::endl;
                std::cout << colors::RED << "   " << result.message << colors::RESET << std::endl;
            }
        }

        std::cout << colors::CYAN << "--------------------------------------" << colors::RESET << std::endl;

        // Summary line with colors based on results
        std::cout << "SUMMARY: " << colors::GREEN << colors::BOLD << passed_count << " passed" << colors::RESET;

        if (failed_count > 0) {
            std::cout << ", " << colors::RED << colors::BOLD << failed_count << " failed" << colors::RESET;
        } else {
            std::cout << ", " << failed_count << " failed";
        }

        std::cout << std::endl;
        std::cout << colors::CYAN << colors::BOLD << "======================================\n"
                  << colors::RESET << std::endl;
    }
};

// Utility function to print colorful CPU register state
inline void print_cpu_state(Cpu& cpu) {
    std::cout << colors::CYAN << colors::BOLD << "CPU STATE:" << colors::RESET << std::endl;

    // Print registers
    std::cout << colors::YELLOW << "A:  " << colors::BOLD << "0x" << std::hex << std::setfill('0') << std::setw(2)
              << static_cast<int>(cpu.get(Register::A)) << colors::RESET << std::endl;

    std::cout << colors::YELLOW << "X:  " << colors::BOLD << "0x" << std::hex << std::setfill('0') << std::setw(2)
              << static_cast<int>(cpu.get(Register::X)) << colors::RESET << std::endl;

    std::cout << colors::YELLOW << "Y:  " << colors::BOLD << "0x" << std::hex << std::setfill('0') << std::setw(2)
              << static_cast<int>(cpu.get(Register::Y)) << colors::RESET << std::endl;

    std::cout << colors::YELLOW << "SP: " << colors::BOLD << "0x" << std::hex << std::setfill('0') << std::setw(2)
              << static_cast<int>(cpu.SP) << colors::RESET << std::endl;

    std::cout << colors::YELLOW << "PC: " << colors::BOLD << "0x" << std::hex << std::setfill('0') << std::setw(4)
              << cpu.PC << colors::RESET << std::endl;

    // Print flags
    std::cout << colors::MAGENTA << "Flags:" << colors::RESET << std::endl;
    std::cout << "N: " << (cpu.N ? colors::GREEN : colors::RED) << static_cast<int>(cpu.N) << colors::RESET
              << std::endl;
    std::cout << "V: " << (cpu.V ? colors::GREEN : colors::RED) << static_cast<int>(cpu.V) << colors::RESET
              << std::endl;
    std::cout << "B: " << (cpu.B ? colors::GREEN : colors::RED) << static_cast<int>(cpu.B) << colors::RESET
              << std::endl;
    std::cout << "D: " << (cpu.D ? colors::GREEN : colors::RED) << static_cast<int>(cpu.D) << colors::RESET
              << std::endl;
    std::cout << "I: " << (cpu.I ? colors::GREEN : colors::RED) << static_cast<int>(cpu.I) << colors::RESET
              << std::endl;
    std::cout << "Z: " << (cpu.Z ? colors::GREEN : colors::RED) << static_cast<int>(cpu.Z) << colors::RESET
              << std::endl;
    std::cout << "C: " << (cpu.C ? colors::GREEN : colors::RED) << static_cast<int>(cpu.C) << colors::RESET
              << std::endl;
}
}  // namespace testing

#endif  // TEST_UTILS_H
