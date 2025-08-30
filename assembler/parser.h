#ifndef PARSER_H
#define PARSER_H

#include <map>
#include <string>
#include <vector>

#include "assembler_types.h"
#include "lexer.h"

namespace assembler {

class Parser {
   private:
    std::vector<Token> tokens_;
    size_t current_token_;
    std::map<std::string, word> labels_;
    word current_address_;

    // Helper methods
    Token& current();
    const Token& current() const;
    Token& peek(int offset = 1);
    const Token& peek(int offset = 1) const;
    bool match(TokenType type);
    void advance();
    void expect(TokenType type);
    bool is_at_end();

    // Parsing methods
    IRNode parse_statement();
    Instruction parse_instruction();
    Directive parse_directive();
    Label parse_label();

    // Operand parsing
    Operand parse_operand(const std::string& mnemonic);
    AddressingMode determine_addressing_mode(const std::string& mnemonic, const Token& operand_token);
    word parse_number(const std::string& value);
    bool is_mnemonic(const std::string& identifier);

    // Address mode specific parsing
    Operand parse_immediate();
    Operand parse_zero_page_or_absolute();
    Operand parse_indexed(AddressingMode base_mode);
    Operand parse_indirect();

    // Validation
    bool validate_addressing_mode(const std::string& mnemonic, AddressingMode mode);
    size_t get_instruction_size(const std::string& mnemonic, AddressingMode mode);

   public:
    explicit Parser(const std::vector<Token>& tokens);

    // Main parsing method
    std::vector<IRNode> parse();

    // Error handling
    std::string get_error_message(const std::string& message) const;
    std::string get_current_location() const;
};

}  // namespace assembler

#endif  // PARSER_H
