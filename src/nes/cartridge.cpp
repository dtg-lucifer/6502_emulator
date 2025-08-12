#include "nes/cartridge.h"

#include <fstream>
#include <iostream>

#include "colors.h"

Cartridge::Cartridge()
    : mapperNumber(0),
      verticalMirroring(false),
      hasBatteryBacked(false),
      hasTrainer(false),
      prgRamSize(0),
      prgBankCount(0),
      chrBankCount(0) {}

Cartridge::~Cartridge() {}

bool Cartridge::load(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        std::cerr << colors::RED << "Failed to open cartridge file: " << path << colors::RESET << std::endl;
        return false;
    }

    // Read iNES header
    iNESHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(header));

    // Verify iNES format
    if (header.signature[0] != 'N' || header.signature[1] != 'E' || header.signature[2] != 'S' ||
        header.signature[3] != 0x1A) {
        std::cerr << colors::RED << "Invalid iNES format" << colors::RESET << std::endl;
        return false;
    }

    // Parse header information
    parseHeader(header);

    // Skip trainer if present
    if (hasTrainer) {
        file.seekg(512, std::ios::cur);
    }

    // Read PRG ROM data
    prgRom.resize(header.prg_rom_size * 16384);  // 16KB per unit
    file.read(reinterpret_cast<char*>(prgRom.data()), prgRom.size());

    // Read CHR ROM data
    if (header.chr_rom_size > 0) {
        chrRom.resize(header.chr_rom_size * 8192);  // 8KB per unit
        file.read(reinterpret_cast<char*>(chrRom.data()), chrRom.size());
    } else {
        // No CHR ROM means CHR RAM - allocate 8KB
        chrRom.resize(8192, 0);
    }

    // Initialize PRG RAM if needed
    if (prgRamSize > 0) {
        prgRam.resize(prgRamSize, 0);
    }

    std::cout << colors::GREEN << "Loaded cartridge:" << colors::RESET << std::endl;
    std::cout << "  Mapper: " << mapperNumber << std::endl;
    std::cout << "  PRG ROM: " << (prgRom.size() / 1024) << "KB (" << prgBankCount << " banks)" << std::endl;
    std::cout << "  CHR ROM: " << (chrRom.size() / 1024) << "KB (" << chrBankCount << " banks)" << std::endl;
    std::cout << "  Mirroring: " << (verticalMirroring ? "Vertical" : "Horizontal") << std::endl;
    std::cout << "  Battery: " << (hasBatteryBacked ? "Yes" : "No") << std::endl;

    return true;
}

void Cartridge::parseHeader(const iNESHeader& header) {
    // Extract mapper number (bits 4-7 from flags6 + bits 4-7 from flags7)
    mapperNumber = (header.flags6 >> 4) | (header.flags7 & 0xF0);

    // Extract mirroring type (bit 0 from flags6)
    verticalMirroring = (header.flags6 & 0x01) != 0;

    // Check for battery-backed RAM (bit 1 from flags6)
    hasBatteryBacked = (header.flags6 & 0x02) != 0;

    // Check for trainer (bit 2 from flags6)
    hasTrainer = (header.flags6 & 0x04) != 0;

    // Get bank counts
    prgBankCount = header.prg_rom_size;
    chrBankCount = header.chr_rom_size;

    // Determine PRG RAM size
    if (header.flags8 == 0) {
        // For older iNES format, assume 8KB if not specified
        prgRamSize = 8192;
    } else {
        // NES 2.0 format
        prgRamSize = 64 << (header.flags8 & 0x0F);
    }

    // Four-screen VRAM layout overrides mirroring
    if (header.flags6 & 0x08) {
        std::cout << colors::YELLOW << "Four-screen VRAM layout not implemented" << colors::RESET << std::endl;
    }
}

bool Cartridge::saveBatteryRam(const std::string& path) {
    if (!hasBatteryBacked || prgRam.empty()) {
        return false;
    }

    std::ofstream file(path, std::ios::binary);
    if (!file) {
        std::cerr << colors::RED << "Failed to save battery RAM: " << path << colors::RESET << std::endl;
        return false;
    }

    file.write(reinterpret_cast<const char*>(prgRam.data()), prgRam.size());
    return true;
}

void Cartridge::reset() {
    // Reset mapper-specific state if needed
}

// Memory mappings - these will vary based on the mapper
byte Cartridge::cpuRead(u32 address) {
    // Default implementation for mapper 0 (NROM)
    if (address >= 0x8000 && address <= 0xFFFF) {
        // For small ROMs (16KB), mirror the ROM
        if (prgRom.size() == 16384) {
            return prgRom[address & 0x3FFF];
        } else {
            return prgRom[address & 0x7FFF];
        }
    }
    return 0;
}

void Cartridge::cpuWrite(u32 address, byte value) {
    // Default implementation for mapper 0 (NROM)
    // NROM has no writable registers, so this is a no-op for ROM addresses
    if (address >= 0x6000 && address < 0x8000 && !prgRam.empty()) {
        // Write to PRG RAM (battery-backed RAM)
        prgRam[address - 0x6000] = value;
    }
}

byte Cartridge::ppuRead(u32 address) {
    // Handle pattern tables (CHR ROM/RAM)
    if (address < 0x2000) {
        return chrRom[address];
    }
    return 0;
}

void Cartridge::ppuWrite(u32 address, byte value) {
    // If CHR RAM is present (no CHR ROM), allow writing
    if (address < 0x2000 && chrBankCount == 0) {
        chrRom[address] = value;
    }
}
