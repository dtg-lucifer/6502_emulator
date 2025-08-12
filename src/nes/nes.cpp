#include "nes/nes.h"

#include <iostream>
#include <string>

#include "colors.h"

// NES System Implementation

NES::NES() : cpuMemory(), mapper(cpuMemory), cartridge(nullptr) {
    // Initialize CPU memory
    cpuMemory.init();
    // Initialize components
    std::cout << colors::CYAN << "Initializing NES system..." << colors::RESET << std::endl;

    // Setup memory mapping
    setupMemoryMap();
}

NES::~NES() {
    // Clean up resources
    if (cartridge) {
        delete cartridge;
    }
}

void NES::setupMemoryMap() {
    // Register memory handlers for different regions

    // RAM and mirrors (0x0000-0x1FFF)
    mapper.registerHandler(
        RAM_START, RAM_MIRROR_END, [this](u32 addr) { return this->readRAM(addr); },
        [this](u32 addr, byte value) { this->writeRAM(addr, value); });

    // PPU registers and mirrors (0x2000-0x3FFF)
    mapper.registerHandler(
        PPU_REGISTERS_START, PPU_REGISTERS_MIRROR_END, [this](u32 addr) { return this->readPPU(addr); },
        [this](u32 addr, byte value) { this->writePPU(addr, value); });

    // APU and I/O registers (0x4000-0x4017)
    mapper.registerHandler(
        APU_IO_REGISTERS_START, APU_IO_REGISTERS_END, [this](u32 addr) { return this->readAPU(addr); },
        [this](u32 addr, byte value) { this->writeAPU(addr, value); });

    // Cartridge space (0x4020-0xFFFF)
    mapper.registerHandler(
        CARTRIDGE_START, CARTRIDGE_END, [this](u32 addr) { return this->readCartridge(addr); },
        [this](u32 addr, byte value) { this->writeCartridge(addr, value); });
}

bool NES::loadCartridge(const std::string& path) {
    // Check if a cartridge is already loaded
    if (cartridge) {
        delete cartridge;
    }

    // Create and load the new cartridge
    cartridge = new Cartridge();
    bool result = cartridge->load(path);

    if (result) {
        std::cout << colors::GREEN << "Cartridge loaded successfully: " << path << colors::RESET << std::endl;
        // Connect the cartridge to the PPU
        ppu.connectCartridge(cartridge);
        // Reset the system with the new cartridge
        reset();
    } else {
        std::cout << colors::RED << "Failed to load cartridge: " << path << colors::RESET << std::endl;
        delete cartridge;
        cartridge = nullptr;
    }

    return result;
}

void NES::reset() {
    // Reset CPU
    cpu.reset(cpuMemory);

    // Reset PPU
    ppu.reset();

    // Reset APU
    apu.reset();

    // Reset cartridge if present
    if (cartridge) {
        cartridge->reset();
    }

    // Set the CPU's program counter to the reset vector
    word resetVector = mapper.read(0xFFFC) | (mapper.read(0xFFFD) << 8);
    cpu.PC = resetVector;

    std::cout << colors::YELLOW << "NES system reset. PC set to 0x" << std::hex << resetVector << std::dec
              << colors::RESET << std::endl;
}

void NES::step() {
    // Execute one CPU instruction
    int cpuCycles = cpu.execute(1, cpuMemory, nullptr, true);

    // For each CPU cycle, the PPU runs 3 cycles
    for (int i = 0; i < cpuCycles * 3; i++) {
        ppu.step();

        // Check if PPU triggered an NMI
        if (ppu.isFrameComplete()) {
            // Save frame to PPM file for visual inspection
            static int frameCount = 0;
            std::string filename = "frame_" + std::to_string(frameCount++) + ".ppm";
            ppu.saveFrameBufferToPPM(filename);
            ppu.clearFrameComplete();
        }

        if (ppu.isNMIAsserted()) {
            handleNMI();
            ppu.clearNMI();
        }
    }

    // Run APU for the same number of CPU cycles
    apu.step(cpuCycles);

    // Check if APU triggered an IRQ
    if (apu.irqAsserted()) {
        handleIRQ();
        apu.clearIRQ();
    }

    // Update controller state
    controllers[0].update();
    controllers[1].update();
}

void NES::run() {
    running = true;

    std::cout << colors::GREEN << "Starting NES emulation..." << colors::RESET << std::endl;

    while (running) {
        step();

        // Check if we should stop after a certain number of frames
        static int totalFrameCount = 0;
        if (ppu.isFrameComplete()) {
            totalFrameCount++;
            if (totalFrameCount > 10) {  // Stop after 10 frames
                running = false;
                std::cout << colors::YELLOW << "Finished 10 frames, stopping emulation." << colors::RESET << std::endl;
            }
        }
    }
}

void NES::stop() {
    running = false;
    std::cout << colors::YELLOW << "NES emulation stopped." << colors::RESET << std::endl;
}

void NES::handleNMI() {
    // Push PC and status to stack
    cpuMemory[0x0100 + cpu.SP] = (cpu.PC >> 8) & 0xFF;  // Push high byte
    cpu.SP--;
    cpuMemory[0x0100 + cpu.SP] = cpu.PC & 0xFF;  // Push low byte
    cpu.SP--;

    // Push status register with B flag cleared
    cpuMemory[0x0100 + cpu.SP] = cpu.FLAGS & ~0x10;  // Clear B flag (bit 4)
    cpu.SP--;

    // Set interrupt disable flag
    cpu.FLAGS_I = 1;

    // Load PC from NMI vector
    word nmiVector = mapper.read(0xFFFA) | (mapper.read(0xFFFB) << 8);
    cpu.PC = nmiVector;
}

void NES::handleIRQ() {
    // Only process IRQ if interrupt disable flag is not set
    if (!cpu.FLAGS_I) {
        // Push PC and status to stack
        cpuMemory[0x0100 + cpu.SP] = (cpu.PC >> 8) & 0xFF;  // Push high byte
        cpu.SP--;
        cpuMemory[0x0100 + cpu.SP] = cpu.PC & 0xFF;  // Push low byte
        cpu.SP--;

        // Push status register with B flag cleared
        cpuMemory[0x0100 + cpu.SP] = cpu.FLAGS & ~0x10;  // Clear B flag (bit 4)
        cpu.SP--;

        // Set interrupt disable flag
        cpu.FLAGS_I = 1;

        // Load PC from IRQ vector
        word irqVector = mapper.read(0xFFFE) | (mapper.read(0xFFFF) << 8);
        cpu.PC = irqVector;
    }
}

// Memory handlers

byte NES::readRAM(u32 address) {
    // RAM is mirrored every 2KB (0x0800)
    return cpuMemory[address & 0x07FF];
}

// Add logging for memory reads

void NES::writeRAM(u32 address, byte value) {
    // RAM is mirrored every 2KB (0x0800)
    cpuMemory[address & 0x07FF] = value;
}

byte NES::readPPU(u32 address) {
    // PPU registers are mirrored every 8 bytes (0x0008)
    return ppu.cpuRead(address & 0x2007);
}

void NES::writePPU(u32 address, byte value) {
    // PPU registers are mirrored every 8 bytes (0x0008)
    ppu.cpuWrite(address & 0x2007, value);
}

byte NES::readAPU(u32 address) {
    // Special case for controller ports
    if (address == 0x4016) {
        return controllers[0].read();
    } else if (address == 0x4017) {
        return controllers[1].read();
    }

    // APU registers
    return apu.read(address);
}

void NES::writeAPU(u32 address, byte value) {
    // Special case for controller ports
    if (address == 0x4016) {
        controllers[0].write(value);
        controllers[1].write(value);
        return;
    }

    // APU registers
    apu.write(address, value);
}

byte NES::readCartridge(u32 address) {
    if (cartridge) {
        return cartridge->cpuRead(address);
    }
    return 0;
}

void NES::writeCartridge(u32 address, byte value) {
    if (cartridge) {
        cartridge->cpuWrite(address, value);
    }
}
