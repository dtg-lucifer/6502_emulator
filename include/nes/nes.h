#ifndef NES_H
#define NES_H

#include "../cpu.h"
#include "../memory_mapper.h"
#include "apu.h"
#include "cartridge.h"
#include "simplified_controller.h"
#include "simplified_ppu.h"

class NES {
   private:
    Cpu cpu;
    Mem cpuMemory;
    MemoryMapper mapper;
    SimplifiedPPU ppu;
    APU apu;
    SimplifiedController controllers[2];
    Cartridge* cartridge;

    bool running = false;

    // NES memory map regions
    static constexpr u32 RAM_START = 0x0000;
    static constexpr u32 RAM_END = 0x07FF;
    static constexpr u32 RAM_MIRROR_END = 0x1FFF;
    static constexpr u32 PPU_REGISTERS_START = 0x2000;
    static constexpr u32 PPU_REGISTERS_END = 0x2007;
    static constexpr u32 PPU_REGISTERS_MIRROR_END = 0x3FFF;
    static constexpr u32 APU_IO_REGISTERS_START = 0x4000;
    static constexpr u32 APU_IO_REGISTERS_END = 0x4017;
    static constexpr u32 CARTRIDGE_START = 0x4020;
    static constexpr u32 CARTRIDGE_END = 0xFFFF;

    void setupMemoryMap();
    void handleNMI();
    void handleIRQ();

    // Memory handlers
    byte readRAM(u32 address);
    void writeRAM(u32 address, byte value);
    byte readPPU(u32 address);
    void writePPU(u32 address, byte value);
    byte readAPU(u32 address);
    void writeAPU(u32 address, byte value);
    byte readCartridge(u32 address);
    void writeCartridge(u32 address, byte value);

   public:
    NES();
    ~NES();
    bool loadCartridge(const std::string& path);
    void reset();
    void step();
    void run();
    void stop();
    bool isRunning() const { return running; }
};

#endif  // NES_H
