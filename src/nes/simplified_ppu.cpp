#include "nes/simplified_ppu.h"

#include <fstream>
#include <iostream>

#include "colors.h"
#include "nes/cartridge.h"

SimplifiedPPU::SimplifiedPPU()
    : ppuCtrl(0),
      ppuMask(0),
      ppuStatus(0),
      oamAddr(0),
      oamData(0),
      ppuScroll(0),
      ppuAddr(0),
      ppuData(0),
      ppuInternalAddr(0),
      addrLatch(false),
      scanline(0),
      cycle(0),
      frameComplete(false),
      nmiOccurred(false),
      nmiEnabled(false),
      cartridge(nullptr) {
    // Initialize PPU memory
    vram.resize(2048, 0);                  // 2KB VRAM
    oam.resize(256, 0);                    // 256 bytes OAM
    palette.resize(32, 0);                 // 32 bytes palette RAM
    nametables.resize(2048, 0);            // 2KB for nametables
    frameBuffer.resize(256 * 240 * 4, 0);  // 256x240 RGBA

    std::cout << colors::GREEN << "Simplified PPU initialized." << colors::RESET << std::endl;
}

SimplifiedPPU::~SimplifiedPPU() {
    std::cout << colors::YELLOW << "SimplifiedPPU destroyed." << colors::RESET << std::endl;
}

void SimplifiedPPU::connectCartridge(Cartridge* cart) {
    cartridge = cart;
    std::cout << colors::YELLOW << "Cartridge connected to PPU" << colors::RESET << std::endl;
}

void SimplifiedPPU::reset() {
    // Reset PPU registers
    ppuCtrl = 0;
    ppuMask = 0;
    ppuStatus = 0;
    oamAddr = 0;
    ppuScroll = 0;
    ppuAddr = 0;
    ppuData = 0;
    ppuInternalAddr = 0;
    addrLatch = false;

    // Reset rendering position
    scanline = 0;
    cycle = 0;

    // Reset flags
    frameComplete = false;
    nmiOccurred = false;
    nmiEnabled = false;

    std::cout << colors::YELLOW << "PPU reset" << colors::RESET << std::endl;
}

byte SimplifiedPPU::cpuRead(u32 addr) {
    addr &= 0x07;  // Mirror down to 8 registers

    switch (addr) {
        case 0x0000:  // PPUCTRL - Write only
            return 0;

        case 0x0001:  // PPUMASK - Write only
            return 0;

        case 0x0002:  // PPUSTATUS
        {
            // Reading status resets the address latch
            byte result = (ppuStatus & 0xE0) | (ppuData & 0x1F);
            ppuStatus &= ~0x80;  // Clear vblank flag
            addrLatch = false;
            return result;
        }

        case 0x0003:  // OAMADDR - Write only
            return 0;

        case 0x0004:  // OAMDATA
            return oam[oamAddr];

        case 0x0005:  // PPUSCROLL - Write only
            return 0;

        case 0x0006:  // PPUADDR - Write only
            return 0;

        case 0x0007:  // PPUDATA
        {
            byte result = ppuData;
            ppuData = ppuRead(ppuInternalAddr);

            // Auto-increment the address
            ppuInternalAddr += (ppuCtrl & 0x04) ? 32 : 1;
            return result;
        }

        default:
            return 0;
    }
}

void SimplifiedPPU::cpuWrite(u32 addr, byte value) {
    addr &= 0x07;  // Mirror down to 8 registers

    switch (addr) {
        case 0x0000:  // PPUCTRL
            ppuCtrl = value;
            nmiEnabled = (value & 0x80) != 0;
            // Update internal address for vertical/horizontal scrolling mode
            break;

        case 0x0001:  // PPUMASK
            ppuMask = value;
            break;

        case 0x0002:  // PPUSTATUS - Read only
            break;

        case 0x0003:  // OAMADDR
            oamAddr = value;
            break;

        case 0x0004:  // OAMDATA
            oam[oamAddr] = value;
            oamAddr++;  // Auto-increment
            break;

        case 0x0005:  // PPUSCROLL
            if (!addrLatch) {
                // First write: X scroll
                ppuScroll = value;
            } else {
                // Second write: Y scroll
                // Implementation depends on how we manage scrolling
            }
            addrLatch = !addrLatch;
            break;

        case 0x0006:  // PPUADDR
            if (!addrLatch) {
                // First write: high byte
                ppuInternalAddr = (ppuInternalAddr & 0x00FF) | ((value & 0x3F) << 8);
            } else {
                // Second write: low byte
                ppuInternalAddr = (ppuInternalAddr & 0xFF00) | value;
            }
            addrLatch = !addrLatch;
            break;

        case 0x0007:  // PPUDATA
            ppuWrite(ppuInternalAddr, value);
            // Auto-increment the address
            ppuInternalAddr += (ppuCtrl & 0x04) ? 32 : 1;
            break;
    }
}

byte SimplifiedPPU::ppuRead(u32 addr) {
    addr &= 0x3FFF;  // Mirror down to 14 bits

    if (addr < 0x2000) {
        // Pattern tables (CHR ROM/RAM)
        if (cartridge) {
            return cartridge->ppuRead(addr);
        }
        return 0;
    } else if (addr < 0x3F00) {
        // Nametables
        return nametables[addr & 0x0FFF];  // Mirror down to 4KB
    } else {
        // Palette RAM
        return readPaletteRam(addr);
    }
}

void SimplifiedPPU::ppuWrite(u32 addr, byte value) {
    addr &= 0x3FFF;  // Mirror down to 14 bits

    if (addr < 0x2000) {
        // Pattern tables (CHR ROM/RAM)
        if (cartridge) {
            cartridge->ppuWrite(addr, value);
        }
    } else if (addr < 0x3F00) {
        // Nametables
        nametables[addr & 0x0FFF] = value;  // Mirror down to 4KB
    } else {
        // Palette RAM
        writePaletteRam(addr, value);
    }
}

byte SimplifiedPPU::readPaletteRam(u32 addr) {
    addr &= 0x1F;  // Mirror down to 32 bytes

    // Addresses 0x10, 0x14, 0x18, 0x1C are mirrors of 0x00, 0x04, 0x08, 0x0C
    if (addr >= 0x10 && (addr & 0x03) == 0) {
        addr -= 0x10;
    }

    return palette[addr];
}

void SimplifiedPPU::writePaletteRam(u32 addr, byte value) {
    addr &= 0x1F;  // Mirror down to 32 bytes

    // Addresses 0x10, 0x14, 0x18, 0x1C are mirrors of 0x00, 0x04, 0x08, 0x0C
    if (addr >= 0x10 && (addr & 0x03) == 0) {
        addr -= 0x10;
    }

    palette[addr] = value;
}

void SimplifiedPPU::step() {
    // PPU clock cycle logic
    // This is simplified - a real PPU has complex timing

    // Advance PPU state
    cycle++;
    if (cycle >= 341) {  // 341 PPU cycles per scanline
        cycle = 0;
        scanline++;

        if (scanline == 241) {
            // Start of VBlank
            ppuStatus |= 0x80;  // Set VBlank flag
            if (nmiEnabled) {
                nmiOccurred = true;
            }
        } else if (scanline >= 262) {  // NTSC: 262 scanlines
            // Start of new frame
            scanline = 0;
            frameComplete = true;
            ppuStatus &= ~0x80;  // Clear VBlank flag
        }
    }

    // Render current pixel if in visible area
    if (scanline < 240 && cycle < 256) {
        // Simple placeholder rendering
        // In a real implementation, this would handle background and sprites

        // Example: create a simple pattern
        int index = (scanline * 256 + cycle) * 4;

        // Draw a simple pattern based on position
        byte r = (scanline + cycle) % 256;
        byte g = (scanline * 2) % 256;
        byte b = (cycle * 2) % 256;

        // Store in framebuffer
        frameBuffer[index] = r;        // Red
        frameBuffer[index + 1] = g;    // Green
        frameBuffer[index + 2] = b;    // Blue
        frameBuffer[index + 3] = 255;  // Alpha
    }
}

bool SimplifiedPPU::saveFrameBufferToPPM(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << colors::RED << "Failed to open file for writing: " << filename << colors::RESET << std::endl;
        return false;
    }

    // PPM header
    file << "P6\n";       // PPM magic number
    file << "256 240\n";  // Width and height
    file << "255\n";      // Maximum color value

    // Write RGB data (skip Alpha channel)
    for (size_t i = 0; i < 256 * 240; i++) {
        size_t index = i * 4;
        file.write(reinterpret_cast<char*>(&frameBuffer[index]), 3);  // Write RGB only
    }

    std::cout << colors::GREEN << "Frame saved to " << filename << colors::RESET << std::endl;
    return true;
}
