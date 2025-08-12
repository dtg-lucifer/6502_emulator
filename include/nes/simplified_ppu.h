#ifndef SIMPLIFIED_PPU_H
#define SIMPLIFIED_PPU_H

#include <string>
#include <vector>

#include "../types.h"

// Forward declaration for cyclic dependencies
class Cartridge;

// Simplified PPU (Picture Processing Unit) for NES emulation
// This implementation avoids external graphics library dependencies
class SimplifiedPPU {
   private:
    // PPU memory
    std::vector<byte> vram;        // 2KB Video RAM (pattern tables)
    std::vector<byte> oam;         // 256 bytes Object Attribute Memory (sprites)
    std::vector<byte> palette;     // 32 bytes Color palette
    std::vector<byte> nametables;  // 2KB Name tables (background layout)

    // Frame buffer for rendered output
    std::vector<byte> frameBuffer;  // 256x240x4 bytes (RGBA)

    // PPU registers
    byte ppuCtrl;    // $2000: PPU control
    byte ppuMask;    // $2001: PPU mask
    byte ppuStatus;  // $2002: PPU status
    byte oamAddr;    // $2003: OAM address
    byte oamData;    // $2004: OAM data
    byte ppuScroll;  // $2005: PPU scroll
    byte ppuAddr;    // $2006: PPU address
    byte ppuData;    // $2007: PPU data

    // Internal state
    word ppuInternalAddr;  // PPU internal address register
    bool addrLatch;        // Address latch for ppuAddr/ppuScroll (0 = first write, 1 = second write)
    int scanline;          // Current scanline (0-261, 0-239=visible, 240=post, 241-260=vblank)
    int cycle;             // Current dot/pixel in scanline (0-340)
    bool frameComplete;    // Set to true when a frame is complete
    bool nmiOccurred;      // Set when NMI should be triggered
    bool nmiEnabled;       // NMI enable flag from ppuCtrl

    // Cartridge reference
    Cartridge* cartridge;

    // NES palette colors (RGB values)
    const unsigned int nesPalette[64] = {
        0x7C7C7C, 0x0000FC, 0x0000BC, 0x4428BC, 0x940084, 0xA80020, 0xA81000, 0x881400, 0x503000, 0x007800, 0x006800,
        0x005800, 0x004058, 0x000000, 0x000000, 0x000000, 0xBCBCBC, 0x0078F8, 0x0058F8, 0x6844FC, 0xD800CC, 0xE40058,
        0xF83800, 0xE45C10, 0xAC7C00, 0x00B800, 0x00A800, 0x00A844, 0x008888, 0x000000, 0x000000, 0x000000, 0xF8F8F8,
        0x3CBCFC, 0x6888FC, 0x9878F8, 0xF878F8, 0xF85898, 0xF87858, 0xFCA044, 0xF8B800, 0xB8F818, 0x58D854, 0x58F898,
        0x00E8D8, 0x787878, 0x000000, 0x000000, 0xFCFCFC, 0xA4E4FC, 0xB8B8F8, 0xD8B8F8, 0xF8B8F8, 0xF8A4C0, 0xF0D0B0,
        0xFCE0A8, 0xF8D878, 0xD8F878, 0xB8F8B8, 0xB8F8D8, 0x00FCFC, 0xF8D8F8, 0x000000, 0x000000};

    // Helper functions
    void renderScanline();
    void renderSprites();
    void evaluateSprites();
    byte readPaletteRam(u32 addr);
    void writePaletteRam(u32 addr, byte value);

   public:
    SimplifiedPPU();
    ~SimplifiedPPU();

    // PPU interface
    byte cpuRead(u32 addr);               // Read from PPU registers (CPU-facing)
    void cpuWrite(u32 addr, byte value);  // Write to PPU registers (CPU-facing)
    byte ppuRead(u32 addr);               // Read from PPU memory (PPU-internal)
    void ppuWrite(u32 addr, byte value);  // Write to PPU memory (PPU-internal)

    // Connect to cartridge
    void connectCartridge(class Cartridge* cart);

    // PPU operations
    void reset();
    void step();  // Execute one PPU cycle

    // Save frame buffer to PPM file for visual testing
    bool saveFrameBufferToPPM(const std::string& filename);

    // Status functions
    bool isFrameComplete() { return frameComplete; }
    void clearFrameComplete() { frameComplete = false; }
    bool isNMIAsserted() { return nmiOccurred && nmiEnabled; }
    void clearNMI() { nmiOccurred = false; }

    // Window state simulation for simplified version
    bool isWindowClosed() { return false; }  // Always return false since there's no real window
};

#endif  // SIMPLIFIED_PPU_H
