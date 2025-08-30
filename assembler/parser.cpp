#include "parser.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <unordered_set>

namespace assembler {

// Set of valid 6502 mnemonics
static const std::unordered_set<std::string> valid_mnemonics = {
    // Load/Store Operations
    "LDA", "LDX", "LDY", "STA", "STX", "STY",

    // Register Transfer Operations
    "TAX", "TAY", "TXA", "TYA", "TSX", "TXS",

    // Stack Operations
    "PHA", "PHP", "PLA", "PLP",

    // Logical Operations
    "AND", "EOR", "ORA", "BIT",

    // Arithmetic Operations
    "ADC", "SBC", "CMP", "CPX", "CPY",

    // Increment/Decrement Operations
    "INC", "INX", "INY", "DEC", "DEX", "DEY",

    // Shifts and Rotates
    "ASL", "LSR", "ROL", "ROR",

    // Jump/Branch Operations
    "JMP", "JSR", "RTS", "BRK", "BCC", "BCS", "BEQ", "BMI", "BNE", "BPL", "BVC", "BVS",

    // Status Flag Operations
    "CLC", "CLD", "CLI", "CLV", "SEC", "SED", "SEI",

    // No Operation
    "NOP"};

Parser::Parser(const std::vector<Token>& tokens) : tokens_(tokens), current_token_(0), current_address_(0) {}

Token& Parser::current() {
    if (current_token_ >= tokens_.size()) {
        static Token eof_token(TokenType::EOF_TOKEN, "", 0, 0);
        return eof_token;
    }
    return tokens_[current_token_];
}

const Token& Parser::current() const {
    if (current_token_ >= tokens_.size()) {
        static Token eof_token(TokenType::EOF_TOKEN, "", 0, 0);
        return eof_token;
    }
    return tokens_[current_token_];
}

Token& Parser::peek(int offset) {
    size_t peek_index = current_token_ + offset;
    if (peek_index >= tokens_.size()) {
        static Token eof_token(TokenType::EOF_TOKEN, "", 0, 0);
        return eof_token;
    }
    return tokens_[peek_index];
}

const Token& Parser::peek(int offset) const {
    size_t peek_index = current_token_ + offset;
    if (peek_index >= tokens_.size()) {
        static Token eof_token(TokenType::EOF_TOKEN, "", 0, 0);
        return eof_token;
    }
    return tokens_[peek_index];
}

bool Parser::match(TokenType type) {
    if (current().type == type) {
        advance();
        return true;
    }
    return false;
}

void Parser::advance() {
    if (current_token_ < tokens_.size()) {
        current_token_++;
    }
}

void Parser::expect(TokenType type) {
    if (current().type != type) {
        throw std::runtime_error("Expected token type but got " + current().value);
    }
    advance();
}

bool Parser::is_at_end() {
    return current().type == TokenType::EOF_TOKEN;
}

std::vector<IRNode> Parser::parse() {
    std::vector<IRNode> nodes;

    while (!is_at_end()) {
        // Skip newlines
        if (match(TokenType::NEWLINE)) {
            continue;
        }

        try {
            IRNode node = parse_statement();
            nodes.push_back(node);
        } catch (const std::exception& e) {
            std::cerr << "Parse error at line " << current().line << ": " << e.what() << std::endl;
            std::cerr << "Current token: '" << current().value << "' (type: " << static_cast<int>(current().type) << ")"
                      << std::endl;
            // Skip to next line to continue parsing
            while (!is_at_end() && !match(TokenType::NEWLINE)) {
                advance();
            }
        }
    }

    return nodes;
}

IRNode Parser::parse_statement() {
    // Check for label (identifier followed by colon)
    if (current().type == TokenType::IDENTIFIER && peek().type == TokenType::COLON) {
        return IRNode(parse_label(), current().line);
    }

    // Check for directive (starts with dot or is a directive token)
    if (current().type == TokenType::DOT || current().type == TokenType::ORG || current().type == TokenType::WORD ||
        current().type == TokenType::BYTE ||
        (current().type == TokenType::IDENTIFIER &&
         (current().value == ".org" || current().value == ".word" || current().value == ".byte"))) {
        return IRNode(parse_directive(), current().line);
    }

    // Must be an instruction
    if (current().type == TokenType::IDENTIFIER) {
        return IRNode(parse_instruction(), current().line);
    }

    throw std::runtime_error("Expected label, directive, or instruction");
}

Instruction Parser::parse_instruction() {
    if (current().type != TokenType::IDENTIFIER) {
        throw std::runtime_error("Expected instruction mnemonic");
    }

    std::string mnemonic = current().value;
    std::transform(mnemonic.begin(), mnemonic.end(), mnemonic.begin(), ::toupper);

    if (!is_mnemonic(mnemonic)) {
        throw std::runtime_error("Unknown instruction: " + mnemonic);
    }

    size_t line = current().line;
    advance();

    // Parse operand if present
    Operand operand;
    AddressingMode mode = AddressingMode::IMPLIED;

    if (!is_at_end() && current().type != TokenType::NEWLINE && current().type != TokenType::EOF_TOKEN) {
        operand = parse_operand(mnemonic);

        // Extract addressing mode from operand
        if (std::holds_alternative<ImmediateOperand>(operand)) {
            mode = AddressingMode::IMMEDIATE;
        } else if (std::holds_alternative<AddressOperand>(operand)) {
            mode = std::get<AddressOperand>(operand).mode;
        } else if (std::holds_alternative<LabelOperand>(operand)) {
            mode = std::get<LabelOperand>(operand).mode;
        }

        // Override mode for branch instructions
        if (mnemonic == "BCC" || mnemonic == "BCS" || mnemonic == "BEQ" || mnemonic == "BMI" || mnemonic == "BNE" ||
            mnemonic == "BPL" || mnemonic == "BVC" || mnemonic == "BVS") {
            mode = AddressingMode::RELATIVE;
            // Update operand mode if it's an address or label operand
            if (std::holds_alternative<AddressOperand>(operand)) {
                auto& addr_op = std::get<AddressOperand>(operand);
                addr_op.mode = AddressingMode::RELATIVE;
            } else if (std::holds_alternative<LabelOperand>(operand)) {
                auto& label_op = std::get<LabelOperand>(operand);
                label_op.mode = AddressingMode::RELATIVE;
            }
        }
    }

    if (!validate_addressing_mode(mnemonic, mode)) {
        throw std::runtime_error("Invalid addressing mode for instruction " + mnemonic);
    }

    return Instruction(mnemonic, mode, operand, line);
}

Directive Parser::parse_directive() {
    std::string directive_name;

    if (current().type == TokenType::IDENTIFIER &&
        (current().value == ".org" || current().value == ".word" || current().value == ".byte")) {
        // Handle .org, .word, .byte as complete identifiers
        std::string value = current().value;
        if (value == ".org") {
            directive_name = "org";
        } else if (value == ".word") {
            directive_name = "word";
        } else if (value == ".byte") {
            directive_name = "byte";
        }
        advance();
    } else if (current().type == TokenType::DOT) {
        advance();
        if (current().type != TokenType::IDENTIFIER) {
            throw std::runtime_error("Expected directive name after '.'");
        }
        directive_name = current().value;
        advance();
    } else if (current().type == TokenType::ORG) {
        directive_name = "org";
        advance();
    } else if (current().type == TokenType::WORD) {
        directive_name = "word";
        advance();
    } else if (current().type == TokenType::BYTE) {
        directive_name = "byte";
        advance();
    } else {
        throw std::runtime_error("Unknown directive: " + current().value);
    }

    std::transform(directive_name.begin(), directive_name.end(), directive_name.begin(), ::tolower);
    size_t line = current().line;

    // Parse directive value
    std::variant<word, std::string> value;

    if (directive_name == "org") {
        if (current().type != TokenType::NUMBER) {
            throw std::runtime_error(".org directive requires a number");
        }
        word address = parse_number(current().value);
        current_address_ = address;
        value = address;
        advance();
    } else if (directive_name == "word") {
        if (current().type == TokenType::NUMBER) {
            value = parse_number(current().value);
            advance();
        } else if (current().type == TokenType::IDENTIFIER) {
            value = current().value;
            advance();
        } else {
            throw std::runtime_error(".word directive requires a number or label");
        }
    } else if (directive_name == "byte") {
        if (current().type != TokenType::NUMBER) {
            throw std::runtime_error(".byte directive requires a number");
        }
        word byte_value = parse_number(current().value);
        if (byte_value > 255) {
            throw std::runtime_error(".byte value must be 0-255");
        }
        value = byte_value;
        advance();
    } else {
        throw std::runtime_error("Unknown directive: " + directive_name);
    }

    return Directive(directive_name, value, line);
}

Label Parser::parse_label() {
    if (current().type != TokenType::IDENTIFIER) {
        throw std::runtime_error("Expected label name");
    }

    std::string name = current().value;
    size_t line = current().line;
    advance();

    expect(TokenType::COLON);

    // Store label in symbol table
    labels_[name] = current_address_;

    return Label(name, current_address_, line);
}

Operand Parser::parse_operand(const std::string& mnemonic) {
    // Immediate addressing (#value)
    if (current().type == TokenType::HASH) {
        return parse_immediate();
    }

    // Indirect addressing (...)
    if (current().type == TokenType::LPAREN) {
        return parse_indirect();
    }

    // Check for accumulator addressing mode (A)
    if (current().type == TokenType::IDENTIFIER && current().value == "A") {
        advance();
        return AddressOperand{0, AddressingMode::ACCUMULATOR};
    }

    // Zero page or absolute addressing
    return parse_zero_page_or_absolute();
}

AddressingMode Parser::determine_addressing_mode(const std::string& mnemonic, const Token& operand_token) {
    // This method is called after operand parsing, so we don't need to determine mode here
    // The mode is already determined in the operand parsing methods
    return AddressingMode::IMPLIED;  // This will be overridden by the actual operand
}

word Parser::parse_number(const std::string& value) {
    if (value.empty()) {
        throw std::runtime_error("Empty number");
    }

    if (value[0] == '$') {
        // Hexadecimal
        return static_cast<word>(std::stol(value.substr(1), nullptr, 16));
    } else if (value[0] == '%') {
        // Binary
        return static_cast<word>(std::stol(value.substr(1), nullptr, 2));
    } else {
        // Decimal
        return static_cast<word>(std::stol(value));
    }
}

bool Parser::is_mnemonic(const std::string& identifier) {
    return valid_mnemonics.find(identifier) != valid_mnemonics.end();
}

Operand Parser::parse_immediate() {
    expect(TokenType::HASH);

    if (current().type == TokenType::NUMBER) {
        word value = parse_number(current().value);
        advance();
        return ImmediateOperand{value};
    } else if (current().type == TokenType::IDENTIFIER) {
        std::string label = current().value;
        advance();
        return LabelOperand{label, AddressingMode::IMMEDIATE};
    }

    throw std::runtime_error("Expected number or label after #");
}

Operand Parser::parse_zero_page_or_absolute() {
    if (current().type == TokenType::NUMBER) {
        word address = parse_number(current().value);
        advance();

        AddressingMode mode = (address <= 0xFF) ? AddressingMode::ZERO_PAGE : AddressingMode::ABSOLUTE;

        // Check for indexing
        if (current().type == TokenType::COMMA) {
            advance();
            if (current().value == "X") {
                advance();
                mode = (address <= 0xFF) ? AddressingMode::ZERO_PAGE_X : AddressingMode::ABSOLUTE_X;
            } else if (current().value == "Y") {
                advance();
                mode = (address <= 0xFF) ? AddressingMode::ZERO_PAGE_Y : AddressingMode::ABSOLUTE_Y;
            }
        }

        return AddressOperand{address, mode};

    } else if (current().type == TokenType::IDENTIFIER) {
        std::string label = current().value;
        advance();

        AddressingMode mode = AddressingMode::ABSOLUTE;  // Default for labels

        // Check for indexing
        if (current().type == TokenType::COMMA) {
            advance();
            if (current().value == "X") {
                advance();
                mode = AddressingMode::ABSOLUTE_X;
            } else if (current().value == "Y") {
                advance();
                mode = AddressingMode::ABSOLUTE_Y;
            }
        }

        return LabelOperand{label, mode};
    }

    throw std::runtime_error("Expected address or label");
}

Operand Parser::parse_indexed(AddressingMode base_mode) {
    // This is handled in parse_zero_page_or_absolute
    throw std::runtime_error("Indexed addressing parsed elsewhere");
}

Operand Parser::parse_indirect() {
    expect(TokenType::LPAREN);

    if (current().type == TokenType::NUMBER) {
        word address = parse_number(current().value);
        advance();

        // Check for indirect,X or indirect,Y
        if (current().type == TokenType::COMMA) {
            advance();
            if (current().value == "X") {
                advance();
                expect(TokenType::RPAREN);
                return AddressOperand{address, AddressingMode::INDIRECT_X};
            }
        }

        expect(TokenType::RPAREN);

        // Check for ),Y
        if (current().type == TokenType::COMMA) {
            advance();
            if (current().value == "Y") {
                advance();
                return AddressOperand{address, AddressingMode::INDIRECT_Y};
            }
        }

        return AddressOperand{address, AddressingMode::INDIRECT};

    } else if (current().type == TokenType::IDENTIFIER) {
        std::string label = current().value;
        advance();

        // Check for indirect,X or indirect,Y
        if (current().type == TokenType::COMMA) {
            advance();
            if (current().value == "X") {
                advance();
                expect(TokenType::RPAREN);
                return LabelOperand{label, AddressingMode::INDIRECT_X};
            }
        }

        expect(TokenType::RPAREN);

        // Check for ),Y
        if (current().type == TokenType::COMMA) {
            advance();
            if (current().value == "Y") {
                advance();
                return LabelOperand{label, AddressingMode::INDIRECT_Y};
            }
        }

        return LabelOperand{label, AddressingMode::INDIRECT};
    }

    throw std::runtime_error("Expected address or label in indirect addressing");
}

bool Parser::validate_addressing_mode(const std::string& mnemonic, AddressingMode mode) {
    // This is a simplified validation - in a real assembler, you'd have
    // comprehensive tables of which addressing modes are valid for each instruction

    // Some instructions only support specific addressing modes
    if (mnemonic == "JSR" && mode != AddressingMode::ABSOLUTE) {
        return false;
    }

    if (mnemonic == "RTS" && mode != AddressingMode::IMPLIED) {
        return false;
    }

    if (mnemonic == "BRK" && mode != AddressingMode::IMPLIED) {
        return false;
    }

    // Register transfer instructions are implied mode only
    if ((mnemonic == "TAX" || mnemonic == "TAY" || mnemonic == "TXA" || mnemonic == "TYA" || mnemonic == "TSX" ||
         mnemonic == "TXS") &&
        mode != AddressingMode::IMPLIED) {
        return false;
    }

    // Stack operations are implied mode only
    if ((mnemonic == "PHA" || mnemonic == "PHP" || mnemonic == "PLA" || mnemonic == "PLP") &&
        mode != AddressingMode::IMPLIED) {
        return false;
    }

    return true;
}

size_t Parser::get_instruction_size(const std::string& mnemonic, AddressingMode mode) {
    switch (mode) {
        case AddressingMode::IMPLIED:
        case AddressingMode::ACCUMULATOR:
            return 1;

        case AddressingMode::IMMEDIATE:
        case AddressingMode::ZERO_PAGE:
        case AddressingMode::ZERO_PAGE_X:
        case AddressingMode::ZERO_PAGE_Y:
        case AddressingMode::INDIRECT_X:
        case AddressingMode::INDIRECT_Y:
        case AddressingMode::RELATIVE:
            return 2;

        case AddressingMode::ABSOLUTE:
        case AddressingMode::ABSOLUTE_X:
        case AddressingMode::ABSOLUTE_Y:
        case AddressingMode::INDIRECT:
            return 3;

        default:
            return 1;
    }
}

std::string Parser::get_error_message(const std::string& message) const {
    std::stringstream ss;
    ss << "Parse error at line " << current().line << ", column " << current().column << ": " << message;
    return ss.str();
}

std::string Parser::get_current_location() const {
    std::stringstream ss;
    ss << "Line " << current().line << ", Column " << current().column;
    return ss.str();
}

}  // namespace assembler
