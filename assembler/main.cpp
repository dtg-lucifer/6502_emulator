#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

#include "assembler.h"

namespace assembler {

Assembler::Assembler() : verbose_(false), debug_(false) {
    reset();
}

Assembler::Assembler(const std::string& source_file) : source_file_(source_file), verbose_(false), debug_(false) {
    output_file_ = get_default_output_filename(source_file);
    reset();
}

Assembler::Assembler(const std::string& source_file, const std::string& output_file)
    : source_file_(source_file), output_file_(output_file), verbose_(false), debug_(false) {
    reset();
}

Assembler::~Assembler() {
    cleanup();
}

void Assembler::set_source_file(const std::string& filename) {
    source_file_ = filename;
    if (output_file_.empty()) {
        output_file_ = get_default_output_filename(filename);
    }
}

void Assembler::set_output_file(const std::string& filename) {
    output_file_ = filename;
}

void Assembler::set_verbose(bool verbose) {
    verbose_ = verbose;
}

void Assembler::set_debug(bool debug) {
    debug_ = debug;
}

bool Assembler::assemble() {
    if (source_file_.empty()) {
        add_error("No source file specified");
        return false;
    }

    return assemble_file(source_file_, output_file_);
}

bool Assembler::assemble_from_string(const std::string& source) {
    reset();

    if (verbose_) {
        std::cout << "Assembling from string..." << std::endl;
    }

    // Tokenization stage
    lexer_ = std::make_unique<Lexer>(source);
    if (!tokenize_stage()) {
        return false;
    }

    // Parsing stage
    if (!parse_stage()) {
        return false;
    }

    // IR processing stage
    if (!ir_stage()) {
        return false;
    }

    // Code generation stage
    if (!codegen_stage()) {
        return false;
    }

    if (verbose_) {
        std::cout << "Assembly completed successfully." << std::endl;
        print_statistics();
    }

    return true;
}

bool Assembler::assemble_file(const std::string& source_file, const std::string& output_file) {
    reset();

    if (verbose_) {
        std::cout << "Assembling file: " << source_file << std::endl;
    }

    // Read source file
    std::string source_content;
    if (!read_source_file(source_file, source_content)) {
        add_error("Failed to read source file: " + source_file);
        return false;
    }

    // Set output file if not provided
    std::string output = output_file.empty() ? get_default_output_filename(source_file) : output_file;
    output_file_ = output;

    // Assemble from string
    bool success = assemble_from_string(source_content);

    // Write output if assembly was successful
    if (success && !output.empty()) {
        if (!write_output(output)) {
            add_error("Failed to write output file: " + output);
            success = false;
        }
    }

    if (verbose_) {
        if (success) {
            std::cout << "Output written to: " << output << std::endl;
        } else {
            print_errors();
        }
    }

    return success;
}

const AssemblyResult& Assembler::get_result() const {
    return result_;
}

const std::vector<std::string>& Assembler::get_errors() const {
    return errors_;
}

const std::vector<std::string>& Assembler::get_warnings() const {
    return warnings_;
}

bool Assembler::has_errors() const {
    return !errors_.empty();
}

bool Assembler::has_warnings() const {
    return !warnings_.empty();
}

bool Assembler::write_output() {
    if (output_file_.empty()) {
        add_error("No output file specified");
        return false;
    }
    return write_output(output_file_);
}

bool Assembler::write_output(const std::string& filename) {
    if (!result_.success || result_.binary_data.empty()) {
        add_error("No binary data to write");
        return false;
    }

    return write_binary_file(filename, result_.binary_data);
}

std::vector<byte> Assembler::get_binary() const {
    return result_.binary_data;
}

std::vector<byte> Assembler::get_binary(word start_addr, word end_addr) const {
    if (!codegen_) {
        return {};
    }
    return codegen_->get_binary_data(start_addr, end_addr);
}

const std::map<word, byte>& Assembler::get_memory_map() const {
    return result_.memory_map;
}

word Assembler::get_start_address() const {
    if (!codegen_) {
        return 0;
    }
    return codegen_->get_start_address();
}

word Assembler::get_end_address() const {
    if (!codegen_) {
        return 0;
    }
    return codegen_->get_end_address();
}

size_t Assembler::get_total_size() const {
    if (!codegen_) {
        return 0;
    }
    return codegen_->get_total_size();
}

void Assembler::print_tokens() const {
    if (!lexer_) {
        std::cout << "No tokens available (lexer not run)" << std::endl;
        return;
    }

    std::cout << "Tokens:" << std::endl;
    std::cout << "-------" << std::endl;

    // This would require storing tokens in the lexer
    // For now, just indicate that tokens were processed
    std::cout << "Tokens were processed during lexical analysis." << std::endl;
}

void Assembler::print_ir() const {
    if (!ir_) {
        std::cout << "No IR available (parser not run)" << std::endl;
        return;
    }

    ir_->print_nodes();
}

void Assembler::print_symbol_table() const {
    if (!ir_) {
        std::cout << "No symbol table available (parser not run)" << std::endl;
        return;
    }

    ir_->print_symbol_table();
}

void Assembler::print_memory_map() const {
    if (!codegen_) {
        std::cout << "No memory map available (code generator not run)" << std::endl;
        return;
    }

    codegen_->print_memory_map();
}

void Assembler::print_disassembly() const {
    if (!codegen_) {
        std::cout << "No disassembly available (code generator not run)" << std::endl;
        return;
    }

    codegen_->print_disassembly();
}

std::string Assembler::get_default_output_filename(const std::string& source_file) {
    size_t dot_pos = source_file.find_last_of('.');
    if (dot_pos != std::string::npos) {
        return source_file.substr(0, dot_pos) + ".bin";
    }
    return source_file + ".bin";
}

bool Assembler::is_valid_source_file(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

std::string Assembler::get_file_extension(const std::string& filename) {
    size_t dot_pos = filename.find_last_of('.');
    if (dot_pos != std::string::npos) {
        return filename.substr(dot_pos);
    }
    return "";
}

bool Assembler::tokenize_stage() {
    try {
        auto tokens = lexer_->tokenize();
        parser_ = std::make_unique<Parser>(tokens);

        if (debug_) {
            std::cout << "Tokenization completed. Generated " << tokens.size() << " tokens." << std::endl;
        }

        return true;
    } catch (const std::exception& e) {
        add_error("Tokenization failed: " + std::string(e.what()));
        return false;
    }
}

bool Assembler::parse_stage() {
    try {
        auto ir_nodes = parser_->parse();
        ir_ = std::make_unique<IR>(ir_nodes);

        if (debug_) {
            std::cout << "Parsing completed. Generated " << ir_nodes.size() << " IR nodes." << std::endl;
        }

        return true;
    } catch (const std::exception& e) {
        add_error("Parsing failed: " + std::string(e.what()));
        return false;
    }
}

bool Assembler::ir_stage() {
    try {
        ir_->process();

        auto ir_errors = ir_->get_errors();
        for (const auto& error : ir_errors) {
            add_error(error);
        }

        if (!ir_errors.empty()) {
            return false;
        }

        if (debug_) {
            std::cout << "IR processing completed." << std::endl;
            std::cout << "Symbol table contains " << ir_->get_symbol_table().size() << " symbols." << std::endl;
        }

        return true;
    } catch (const std::exception& e) {
        add_error("IR processing failed: " + std::string(e.what()));
        return false;
    }
}

bool Assembler::codegen_stage() {
    try {
        codegen_ = std::make_unique<CodeGenerator>(*ir_);
        result_ = codegen_->generate();

        // Add any code generation errors
        auto codegen_errors = codegen_->get_errors();
        for (const auto& error : codegen_errors) {
            add_error(error);
        }

        if (!codegen_errors.empty()) {
            result_.success = false;
            return false;
        }

        if (debug_) {
            std::cout << "Code generation completed." << std::endl;
            std::cout << "Generated " << result_.binary_data.size() << " bytes of binary data." << std::endl;
            std::cout << "Address range: $" << std::hex << get_start_address() << " - $" << get_end_address()
                      << std::dec << std::endl;
        }

        return true;
    } catch (const std::exception& e) {
        add_error("Code generation failed: " + std::string(e.what()));
        return false;
    }
}

bool Assembler::read_source_file(const std::string& filename, std::string& content) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    content = buffer.str();

    return true;
}

bool Assembler::write_binary_file(const std::string& filename, const std::vector<byte>& data) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    file.write(reinterpret_cast<const char*>(data.data()), data.size());
    return file.good();
}

void Assembler::print_errors() const {
    if (!errors_.empty()) {
        std::cout << "Errors:" << std::endl;
        for (const auto& error : errors_) {
            std::cout << "  " << error << std::endl;
        }
    }
}

void Assembler::print_warnings() const {
    if (!warnings_.empty()) {
        std::cout << "Warnings:" << std::endl;
        for (const auto& warning : warnings_) {
            std::cout << "  " << warning << std::endl;
        }
    }
}

void Assembler::print_statistics() const {
    std::cout << "Assembly Statistics:" << std::endl;
    std::cout << "  Binary size: " << result_.binary_data.size() << " bytes" << std::endl;
    std::cout << "  Start address: $" << std::hex << get_start_address() << std::dec << std::endl;
    std::cout << "  End address: $" << std::hex << get_end_address() << std::dec << std::endl;
    std::cout << "  Total size: " << get_total_size() << " bytes" << std::endl;
    std::cout << "  Symbols: " << (ir_ ? ir_->get_symbol_table().size() : 0) << std::endl;
    std::cout << "  Errors: " << errors_.size() << std::endl;
    std::cout << "  Warnings: " << warnings_.size() << std::endl;
}

void Assembler::add_error(const std::string& message) {
    errors_.push_back(message);
}

void Assembler::add_warning(const std::string& message) {
    warnings_.push_back(message);
}

void Assembler::cleanup() {
    lexer_.reset();
    parser_.reset();
    ir_.reset();
    codegen_.reset();
}

void Assembler::reset() {
    cleanup();
    errors_.clear();
    warnings_.clear();
    result_ = AssemblyResult();
}

// Convenience functions
AssemblyResult assemble_file(const std::string& source_file, const std::string& output_file, bool verbose) {
    Assembler assembler(source_file, output_file);
    assembler.set_verbose(verbose);

    if (assembler.assemble()) {
        return assembler.get_result();
    } else {
        AssemblyResult result;
        result.errors = assembler.get_errors();
        result.success = false;
        return result;
    }
}

AssemblyResult assemble_string(const std::string& source, bool verbose) {
    Assembler assembler;
    assembler.set_verbose(verbose);

    if (assembler.assemble_from_string(source)) {
        return assembler.get_result();
    } else {
        AssemblyResult result;
        result.errors = assembler.get_errors();
        result.success = false;
        return result;
    }
}

std::string get_assembler_version() {
    return "6502 Assembler v1.0.0";
}

void print_assembler_info() {
    std::cout << get_assembler_version() << std::endl;
    std::cout << "A 6502 assembly language assembler" << std::endl;
    std::cout << "Supports standard 6502 instruction set and addressing modes" << std::endl;
}

}  // namespace assembler
