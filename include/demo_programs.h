#ifndef DEMO_PROGRAMS_H
#define DEMO_PROGRAMS_H

#include <map>
#include <vector>

#include "types.h"

namespace demo_programs {

// Demo program showcasing only LDA instructions with various addressing modes
std::map<u32, std::vector<byte>> get_lda_demo();

// Demo program showcasing only LDX instructions with various addressing modes
std::map<u32, std::vector<byte>> get_ldx_demo();

// Demo program showcasing only LDY instructions with various addressing modes
std::map<u32, std::vector<byte>> get_ldy_demo();

// A comprehensive demo program showcasing various instructions and addressing modes
// including LDA, LDX, LDY, JSR, RTS with their different addressing variants
std::map<u32, std::vector<byte>> get_instruction_demo();

}  // namespace demo_programs

#endif  // DEMO_PROGRAMS_H
