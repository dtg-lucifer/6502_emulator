#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <memory>
#include <string>
#include <vector>

#include "assembler_types.h"
#include "codegen.h"
#include "ir.h"
#include "lexer.h"
#include "parser.h"

namespace assembler {

class Assembler {
   private:
    std::string source_file_;
    std::string output_file_;
    bool verbose_;
    bool debug_;

    // Assembly stages
    std::unique_ptr<Lexer> lexer_;
    std::unique_ptr<Parser> parser_;
    std::unique_ptr<IR> ir_;
    std::unique_ptr<CodeGenerator> codegen_;

    // Results and diagnostics
    std::vector<std::string> errors_;
    std::vector<std::string> warnings_;
    AssemblyResult result_;

    // Helper methods
    bool read_source_file(const std::string& filename, std::string& content);
    bool write_binary_file(const std::string& filename, const std::vector<byte>& data);
    void print_errors() const;
    void print_warnings() const;
    void print_statistics() const;
    void add_error(const std::string& message);
    void add_warning(const std::string& message);

   public:
    // Constructors
    Assembler();
    explicit Assembler(const std::string& source_file);
    Assembler(const std::string& source_file, const std::string& output_file);

    // Destructor
    ~Assembler();

    // Configuration
    void set_source_file(const std::string& filename);
    void set_output_file(const std::string& filename);
    void set_verbose(bool verbose);
    void set_debug(bool debug);

    // Main assembly methods
    bool assemble();
    bool assemble_from_string(const std::string& source);
    bool assemble_file(const std::string& source_file, const std::string& output_file = "");

    // Results access
    const AssemblyResult& get_result() const;
    const std::vector<std::string>& get_errors() const;
    const std::vector<std::string>& get_warnings() const;
    bool has_errors() const;
    bool has_warnings() const;

    // Binary output
    bool write_output();
    bool write_output(const std::string& filename);
    std::vector<byte> get_binary() const;
    std::vector<byte> get_binary(word start_addr, word end_addr) const;

    // Memory map access
    const std::map<word, byte>& get_memory_map() const;
    word get_start_address() const;
    word get_end_address() const;
    size_t get_total_size() const;

    // Debug and analysis
    void print_tokens() const;
    void print_ir() const;
    void print_symbol_table() const;
    void print_memory_map() const;
    void print_disassembly() const;

    // Static utility methods
    static std::string get_default_output_filename(const std::string& source_file);
    static bool is_valid_source_file(const std::string& filename);
    static std::string get_file_extension(const std::string& filename);

   private:
    // Assembly pipeline stages
    bool tokenize_stage();
    bool parse_stage();
    bool ir_stage();
    bool codegen_stage();

    // Cleanup
    void cleanup();
    void reset();
};

// Convenience functions for single-step assembly
AssemblyResult assemble_file(const std::string& source_file, const std::string& output_file = "", bool verbose = false);

AssemblyResult assemble_string(const std::string& source, bool verbose = false);

// Utility functions
std::string get_assembler_version();
void print_assembler_info();

}  // namespace assembler

#endif  // ASSEMBLER_H
