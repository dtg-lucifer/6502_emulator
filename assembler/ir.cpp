#include "ir.h"

#include <iostream>
#include <sstream>

namespace assembler {

IR::IR(const std::vector<IRNode>& nodes) : nodes_(nodes), current_address_(0), processed_(false) {}

void IR::process() {
    if (processed_) {
        return;
    }

    errors_.clear();

    // First pass: build symbol table and calculate addresses
    build_symbol_table();
    calculate_addresses();

    // Second pass: resolve labels
    resolve_labels();

    // Validate the IR
    if (!validate()) {
        processed_ = false;
        return;
    }

    processed_ = true;
}

void IR::build_symbol_table() {
    symbol_table_.clear();
    current_address_ = 0;

    for (const auto& node : nodes_) {
        switch (node.type) {
            case IRNodeType::LABEL: {
                const auto& label = std::get<Label>(node.data);
                if (!add_symbol(label.name, current_address_)) {
                    errors_.push_back("Duplicate label: " + label.name + " at line " + std::to_string(node.line));
                }
                break;
            }

            case IRNodeType::DIRECTIVE: {
                const auto& directive = std::get<Directive>(node.data);
                if (directive.name == "org") {
                    word address = std::get<word>(directive.value);
                    current_address_ = address;
                }
                break;
            }

            case IRNodeType::INSTRUCTION: {
                // Instructions advance the address by their size
                current_address_ += get_node_size(node);
                break;
            }
        }
    }
}

void IR::calculate_addresses() {
    current_address_ = 0;

    for (auto& node : nodes_) {
        switch (node.type) {
            case IRNodeType::DIRECTIVE: {
                auto& directive = std::get<Directive>(node.data);
                if (directive.name == "org") {
                    word address = std::get<word>(directive.value);
                    current_address_ = address;
                }
                break;
            }

            case IRNodeType::LABEL: {
                auto& label = std::get<Label>(node.data);
                label.address = current_address_;
                // Update symbol table with calculated address
                symbol_table_[label.name] = current_address_;
                break;
            }

            case IRNodeType::INSTRUCTION: {
                current_address_ += get_node_size(node);
                break;
            }
        }
    }
}

void IR::resolve_labels() {
    // This pass resolves label operands to their addresses
    for (auto& node : nodes_) {
        if (node.type == IRNodeType::INSTRUCTION) {
            auto& instruction = std::get<Instruction>(node.data);

            // Check if operand is a label
            if (std::holds_alternative<LabelOperand>(instruction.operand)) {
                const auto& label_operand = std::get<LabelOperand>(instruction.operand);

                if (has_symbol(label_operand.label)) {
                    word address = get_symbol_address(label_operand.label);

                    // Replace label operand with address operand
                    instruction.operand = AddressOperand{address, label_operand.mode};
                } else {
                    errors_.push_back("Undefined label: " + label_operand.label + " at line " +
                                      std::to_string(node.line));
                }
            }
        } else if (node.type == IRNodeType::DIRECTIVE) {
            auto& directive = std::get<Directive>(node.data);

            if (directive.name == "word" && std::holds_alternative<std::string>(directive.value)) {
                const std::string& label = std::get<std::string>(directive.value);

                if (has_symbol(label)) {
                    word address = get_symbol_address(label);
                    directive.value = address;
                } else {
                    errors_.push_back("Undefined label in .word directive: " + label + " at line " +
                                      std::to_string(node.line));
                }
            }
        }
    }
}

word IR::calculate_address(const IRNode& node) {
    // This method calculates the address where a node should be placed
    // It's used during the address calculation phase
    return current_address_;
}

size_t IR::get_node_size(const IRNode& node) const {
    switch (node.type) {
        case IRNodeType::LABEL:
            return 0;  // Labels don't take up space

        case IRNodeType::DIRECTIVE: {
            const auto& directive = std::get<Directive>(node.data);
            if (directive.name == "word") {
                return 2;  // 16-bit word
            } else if (directive.name == "byte") {
                return 1;  // 8-bit byte
            } else if (directive.name == "org") {
                return 0;  // Origin directive doesn't take space
            }
            return 0;
        }

        case IRNodeType::INSTRUCTION: {
            const auto& instruction = std::get<Instruction>(node.data);

            switch (instruction.mode) {
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
    }

    return 0;
}

bool IR::add_symbol(const std::string& name, word address) {
    if (symbol_table_.find(name) != symbol_table_.end()) {
        return false;  // Symbol already exists
    }

    symbol_table_[name] = address;
    return true;
}

word IR::get_symbol_address(const std::string& name) const {
    auto it = symbol_table_.find(name);
    if (it != symbol_table_.end()) {
        return it->second;
    }
    return 0;  // Return 0 if symbol not found
}

bool IR::has_symbol(const std::string& name) const {
    return symbol_table_.find(name) != symbol_table_.end();
}

word IR::get_current_address() const {
    return current_address_;
}

void IR::set_current_address(word address) {
    current_address_ = address;
}

bool IR::validate() {
    bool valid = true;

    // Check for unresolved labels
    for (const auto& node : nodes_) {
        if (node.type == IRNodeType::INSTRUCTION) {
            const auto& instruction = std::get<Instruction>(node.data);

            if (std::holds_alternative<LabelOperand>(instruction.operand)) {
                const auto& label_operand = std::get<LabelOperand>(instruction.operand);
                if (!has_symbol(label_operand.label)) {
                    errors_.push_back("Undefined label: " + label_operand.label + " at line " +
                                      std::to_string(node.line));
                    valid = false;
                }
            }
        }
    }

    // Validate addressing modes and operand ranges
    for (const auto& node : nodes_) {
        if (node.type == IRNodeType::INSTRUCTION) {
            const auto& instruction = std::get<Instruction>(node.data);

            // Validate zero page addressing
            if (instruction.mode == AddressingMode::ZERO_PAGE || instruction.mode == AddressingMode::ZERO_PAGE_X ||
                instruction.mode == AddressingMode::ZERO_PAGE_Y) {
                if (std::holds_alternative<AddressOperand>(instruction.operand)) {
                    const auto& addr_operand = std::get<AddressOperand>(instruction.operand);
                    if (addr_operand.address > 0xFF) {
                        errors_.push_back("Zero page address out of range at line " + std::to_string(node.line));
                        valid = false;
                    }
                }
            }

            // Validate immediate operand range
            if (instruction.mode == AddressingMode::IMMEDIATE) {
                if (std::holds_alternative<ImmediateOperand>(instruction.operand)) {
                    const auto& imm_operand = std::get<ImmediateOperand>(instruction.operand);
                    if (imm_operand.value > 0xFF) {
                        errors_.push_back("Immediate value out of range at line " + std::to_string(node.line));
                        valid = false;
                    }
                }
            }
        }
    }

    return valid;
}

std::vector<std::string> IR::get_errors() const {
    return errors_;
}

const std::vector<IRNode>& IR::get_nodes() const {
    return nodes_;
}

const std::map<std::string, word>& IR::get_symbol_table() const {
    return symbol_table_;
}

void IR::print_symbol_table() const {
    std::cout << "Symbol Table:\n";
    std::cout << "-------------\n";

    for (const auto& symbol : symbol_table_) {
        std::cout << symbol.first << " = $" << std::hex << symbol.second << std::dec << "\n";
    }

    std::cout << std::endl;
}

void IR::print_nodes() const {
    std::cout << "IR Nodes:\n";
    std::cout << "---------\n";

    word address = 0;

    for (const auto& node : nodes_) {
        std::cout << "Line " << node.line << ": ";

        switch (node.type) {
            case IRNodeType::LABEL: {
                const auto& label = std::get<Label>(node.data);
                std::cout << "LABEL " << label.name << " @ $" << std::hex << label.address << std::dec;
                break;
            }

            case IRNodeType::DIRECTIVE: {
                const auto& directive = std::get<Directive>(node.data);
                std::cout << "DIRECTIVE ." << directive.name;

                if (std::holds_alternative<word>(directive.value)) {
                    std::cout << " $" << std::hex << std::get<word>(directive.value) << std::dec;
                } else {
                    std::cout << " " << std::get<std::string>(directive.value);
                }

                if (directive.name == "org") {
                    address = std::get<word>(directive.value);
                } else {
                    address += get_node_size(node);
                }
                break;
            }

            case IRNodeType::INSTRUCTION: {
                const auto& instruction = std::get<Instruction>(node.data);
                std::cout << "INSTRUCTION $" << std::hex << address << std::dec << ": " << instruction.mnemonic;

                // Print operand if present
                if (std::holds_alternative<ImmediateOperand>(instruction.operand)) {
                    const auto& imm = std::get<ImmediateOperand>(instruction.operand);
                    std::cout << " #$" << std::hex << imm.value << std::dec;
                } else if (std::holds_alternative<AddressOperand>(instruction.operand)) {
                    const auto& addr = std::get<AddressOperand>(instruction.operand);
                    std::cout << " $" << std::hex << addr.address << std::dec;
                } else if (std::holds_alternative<LabelOperand>(instruction.operand)) {
                    const auto& label = std::get<LabelOperand>(instruction.operand);
                    std::cout << " " << label.label;
                }

                address += get_node_size(node);
                break;
            }
        }

        std::cout << std::endl;
    }
}

}  // namespace assembler
