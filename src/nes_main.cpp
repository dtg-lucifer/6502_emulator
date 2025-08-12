#include <iostream>
#include <string>

#include "colors.h"
#include "nes/nes.h"

int main(int argc, char* argv[]) {
    std::cout << colors::CYAN << colors::BOLD << "NES Emulator based on 6502 CPU" << colors::RESET << std::endl;
    std::cout << "==============================================" << std::endl;

    // Create a new NES system
    NES nes;

    // Check for a ROM file argument
    if (argc < 2) {
        std::cout << colors::YELLOW << "No ROM file specified. Running in test mode." << colors::RESET << std::endl;

        // You could include a test ROM built into the program here
        // For now, just show instructions
        std::cout << "\nUsage: " << argv[0] << " <rom_file.nes>" << std::endl;
        std::cout << "\nControls:" << std::endl;
        std::cout << "  Arrow Keys: D-pad" << std::endl;
        std::cout << "  Z: B button" << std::endl;
        std::cout << "  X: A button" << std::endl;
        std::cout << "  Enter: Start" << std::endl;
        std::cout << "  Right Shift: Select" << std::endl;
        std::cout << "  Escape: Quit" << std::endl;

        // Run with no cartridge - will generate gradient pattern frame files
        std::cout << "Running in test mode. Frame images will be saved as .ppm files." << std::endl;
        std::cout << "These can be viewed with many image viewers or converted with tools like ImageMagick."
                  << std::endl;
        nes.reset();
        nes.run();
        std::cout << "Test frames saved to current directory (frame_*.ppm)" << std::endl;
        return 0;
    }

    // Try to load the specified ROM file
    std::string romPath = argv[1];

    std::cout << colors::YELLOW << "Loading ROM: " << romPath << colors::RESET << std::endl;

    if (!nes.loadCartridge(romPath)) {
        std::cerr << colors::RED << "Failed to load ROM file: " << romPath << colors::RESET << std::endl;
        return 1;
    }

    // Reset and run the NES
    nes.reset();
    nes.run();

    std::cout << colors::GREEN << "Emulation terminated normally." << colors::RESET << std::endl;
    std::cout << "Frame images saved to current directory (frame_*.ppm)" << std::endl;
    return 0;
}
