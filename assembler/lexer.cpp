#include "lexer.h"

#include <cctype>
#include <iostream>
#include <sstream>

namespace assembler {

// Initialize keywords and mnemonics lookup table
const std::unordered_map<std::string, TokenType> Lexer::keywords_ = {
    // Directives
    {".org", TokenType::ORG},  {"org", TokenType::ORG},    {".word", TokenType::WORD},
    {"word", TokenType::WORD}, {".byte", TokenType::BYTE}, {"byte", TokenType::BYTE},

    // All 6502 mnemonics are treated as identifiers initially
    // and validated later in the parser
};

Lexer::Lexer(const std::string& source) : source_(source), position_(0), line_(1), column_(1) {
    current_char_ = position_ < source_.length() ? source_[position_] : '\0';
}

void Lexer::advance() {
    if (current_char_ == '\n') {
        line_++;
        column_ = 1;
    } else {
        column_++;
    }

    position_++;
    current_char_ = position_ < source_.length() ? source_[position_] : '\0';
}

void Lexer::skip_whitespace() {
    while (current_char_ != '\0' && std::isspace(current_char_) && current_char_ != '\n') {
        advance();
    }
}

void Lexer::skip_comment() {
    if (current_char_ == ';') {
        // Skip until end of line
        while (current_char_ != '\0' && current_char_ != '\n') {
            advance();
        }
    }
}

char Lexer::peek() {
    size_t peek_pos = position_ + 1;
    return peek_pos < source_.length() ? source_[peek_pos] : '\0';
}

std::string Lexer::read_identifier() {
    std::string result;

    // Handle leading dot for directives
    if (current_char_ == '.') {
        result += current_char_;
        advance();
    }

    while (current_char_ != '\0' && (is_alnum(current_char_) || current_char_ == '_')) {
        result += current_char_;
        advance();
    }

    return result;
}

std::string Lexer::read_number() {
    std::string result;

    // Handle different number formats
    if (current_char_ == '$') {
        // Hexadecimal number
        result += current_char_;
        advance();

        while (current_char_ != '\0' && is_hex_digit(current_char_)) {
            result += current_char_;
            advance();
        }
    } else if (current_char_ == '%') {
        // Binary number
        result += current_char_;
        advance();

        while (current_char_ != '\0' && (current_char_ == '0' || current_char_ == '1')) {
            result += current_char_;
            advance();
        }
    } else {
        // Decimal number
        while (current_char_ != '\0' && is_digit(current_char_)) {
            result += current_char_;
            advance();
        }
    }

    return result;
}

std::string Lexer::read_string() {
    std::string result;
    advance();  // Skip opening quote

    while (current_char_ != '\0' && current_char_ != '"') {
        if (current_char_ == '\\') {
            advance();
            switch (current_char_) {
                case 'n':
                    result += '\n';
                    break;
                case 't':
                    result += '\t';
                    break;
                case 'r':
                    result += '\r';
                    break;
                case '\\':
                    result += '\\';
                    break;
                case '"':
                    result += '"';
                    break;
                default:
                    result += current_char_;
                    break;
            }
        } else {
            result += current_char_;
        }
        advance();
    }

    if (current_char_ == '"') {
        advance();  // Skip closing quote
    }

    return result;
}

bool Lexer::is_alpha(char c) {
    return std::isalpha(c) || c == '_';
}

bool Lexer::is_digit(char c) {
    return std::isdigit(c);
}

bool Lexer::is_hex_digit(char c) {
    return std::isdigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

bool Lexer::is_alnum(char c) {
    return std::isalnum(c) || c == '_';
}

Token Lexer::next_token() {
    while (current_char_ != '\0') {
        size_t token_line = line_;
        size_t token_column = column_;

        // Skip whitespace (except newlines)
        if (std::isspace(current_char_) && current_char_ != '\n') {
            skip_whitespace();
            continue;
        }

        // Handle newlines
        if (current_char_ == '\n') {
            advance();
            return Token(TokenType::NEWLINE, "\\n", token_line, token_column);
        }

        // Handle comments
        if (current_char_ == ';') {
            skip_comment();
            continue;
        }

        // Handle identifiers and keywords
        if (is_alpha(current_char_) || current_char_ == '.') {
            std::string identifier = read_identifier();

            // Check for special case: X and Y for indexing
            if (identifier == "X" || identifier == "Y") {
                return Token(identifier == "X" ? TokenType::X_INDEX : TokenType::Y_INDEX, identifier, token_line,
                             token_column);
            }

            // Check if it's a keyword/directive
            auto keyword_it = keywords_.find(identifier);
            if (keyword_it != keywords_.end()) {
                return Token(keyword_it->second, identifier, token_line, token_column);
            }

            return Token(TokenType::IDENTIFIER, identifier, token_line, token_column);
        }

        // Handle numbers
        if (is_digit(current_char_) || current_char_ == '$' || current_char_ == '%') {
            std::string number = read_number();
            return Token(TokenType::NUMBER, number, token_line, token_column);
        }

        // Handle strings
        if (current_char_ == '"') {
            std::string str = read_string();
            return Token(TokenType::STRING, str, token_line, token_column);
        }

        // Handle single character tokens
        switch (current_char_) {
            case '+':
                advance();
                return Token(TokenType::PLUS, "+", token_line, token_column);

            case '-':
                advance();
                return Token(TokenType::MINUS, "-", token_line, token_column);

            case '$':
                // This should be handled in read_number, but just in case
                advance();
                return Token(TokenType::DOLLAR, "$", token_line, token_column);

            case '#':
                advance();
                return Token(TokenType::HASH, "#", token_line, token_column);

            case '.':
                advance();
                return Token(TokenType::DOT, ".", token_line, token_column);

            case ',':
                advance();
                return Token(TokenType::COMMA, ",", token_line, token_column);

            case ':':
                advance();
                return Token(TokenType::COLON, ":", token_line, token_column);

            case '(':
                advance();
                return Token(TokenType::LPAREN, "(", token_line, token_column);

            case ')':
                advance();
                return Token(TokenType::RPAREN, ")", token_line, token_column);

            case '[':
                advance();
                return Token(TokenType::LBRACKET, "[", token_line, token_column);

            case ']':
                advance();
                return Token(TokenType::RBRACKET, "]", token_line, token_column);

            default:
                std::string unknown_char(1, current_char_);
                advance();
                return Token(TokenType::UNKNOWN, unknown_char, token_line, token_column);
        }
    }

    return Token(TokenType::EOF_TOKEN, "", line_, column_);
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    Token token = next_token();
    while (token.type != TokenType::EOF_TOKEN) {
        tokens.push_back(token);
        token = next_token();
    }

    // Add EOF token
    tokens.push_back(token);

    return tokens;
}

std::string Lexer::get_error_context(size_t line, size_t column) const {
    std::stringstream ss;
    std::istringstream source_stream(source_);
    std::string current_line;
    size_t current_line_num = 1;

    // Find the line
    while (std::getline(source_stream, current_line) && current_line_num < line) {
        current_line_num++;
    }

    if (current_line_num == line) {
        ss << "Line " << line << ": " << current_line << "\n";
        ss << std::string(9 + column - 1, ' ') << "^";
    } else {
        ss << "Line " << line << ": (line not found)";
    }

    return ss.str();
}

}  // namespace assembler
