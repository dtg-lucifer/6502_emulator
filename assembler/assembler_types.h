#ifndef ASSEMBLER_TYPES_H
#define ASSEMBLER_TYPES_H

#include <map>
#include <string>
#include <variant>
#include <vector>

#include "types.h"

namespace assembler {

// Token types for the lexer
enum class TokenType {
    // Literals and identifiers
    IDENTIFIER,
    NUMBER,
    STRING,

    // Operators and symbols
    PLUS,       // +
    MINUS,      // -
    DOLLAR,     // $
    HASH,       // #
    DOT,        // .
    COMMA,      // ,
    COLON,      // :
    SEMICOLON,  // ;

    // Brackets and parentheses
    LPAREN,    // (
    RPAREN,    // )
    LBRACKET,  // [
    RBRACKET,  // ]

    // Addressing mode indicators
    X_INDEX,  // ,X
    Y_INDEX,  // ,Y

    // Directives
    ORG,   // .org
    WORD,  // .word
    BYTE,  // .byte

    // Special tokens
    NEWLINE,
    EOF_TOKEN,
    UNKNOWN
};

// Addressing modes supported by the 6502
enum class AddressingMode {
    IMPLIED,      // NOP
    ACCUMULATOR,  // ASL A
    IMMEDIATE,    // LDA #$FF
    ZERO_PAGE,    // LDA $80
    ZERO_PAGE_X,  // LDA $80,X
    ZERO_PAGE_Y,  // LDA $80,Y
    ABSOLUTE,     // LDA $8000
    ABSOLUTE_X,   // LDA $8000,X
    ABSOLUTE_Y,   // LDA $8000,Y
    INDIRECT,     // JMP ($8000)
    INDIRECT_X,   // LDA ($80,X)
    INDIRECT_Y,   // LDA ($80),Y
    RELATIVE      // BEQ label
};

// Token structure
struct Token {
    TokenType type;
    std::string value;
    size_t line;
    size_t column;

    Token(TokenType t, const std::string& v, size_t l, size_t c) : type(t), value(v), line(l), column(c) {}
};

// Operand types
struct ImmediateOperand {
    word value;
};

struct AddressOperand {
    word address;
    AddressingMode mode;
};

struct LabelOperand {
    std::string label;
    AddressingMode mode;
};

using Operand = std::variant<ImmediateOperand, AddressOperand, LabelOperand>;

// Instruction structure
struct Instruction {
    std::string mnemonic;
    AddressingMode mode;
    Operand operand;
    size_t line;

    Instruction(const std::string& m, AddressingMode am, const Operand& op, size_t l)
        : mnemonic(m), mode(am), operand(op), line(l) {}
};

// Directive structure
struct Directive {
    std::string name;
    std::variant<word, std::string> value;
    size_t line;

    Directive(const std::string& n, const std::variant<word, std::string>& v, size_t l) : name(n), value(v), line(l) {}
};

// Label structure
struct Label {
    std::string name;
    word address;
    size_t line;

    Label(const std::string& n, word addr, size_t l) : name(n), address(addr), line(l) {}
};

// IR Node types
enum class IRNodeType { INSTRUCTION, DIRECTIVE, LABEL };

// Intermediate Representation Node
struct IRNode {
    IRNodeType type;
    std::variant<Instruction, Directive, Label> data;
    size_t line;

    IRNode(const Instruction& inst, size_t l) : type(IRNodeType::INSTRUCTION), data(inst), line(l) {}

    IRNode(const Directive& dir, size_t l) : type(IRNodeType::DIRECTIVE), data(dir), line(l) {}

    IRNode(const Label& lbl, size_t l) : type(IRNodeType::LABEL), data(lbl), line(l) {}
};

// Assembly result structure
struct AssemblyResult {
    std::vector<byte> binary_data;
    std::map<word, byte> memory_map;
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
    bool success;

    AssemblyResult() : success(false) {}
};

}  // namespace assembler

#endif  // ASSEMBLER_TYPES_H
