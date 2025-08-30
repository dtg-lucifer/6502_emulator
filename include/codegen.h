#ifndef CODEGEN_H
#define CODEGEN_H

#include <map>
#include <string>
#include <vector>

#include "assembler_types.h"
#include "ir.h"
#include "op_codes.h"

namespace assembler {

class CodeGenerator {
   private:
    const IR& ir_;
    std::map<word, byte> output_memory_;
    std::vector<std::string> errors_;
    word start_address_;
    word end_address_;

    // Instruction encoding tables
    static const std::map<std::string, std::map<AddressingMode, byte>> instruction_opcodes_;

    // Helper methods
    bool encode_instruction(const Instruction& instruction, word address);
    bool encode_directive(const Directive& directive, word address);
    byte get_opcode(const std::string& mnemonic, AddressingMode mode) const;
    std::vector<byte> encode_operand(const Operand& operand, AddressingMode mode, word current_address) const;
    size_t get_instruction_size(AddressingMode mode) const;
    bool is_valid_instruction(const std::string& mnemonic, AddressingMode mode) const;

    // Address resolution
    word resolve_operand_address(const Operand& operand, word current_address) const;
    byte resolve_relative_address(word target, word current_address) const;

    // Validation
    bool validate_zero_page_address(word address) const;
    bool validate_relative_branch(word target, word current_address) const;
    bool validate_address_range(word address) const;

    // Output generation
    void write_byte(word address, byte value);
    void write_word(word address, word value);

   public:
    explicit CodeGenerator(const IR& ir);

    // Main code generation method
    AssemblyResult generate();

    // Binary output methods
    std::vector<byte> get_binary_data() const;
    std::vector<byte> get_binary_data(word start_addr, word end_addr) const;
    bool write_binary_file(const std::string& filename) const;

    // Memory map access
    const std::map<word, byte>& get_memory_map() const;
    bool has_data_at(word address) const;
    byte get_byte_at(word address) const;

    // Address range information
    word get_start_address() const;
    word get_end_address() const;
    size_t get_total_size() const;

    // Error handling
    const std::vector<std::string>& get_errors() const;
    bool has_errors() const;

    // Debug output
    void print_memory_map() const;
    void print_disassembly() const;

   private:
    void add_error(const std::string& message);
    std::string format_address(word address) const;
    std::string format_byte(byte value) const;
    std::string format_word(word value) const;
};

}  // namespace assembler

#endif  // CODEGEN_H
