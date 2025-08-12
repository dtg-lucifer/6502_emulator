#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <string>
#include <vector>

#include "../types.h"

// iNES file format header
struct iNESHeader {
    char signature[4];  // "NES^Z" (4E 45 53 1A)
    byte prg_rom_size;  // PRG ROM size in 16KB units
    byte chr_rom_size;  // CHR ROM size in 8KB units
    byte flags6;        // Mapper, mirroring, battery, trainer
    byte flags7;        // Mapper, VS/Playchoice, NES 2.0
    byte flags8;        // PRG-RAM size (NES 2.0 format only)
    byte flags9;        // TV system (NES 2.0 format only)
    byte flags10;       // TV system, PRG-RAM presence (NES 2.0)
    byte padding[5];    // Unused padding
};

// NES cartridge with memory mapper support
class Cartridge {
   private:
    // ROM data
    std::vector<byte> prgRom;  // Program ROM
    std::vector<byte> chrRom;  // Character ROM (patterns for PPU)
    std::vector<byte> prgRam;  // Program RAM (battery-backed if supported)

    // Cartridge properties
    int mapperNumber;        // iNES mapper number
    bool verticalMirroring;  // Nametable mirroring type
    bool hasBatteryBacked;   // Save RAM present
    bool hasTrainer;         // 512-byte trainer present before PRG data
    int prgRamSize;          // Size of PRG RAM in bytes

    // Memory mapper state
    int prgBankCount;  // Number of PRG ROM banks
    int chrBankCount;  // Number of CHR ROM banks

    // Helper functions
    void parseHeader(const iNESHeader& header);

    // Memory mapper specific functions
    byte mapCpuRead(u32 address);
    void mapCpuWrite(u32 address, byte value);
    byte mapPpuRead(u32 address);
    void mapPpuWrite(u32 address, byte value);

   public:
    Cartridge();
    ~Cartridge();

    // Load cartridge from file
    bool load(const std::string& path);

    // Save battery-backed RAM
    bool saveBatteryRam(const std::string& path);

    // Memory access
    byte cpuRead(u32 address);
    void cpuWrite(u32 address, byte value);
    byte ppuRead(u32 address);
    void ppuWrite(u32 address, byte value);

    // Cartridge information
    int getMapper() const { return mapperNumber; }
    bool hasBattery() const { return hasBatteryBacked; }
    bool isVerticalMirroring() const { return verticalMirroring; }

    // Reset the cartridge state
    void reset();
};

#endif  // CARTRIDGE_H
