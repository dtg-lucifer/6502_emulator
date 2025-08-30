#include "codegen.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace assembler {

// Initialize instruction opcodes mapping - matches all opcodes from op_codes.h
const std::map<std::string, std::map<AddressingMode, byte>> CodeGenerator::instruction_opcodes_ = {
    // Load/Store Operations - LDA variants
    {"LDA",
     {{AddressingMode::IMMEDIATE, 0xA9},     // LDA_IM
      {AddressingMode::ZERO_PAGE, 0xA5},     // LDA_ZP
      {AddressingMode::ZERO_PAGE_X, 0xB5},   // LDA_ZPX
      {AddressingMode::ABSOLUTE, 0xAD},      // LDA_AB
      {AddressingMode::ABSOLUTE_X, 0xBD},    // LDA_ABSX
      {AddressingMode::ABSOLUTE_Y, 0xB9},    // LDA_ABSY
      {AddressingMode::INDIRECT_X, 0xA1},    // LDA_INX
      {AddressingMode::INDIRECT_Y, 0xB1}}},  // LDA_INY

    // LDX variants
    {"LDX",
     {{AddressingMode::IMMEDIATE, 0xA2},     // LDX_IM
      {AddressingMode::ZERO_PAGE, 0xA6},     // LDX_ZP
      {AddressingMode::ZERO_PAGE_Y, 0xB6},   // LDX_ZPY
      {AddressingMode::ABSOLUTE, 0xAE},      // LDX_AB
      {AddressingMode::ABSOLUTE_Y, 0xBE}}},  // LDX_ABSY

    // LDY variants
    {"LDY",
     {{AddressingMode::IMMEDIATE, 0xA0},     // LDY_IM
      {AddressingMode::ZERO_PAGE, 0xA4},     // LDY_ZP
      {AddressingMode::ZERO_PAGE_X, 0xB4},   // LDY_ZPX
      {AddressingMode::ABSOLUTE, 0xAC},      // LDY_AB
      {AddressingMode::ABSOLUTE_X, 0xBC}}},  // LDY_ABSX

    // STA variants
    {"STA",
     {{AddressingMode::ZERO_PAGE, 0x85},     // STA_ZP
      {AddressingMode::ZERO_PAGE_X, 0x95},   // STA_ZPX
      {AddressingMode::ABSOLUTE, 0x8D},      // STA_ABS
      {AddressingMode::ABSOLUTE_X, 0x9D},    // STA_ABSX
      {AddressingMode::ABSOLUTE_Y, 0x99},    // STA_ABSY
      {AddressingMode::INDIRECT_X, 0x81},    // STA_INX
      {AddressingMode::INDIRECT_Y, 0x91}}},  // STA_INY

    // STX variants
    {"STX",
     {{AddressingMode::ZERO_PAGE, 0x86},    // STX_ZP
      {AddressingMode::ZERO_PAGE_Y, 0x96},  // STX_ZPY
      {AddressingMode::ABSOLUTE, 0x8E}}},   // STX_ABS

    // STY variants
    {"STY",
     {{AddressingMode::ZERO_PAGE, 0x84},    // STY_ZP
      {AddressingMode::ZERO_PAGE_X, 0x94},  // STY_ZPX
      {AddressingMode::ABSOLUTE, 0x8C}}},   // STY_ABS

    // Register Transfer Operations
    {"TAX", {{AddressingMode::IMPLIED, 0xAA}}},  // TAX
    {"TAY", {{AddressingMode::IMPLIED, 0xA8}}},  // TAY
    {"TXA", {{AddressingMode::IMPLIED, 0x8A}}},  // TXA
    {"TYA", {{AddressingMode::IMPLIED, 0x98}}},  // TYA
    {"TSX", {{AddressingMode::IMPLIED, 0xBA}}},  // TSX
    {"TXS", {{AddressingMode::IMPLIED, 0x9A}}},  // TXS

    // Stack Operations
    {"PHA", {{AddressingMode::IMPLIED, 0x48}}},  // PHA
    {"PHP", {{AddressingMode::IMPLIED, 0x08}}},  // PHP
    {"PLA", {{AddressingMode::IMPLIED, 0x68}}},  // PLA
    {"PLP", {{AddressingMode::IMPLIED, 0x28}}},  // PLP

    // Logical Operations - AND variants
    {"AND",
     {{AddressingMode::IMMEDIATE, 0x29},     // AND_IM
      {AddressingMode::ZERO_PAGE, 0x25},     // AND_ZP
      {AddressingMode::ZERO_PAGE_X, 0x35},   // AND_ZPX
      {AddressingMode::ABSOLUTE, 0x2D},      // AND_AB
      {AddressingMode::ABSOLUTE_X, 0x3D},    // AND_ABSX
      {AddressingMode::ABSOLUTE_Y, 0x39},    // AND_ABSY
      {AddressingMode::INDIRECT_X, 0x21},    // AND_INX
      {AddressingMode::INDIRECT_Y, 0x31}}},  // AND_INY

    // EOR variants
    {"EOR",
     {{AddressingMode::IMMEDIATE, 0x49},     // EOR_IM
      {AddressingMode::ZERO_PAGE, 0x45},     // EOR_ZP
      {AddressingMode::ZERO_PAGE_X, 0x55},   // EOR_ZPX
      {AddressingMode::ABSOLUTE, 0x4D},      // EOR_AB
      {AddressingMode::ABSOLUTE_X, 0x5D},    // EOR_ABSX
      {AddressingMode::ABSOLUTE_Y, 0x59},    // EOR_ABSY
      {AddressingMode::INDIRECT_X, 0x41},    // EOR_INX
      {AddressingMode::INDIRECT_Y, 0x51}}},  // EOR_INY

    // ORA variants
    {"ORA",
     {{AddressingMode::IMMEDIATE, 0x09},     // ORA_IM
      {AddressingMode::ZERO_PAGE, 0x05},     // ORA_ZP
      {AddressingMode::ZERO_PAGE_X, 0x15},   // ORA_ZPX
      {AddressingMode::ABSOLUTE, 0x0D},      // ORA_AB
      {AddressingMode::ABSOLUTE_X, 0x1D},    // ORA_ABSX
      {AddressingMode::ABSOLUTE_Y, 0x19},    // ORA_ABSY
      {AddressingMode::INDIRECT_X, 0x01},    // ORA_INX
      {AddressingMode::INDIRECT_Y, 0x11}}},  // ORA_INY

    // BIT variants
    {"BIT",
     {{AddressingMode::ZERO_PAGE, 0x24},   // BIT_ZP
      {AddressingMode::ABSOLUTE, 0x2C}}},  // BIT_AB

    // Arithmetic Operations - ADC variants
    {"ADC",
     {{AddressingMode::IMMEDIATE, 0x69},     // ADC_IM
      {AddressingMode::ZERO_PAGE, 0x65},     // ADC_ZP
      {AddressingMode::ZERO_PAGE_X, 0x75},   // ADC_ZPX
      {AddressingMode::ABSOLUTE, 0x6D},      // ADC_AB
      {AddressingMode::ABSOLUTE_X, 0x7D},    // ADC_ABSX
      {AddressingMode::ABSOLUTE_Y, 0x79},    // ADC_ABSY
      {AddressingMode::INDIRECT_X, 0x61},    // ADC_INX
      {AddressingMode::INDIRECT_Y, 0x71}}},  // ADC_INY

    // SBC variants
    {"SBC",
     {{AddressingMode::IMMEDIATE, 0xE9},     // SBC_IM
      {AddressingMode::ZERO_PAGE, 0xE5},     // SBC_ZP
      {AddressingMode::ZERO_PAGE_X, 0xF5},   // SBC_ZPX
      {AddressingMode::ABSOLUTE, 0xED},      // SBC_AB
      {AddressingMode::ABSOLUTE_X, 0xFD},    // SBC_ABSX
      {AddressingMode::ABSOLUTE_Y, 0xF9},    // SBC_ABSY
      {AddressingMode::INDIRECT_X, 0xE1},    // SBC_INX
      {AddressingMode::INDIRECT_Y, 0xF1}}},  // SBC_INY

    // Compare Operations - CMP variants
    {"CMP",
     {{AddressingMode::IMMEDIATE, 0xC9},     // CMP_IM
      {AddressingMode::ZERO_PAGE, 0xC5},     // CMP_ZP
      {AddressingMode::ZERO_PAGE_X, 0xD5},   // CMP_ZPX
      {AddressingMode::ABSOLUTE, 0xCD},      // CMP_AB
      {AddressingMode::ABSOLUTE_X, 0xDD},    // CMP_ABSX
      {AddressingMode::ABSOLUTE_Y, 0xD9},    // CMP_ABSY
      {AddressingMode::INDIRECT_X, 0xC1},    // CMP_INX
      {AddressingMode::INDIRECT_Y, 0xD1}}},  // CMP_INY

    // CPX variants
    {"CPX",
     {{AddressingMode::IMMEDIATE, 0xE0},   // CPX_IM
      {AddressingMode::ZERO_PAGE, 0xE4},   // CPX_ZP
      {AddressingMode::ABSOLUTE, 0xEC}}},  // CPX_AB

    // CPY variants
    {"CPY",
     {{AddressingMode::IMMEDIATE, 0xC0},   // CPY_IM
      {AddressingMode::ZERO_PAGE, 0xC4},   // CPY_ZP
      {AddressingMode::ABSOLUTE, 0xCC}}},  // CPY_AB

    // Increment and Decrement Operations - INC variants
    {"INC",
     {{AddressingMode::ZERO_PAGE, 0xE6},     // INC_ZP
      {AddressingMode::ZERO_PAGE_X, 0xF6},   // INC_ZPX
      {AddressingMode::ABSOLUTE, 0xEE},      // INC_AB
      {AddressingMode::ABSOLUTE_X, 0xFE}}},  // INC_ABSX

    {"INX", {{AddressingMode::IMPLIED, 0xE8}}},  // INX
    {"INY", {{AddressingMode::IMPLIED, 0xC8}}},  // INY

    // DEC variants
    {"DEC",
     {{AddressingMode::ZERO_PAGE, 0xC6},     // DEC_ZP
      {AddressingMode::ZERO_PAGE_X, 0xD6},   // DEC_ZPX
      {AddressingMode::ABSOLUTE, 0xCE},      // DEC_AB
      {AddressingMode::ABSOLUTE_X, 0xDE}}},  // DEC_ABSX

    {"DEX", {{AddressingMode::IMPLIED, 0xCA}}},  // DEX
    {"DEY", {{AddressingMode::IMPLIED, 0x88}}},  // DEY

    // Shifts and Rotates - ASL variants
    {"ASL",
     {{AddressingMode::ACCUMULATOR, 0x0A},   // ASL_A
      {AddressingMode::ZERO_PAGE, 0x06},     // ASL_ZP
      {AddressingMode::ZERO_PAGE_X, 0x16},   // ASL_ZPX
      {AddressingMode::ABSOLUTE, 0x0E},      // ASL_AB
      {AddressingMode::ABSOLUTE_X, 0x1E}}},  // ASL_ABSX

    // LSR variants
    {"LSR",
     {{AddressingMode::ACCUMULATOR, 0x4A},   // LSR_A
      {AddressingMode::ZERO_PAGE, 0x46},     // LSR_ZP
      {AddressingMode::ZERO_PAGE_X, 0x56},   // LSR_ZPX
      {AddressingMode::ABSOLUTE, 0x4E},      // LSR_AB
      {AddressingMode::ABSOLUTE_X, 0x5E}}},  // LSR_ABSX

    // ROL variants
    {"ROL",
     {{AddressingMode::ACCUMULATOR, 0x2A},   // ROL_A
      {AddressingMode::ZERO_PAGE, 0x26},     // ROL_ZP
      {AddressingMode::ZERO_PAGE_X, 0x36},   // ROL_ZPX
      {AddressingMode::ABSOLUTE, 0x2E},      // ROL_AB
      {AddressingMode::ABSOLUTE_X, 0x3E}}},  // ROL_ABSX

    // ROR variants
    {"ROR",
     {{AddressingMode::ACCUMULATOR, 0x6A},   // ROR_A
      {AddressingMode::ZERO_PAGE, 0x66},     // ROR_ZP
      {AddressingMode::ZERO_PAGE_X, 0x76},   // ROR_ZPX
      {AddressingMode::ABSOLUTE, 0x6E},      // ROR_AB
      {AddressingMode::ABSOLUTE_X, 0x7E}}},  // ROR_ABSX

    // Jump and Branch Operations
    {"JMP",
     {{AddressingMode::ABSOLUTE, 0x4C},           // JMP
      {AddressingMode::INDIRECT, 0x6C}}},         // JMPI
    {"JSR", {{AddressingMode::ABSOLUTE, 0x20}}},  // JSR
    {"RTS", {{AddressingMode::IMPLIED, 0x60}}},   // RTS
    {"BRK", {{AddressingMode::IMPLIED, 0x00}}},   // BRK

    // Branch Operations
    {"BCC", {{AddressingMode::RELATIVE, 0x90}}},  // BCC
    {"BCS", {{AddressingMode::RELATIVE, 0xB0}}},  // BCS
    {"BEQ", {{AddressingMode::RELATIVE, 0xF0}}},  // BEQ
    {"BMI", {{AddressingMode::RELATIVE, 0x30}}},  // BMI
    {"BNE", {{AddressingMode::RELATIVE, 0xD0}}},  // BNE
    {"BPL", {{AddressingMode::RELATIVE, 0x10}}},  // BPL
    {"BVC", {{AddressingMode::RELATIVE, 0x50}}},  // BVC
    {"BVS", {{AddressingMode::RELATIVE, 0x70}}},  // BVS

    // Status Flag Operations
    {"CLC", {{AddressingMode::IMPLIED, 0x18}}},  // CLC
    {"CLD", {{AddressingMode::IMPLIED, 0xD8}}},  // CLD
    {"CLI", {{AddressingMode::IMPLIED, 0x58}}},  // CLI
    {"CLV", {{AddressingMode::IMPLIED, 0xB8}}},  // CLV
    {"SEC", {{AddressingMode::IMPLIED, 0x38}}},  // SEC
    {"SED", {{AddressingMode::IMPLIED, 0xF8}}},  // SED
    {"SEI", {{AddressingMode::IMPLIED, 0x78}}},  // SEI

    // No Operation
    {"NOP", {{AddressingMode::IMPLIED, 0xEA}}}  // NOP
};

CodeGenerator::CodeGenerator(const IR& ir) : ir_(ir), start_address_(0xFFFF), end_address_(0x0000) {}

AssemblyResult CodeGenerator::generate() {
    AssemblyResult result;
    errors_.clear();
    output_memory_.clear();

    word current_address = 0;

    for (const auto& node : ir_.get_nodes()) {
        switch (node.type) {
            case IRNodeType::DIRECTIVE: {
                const auto& directive = std::get<Directive>(node.data);
                if (directive.name == "org") {
                    current_address = std::get<word>(directive.value);
                } else {
                    if (!encode_directive(directive, current_address)) {
                        add_error("Failed to encode directive at line " + std::to_string(node.line));
                    }
                    current_address += get_instruction_size(AddressingMode::IMPLIED);
                    if (directive.name == "word") {
                        current_address += 1;  // word is 2 bytes
                    }
                }
                break;
            }

            case IRNodeType::LABEL:
                // Labels don't generate code
                break;

            case IRNodeType::INSTRUCTION: {
                const auto& instruction = std::get<Instruction>(node.data);
                if (!encode_instruction(instruction, current_address)) {
                    add_error("Failed to encode instruction " + instruction.mnemonic + " at line " +
                              std::to_string(node.line));
                }
                current_address += get_instruction_size(instruction.mode);
                break;
            }
        }
    }

    // Update address range
    if (!output_memory_.empty()) {
        start_address_ = output_memory_.begin()->first;
        end_address_ = output_memory_.rbegin()->first;
    }

    // Populate result
    result.memory_map = output_memory_;
    result.binary_data = get_binary_data();
    result.errors = errors_;
    result.success = errors_.empty();

    return result;
}

bool CodeGenerator::encode_instruction(const Instruction& instruction, word address) {
    byte opcode = get_opcode(instruction.mnemonic, instruction.mode);
    if (opcode == 0 && instruction.mnemonic != "BRK") {
        add_error("Invalid instruction/addressing mode combination: " + instruction.mnemonic);
        return false;
    }

    // Write opcode
    write_byte(address, opcode);

    // Encode operand
    std::vector<byte> operand_bytes = encode_operand(instruction.operand, instruction.mode, address);
    for (size_t i = 0; i < operand_bytes.size(); ++i) {
        write_byte(address + 1 + i, operand_bytes[i]);
    }

    return true;
}

bool CodeGenerator::encode_directive(const Directive& directive, word address) {
    if (directive.name == "word") {
        word value = std::get<word>(directive.value);
        write_word(address, value);
        return true;
    } else if (directive.name == "byte") {
        byte value = static_cast<byte>(std::get<word>(directive.value));
        write_byte(address, value);
        return true;
    }

    return false;
}

byte CodeGenerator::get_opcode(const std::string& mnemonic, AddressingMode mode) const {
    auto instruction_it = instruction_opcodes_.find(mnemonic);
    if (instruction_it == instruction_opcodes_.end()) {
        return 0;
    }

    auto mode_it = instruction_it->second.find(mode);
    if (mode_it == instruction_it->second.end()) {
        return 0;
    }

    return mode_it->second;
}

std::vector<byte> CodeGenerator::encode_operand(const Operand& operand, AddressingMode mode,
                                                word current_address) const {
    std::vector<byte> result;

    if (std::holds_alternative<ImmediateOperand>(operand)) {
        const auto& imm = std::get<ImmediateOperand>(operand);
        result.push_back(static_cast<byte>(imm.value & 0xFF));

    } else if (std::holds_alternative<AddressOperand>(operand)) {
        const auto& addr = std::get<AddressOperand>(operand);

        switch (mode) {
            case AddressingMode::ZERO_PAGE:
            case AddressingMode::ZERO_PAGE_X:
            case AddressingMode::ZERO_PAGE_Y:
            case AddressingMode::INDIRECT_X:
            case AddressingMode::INDIRECT_Y:
                result.push_back(static_cast<byte>(addr.address & 0xFF));
                break;

            case AddressingMode::ABSOLUTE:
            case AddressingMode::ABSOLUTE_X:
            case AddressingMode::ABSOLUTE_Y:
            case AddressingMode::INDIRECT:
                result.push_back(static_cast<byte>(addr.address & 0xFF));         // Low byte
                result.push_back(static_cast<byte>((addr.address >> 8) & 0xFF));  // High byte
                break;

            case AddressingMode::RELATIVE: {
                // Calculate relative offset
                byte offset = resolve_relative_address(addr.address, current_address);
                result.push_back(offset);
                break;
            }

            default:
                break;
        }
    }

    return result;
}

size_t CodeGenerator::get_instruction_size(AddressingMode mode) const {
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

bool CodeGenerator::is_valid_instruction(const std::string& mnemonic, AddressingMode mode) const {
    auto instruction_it = instruction_opcodes_.find(mnemonic);
    if (instruction_it == instruction_opcodes_.end()) {
        return false;
    }

    return instruction_it->second.find(mode) != instruction_it->second.end();
}

word CodeGenerator::resolve_operand_address(const Operand& operand, word current_address) const {
    if (std::holds_alternative<AddressOperand>(operand)) {
        return std::get<AddressOperand>(operand).address;
    } else if (std::holds_alternative<ImmediateOperand>(operand)) {
        return std::get<ImmediateOperand>(operand).value;
    }

    return 0;
}

byte CodeGenerator::resolve_relative_address(word target, word current_address) const {
    // Calculate relative offset for branch instructions
    // Branch target is relative to the instruction after the branch (PC + 2)
    int16_t offset = static_cast<int16_t>(target) - static_cast<int16_t>(current_address + 2);

    if (offset < -128 || offset > 127) {
        // Branch out of range - this should be caught in validation
        return 0;
    }

    return static_cast<byte>(offset & 0xFF);
}

bool CodeGenerator::validate_zero_page_address(word address) const {
    return address <= 0xFF;
}

bool CodeGenerator::validate_relative_branch(word target, word current_address) const {
    int16_t offset = static_cast<int16_t>(target) - static_cast<int16_t>(current_address + 2);
    return offset >= -128 && offset <= 127;
}

bool CodeGenerator::validate_address_range(word address) const {
    return address <= 0xFFFF;
}

void CodeGenerator::write_byte(word address, byte value) {
    output_memory_[address] = value;

    // Update address range
    if (address < start_address_) {
        start_address_ = address;
    }
    if (address > end_address_) {
        end_address_ = address;
    }
}

void CodeGenerator::write_word(word address, word value) {
    write_byte(address, static_cast<byte>(value & 0xFF));             // Low byte
    write_byte(address + 1, static_cast<byte>((value >> 8) & 0xFF));  // High byte
}

std::vector<byte> CodeGenerator::get_binary_data() const {
    if (output_memory_.empty()) {
        return {};
    }

    std::vector<byte> result;
    word start = output_memory_.begin()->first;
    word end = output_memory_.rbegin()->first;

    for (word addr = start; addr <= end; ++addr) {
        auto it = output_memory_.find(addr);
        if (it != output_memory_.end()) {
            result.push_back(it->second);
        } else {
            result.push_back(0x00);  // Fill gaps with zeros
        }
    }

    return result;
}

std::vector<byte> CodeGenerator::get_binary_data(word start_addr, word end_addr) const {
    std::vector<byte> result;

    for (word addr = start_addr; addr <= end_addr; ++addr) {
        auto it = output_memory_.find(addr);
        if (it != output_memory_.end()) {
            result.push_back(it->second);
        } else {
            result.push_back(0x00);  // Fill gaps with zeros
        }
    }

    return result;
}

bool CodeGenerator::write_binary_file(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    std::vector<byte> data = get_binary_data();
    file.write(reinterpret_cast<const char*>(data.data()), data.size());

    return file.good();
}

const std::map<word, byte>& CodeGenerator::get_memory_map() const {
    return output_memory_;
}

bool CodeGenerator::has_data_at(word address) const {
    return output_memory_.find(address) != output_memory_.end();
}

byte CodeGenerator::get_byte_at(word address) const {
    auto it = output_memory_.find(address);
    return it != output_memory_.end() ? it->second : 0x00;
}

word CodeGenerator::get_start_address() const {
    return start_address_;
}

word CodeGenerator::get_end_address() const {
    return end_address_;
}

size_t CodeGenerator::get_total_size() const {
    return output_memory_.empty() ? 0 : (end_address_ - start_address_ + 1);
}

const std::vector<std::string>& CodeGenerator::get_errors() const {
    return errors_;
}

bool CodeGenerator::has_errors() const {
    return !errors_.empty();
}

void CodeGenerator::print_memory_map() const {
    std::cout << "Memory Map:\n";
    std::cout << "-----------\n";

    for (const auto& entry : output_memory_) {
        std::cout << format_address(entry.first) << ": " << format_byte(entry.second) << "\n";
    }

    std::cout << std::endl;
}

void CodeGenerator::print_disassembly() const {
    std::cout << "Disassembly:\n";
    std::cout << "------------\n";

    // This would require a more complex disassembler
    // For now, just show the raw bytes
    print_memory_map();
}

void CodeGenerator::add_error(const std::string& message) {
    errors_.push_back(message);
}

std::string CodeGenerator::format_address(word address) const {
    std::stringstream ss;
    ss << "$" << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << address;
    return ss.str();
}

std::string CodeGenerator::format_byte(byte value) const {
    std::stringstream ss;
    ss << "$" << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<int>(value);
    return ss.str();
}

std::string CodeGenerator::format_word(word value) const {
    std::stringstream ss;
    ss << "$" << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << value;
    return ss.str();
}

}  // namespace assembler
