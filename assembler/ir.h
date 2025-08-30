#ifndef IR_H
#define IR_H

#include <map>
#include <string>
#include <vector>

#include "assembler_types.h"

namespace assembler {

class IR {
   private:
    std::vector<IRNode> nodes_;
    std::map<std::string, word> symbol_table_;
    word current_address_;

    // Helper methods
    void build_symbol_table();
    void resolve_labels();
    word calculate_address(const IRNode& node);
    size_t get_node_size(const IRNode& node) const;

   public:
    explicit IR(const std::vector<IRNode>& nodes);

    // Main processing methods
    void process();

    // Symbol resolution
    bool add_symbol(const std::string& name, word address);
    word get_symbol_address(const std::string& name) const;
    bool has_symbol(const std::string& name) const;

    // Address calculation
    void calculate_addresses();
    word get_current_address() const;
    void set_current_address(word address);

    // Validation
    bool validate();
    std::vector<std::string> get_errors() const;

    // Accessors
    const std::vector<IRNode>& get_nodes() const;
    const std::map<std::string, word>& get_symbol_table() const;

    // Debug output
    void print_symbol_table() const;
    void print_nodes() const;

   private:
    std::vector<std::string> errors_;
    bool processed_;
};

}  // namespace assembler

#endif  // IR_H
