#include "emulator_api.h"

#include <iostream>

#include "reader.h"

EmulatorApi::EmulatorApi() : initialized(false) {}

void EmulatorApi::initialize() {
    mem.init();
    initialized = true;
}

bool EmulatorApi::load_binary(const std::string& file_path, u32 load_address) {
    if (!initialized) {
        initialize();
    }

    try {
        binary_reader::read_from_binary_file(cpu, mem, file_path, load_address);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error loading binary file: " << e.what() << std::endl;
        return false;
    }
}

CpuState EmulatorApi::execute_program(bool reset_first, word reset_vector_override) {
    if (!initialized) {
        initialize();
    }

    if (reset_first) {
        cpu.reset(mem);

        // Override reset vector if specified
        if (reset_vector_override != 0) {
            cpu.PC = reset_vector_override;
        }
    }

    bool completed = false;
    cpu.execute(0, mem, &completed, false);  // Run to completion, not in testing mode

    return get_current_state();
}

CpuState EmulatorApi::get_current_state() const {
    CpuState state;

    // Copy register values
    state.A = cpu.A;
    state.X = cpu.X;
    state.Y = cpu.Y;
    state.SP = cpu.SP;
    state.PC = cpu.PC;

    // Copy flags (note the reversed order from CPU to CpuState)
    state.flags.C = cpu.FLAGS_C;
    state.flags.Z = cpu.FLAGS_Z;
    state.flags.I = cpu.FLAGS_I;
    state.flags.D = cpu.FLAGS_D;
    state.flags.B = cpu.FLAGS_B;
    state.flags.U = cpu.FLAGS_U;
    state.flags.V = cpu.FLAGS_V;
    state.flags.N = cpu.FLAGS_N;

    // Set execution statistics
    // In a real implementation, we'd get these from the CPU directly
    // For now we'll use placeholders
    state.cycles_used = 0;
    state.execution_completed = false;

    return state;
}

void EmulatorApi::set_register(Register reg, byte value) {
    cpu.set(reg, value);
}

void EmulatorApi::set_pc(word value) {
    cpu.PC = value;
}

void EmulatorApi::set_sp(byte value) {
    cpu.SP = value;
}

byte EmulatorApi::read_memory(word address) const {
    return mem[address];
}

void EmulatorApi::write_memory(word address, byte value) {
    mem[address] = value;
}

void EmulatorApi::set_memory_block(word start_address, const std::vector<byte>& data) {
    for (size_t i = 0; i < data.size(); ++i) {
        mem[start_address + i] = data[i];
    }
}

void EmulatorApi::set_reset_vector(word address) {
    // Set little endian (low byte first, then high byte)
    mem[0xFFFC] = address & 0xFF;         // Low byte
    mem[0xFFFD] = (address >> 8) & 0xFF;  // High byte
}
