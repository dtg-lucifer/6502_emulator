#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <unordered_map>
#include <vector>

#include "assembler_types.h"

namespace assembler {

class Lexer {
   private:
    std::string source_;
    size_t position_;
    size_t line_;
    size_t column_;
    char current_char_;

    // Keywords and mnemonics lookup table
    static const std::unordered_map<std::string, TokenType> keywords_;

    // Helper methods
    void advance();
    void skip_whitespace();
    void skip_comment();
    char peek();
    std::string read_identifier();
    std::string read_number();
    std::string read_string();
    bool is_alpha(char c);
    bool is_digit(char c);
    bool is_hex_digit(char c);
    bool is_alnum(char c);

   public:
    explicit Lexer(const std::string& source);

    // Main tokenization method
    std::vector<Token> tokenize();

    // Get next token
    Token next_token();

    // Error handling
    std::string get_error_context(size_t line, size_t column) const;
};

}  // namespace assembler

#endif  // LEXER_H
