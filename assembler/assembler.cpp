#include <iostream>
#include <string>
#include <vector>

#include "assembler.h"

void print_usage(const char* program_name) {
    std::cout << "Usage: " << program_name << " [options] <source_file>\n";
    std::cout << "\n";
    std::cout << "Options:\n";
    std::cout << "  -o <file>    Output file (default: <source>.bin)\n";
    std::cout << "  -v           Verbose output\n";
    std::cout << "  -d           Debug output\n";
    std::cout << "  -h, --help   Show this help message\n";
    std::cout << "  --version    Show version information\n";
    std::cout << "  --symbols    Print symbol table\n";
    std::cout << "  --memory     Print memory map\n";
    std::cout << "  --ir         Print intermediate representation\n";
    std::cout << "\n";
    std::cout << "Examples:\n";
    std::cout << "  " << program_name << " program.s\n";
    std::cout << "  " << program_name << " -v -o program.bin program.s\n";
    std::cout << "  " << program_name << " --symbols program.s\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    std::string source_file;
    std::string output_file;
    bool verbose = false;
    bool debug = false;
    bool show_symbols = false;
    bool show_memory = false;
    bool show_ir = false;

    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            print_usage(argv[0]);
            return 0;
        } else if (arg == "--version") {
            assembler::print_assembler_info();
            return 0;
        } else if (arg == "-v") {
            verbose = true;
        } else if (arg == "-d") {
            debug = true;
        } else if (arg == "--symbols") {
            show_symbols = true;
        } else if (arg == "--memory") {
            show_memory = true;
        } else if (arg == "--ir") {
            show_ir = true;
        } else if (arg == "-o") {
            if (i + 1 >= argc) {
                std::cerr << "Error: -o option requires a filename\n";
                return 1;
            }
            output_file = argv[++i];
        } else if (arg[0] == '-') {
            std::cerr << "Error: Unknown option: " << arg << "\n";
            print_usage(argv[0]);
            return 1;
        } else {
            if (source_file.empty()) {
                source_file = arg;
            } else {
                std::cerr << "Error: Multiple source files not supported\n";
                return 1;
            }
        }
    }

    if (source_file.empty()) {
        std::cerr << "Error: No source file specified\n";
        print_usage(argv[0]);
        return 1;
    }

    // Check if source file exists
    if (!assembler::Assembler::is_valid_source_file(source_file)) {
        std::cerr << "Error: Cannot read source file: " << source_file << "\n";
        return 1;
    }

    // Create assembler instance
    assembler::Assembler assembler(source_file, output_file);
    assembler.set_verbose(verbose);
    assembler.set_debug(debug);

    if (verbose) {
        assembler::print_assembler_info();
        std::cout << "\n";
    }

    // Perform assembly
    bool success = assembler.assemble();

    // Print debug information if requested
    if (show_ir) {
        std::cout << "\n";
        assembler.print_ir();
    }

    if (show_symbols) {
        std::cout << "\n";
        assembler.print_symbol_table();
    }

    if (show_memory) {
        std::cout << "\n";
        assembler.print_memory_map();
    }

    // Print results
    if (success) {
        if (verbose) {
            std::cout << "Assembly completed successfully!\n";
            std::cout << "Output file: "
                      << (output_file.empty() ? assembler::Assembler::get_default_output_filename(source_file)
                                              : output_file)
                      << "\n";
            std::cout << "Binary size: " << assembler.get_binary().size() << " bytes\n";
            std::cout << "Address range: $" << std::hex << assembler.get_start_address() << " - $"
                      << assembler.get_end_address() << std::dec << "\n";
        }
        return 0;
    } else {
        std::cerr << "Assembly failed!\n";

        // Print errors
        const auto& errors = assembler.get_errors();
        if (!errors.empty()) {
            std::cerr << "\nErrors:\n";
            for (const auto& error : errors) {
                std::cerr << "  " << error << "\n";
            }
        }

        // Print warnings
        const auto& warnings = assembler.get_warnings();
        if (!warnings.empty()) {
            std::cerr << "\nWarnings:\n";
            for (const auto& warning : warnings) {
                std::cerr << "  " << warning << "\n";
            }
        }

        return 1;
    }
}
