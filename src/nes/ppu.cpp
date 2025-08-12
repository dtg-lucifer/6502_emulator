#include "nes/ppu.h"

#include <iostream>
#include <stdexcept>

#include "colors.h"

PPU::PPU()
    : window(nullptr),
      textureId(0),
      shaderProgram(0),
      ppuCtrl(0),
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
      nmiEnabled(false) {
    // Initialize PPU memory
    vram.resize(2048, 0);                  // 2KB VRAM
    oam.resize(256, 0);                    // 256 bytes OAM
    palette.resize(32, 0);                 // 32 bytes palette RAM
    nametables.resize(2048, 0);            // 2KB for nametables
    frameBuffer.resize(256 * 240 * 4, 0);  // 256x240 RGBA

    // Initialize OpenGL
    try {
        initOpenGL();
        std::cout << colors::GREEN << "PPU initialized with OpenGL rendering." << colors::RESET << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << colors::RED << "Failed to initialize PPU: " << e.what() << colors::RESET << std::endl;
    }
}

PPU::~PPU() {
    // Clean up OpenGL resources
    if (window) {
        glDeleteTextures(1, &textureId);
        glDeleteProgram(shaderProgram);
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}

void PPU::initOpenGL() {
    // Initialize GLFW
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // Create window with fixed size (2x NES resolution)
    window = glfwCreateWindow(512, 480, "NES Emulator", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("Failed to initialize GLEW");
    }

    // Create texture for frame buffer
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 240, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    // Create simple shader program
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        layout (location = 1) in vec2 aTexCoord;
        out vec2 TexCoord;
        void main() {
            gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
            TexCoord = aTexCoord;
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        in vec2 TexCoord;
        out vec4 FragColor;
        uniform sampler2D screenTexture;
        void main() {
            FragColor = texture(screenTexture, TexCoord);
        }
    )";

    // Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // Create shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Clean up shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Setup vertex buffer and attributes
    float vertices[] = {
        // positions   // texCoords
        -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f};

    unsigned int indices[] = {
        0, 1, 2,  // first triangle
        0, 2, 3   // second triangle
    };

    // Create and bind VAO
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void PPU::updateTexture() {
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 256, 240, GL_RGBA, GL_UNSIGNED_BYTE, frameBuffer.data());
}

byte PPU::cpuRead(u32 addr) {
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

void PPU::cpuWrite(u32 addr, byte value) {
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

byte PPU::ppuRead(u32 addr) {
    addr &= 0x3FFF;  // Mirror down to 14 bits

    if (addr < 0x2000) {
        // Pattern tables (CHR ROM/RAM)
        return 0;  // Placeholder - would read from cartridge
    } else if (addr < 0x3F00) {
        // Nametables
        return nametables[addr & 0x0FFF];  // Mirror down to 4KB
    } else {
        // Palette RAM
        return readPaletteRam(addr);
    }
}

void PPU::ppuWrite(u32 addr, byte value) {
    addr &= 0x3FFF;  // Mirror down to 14 bits

    if (addr < 0x2000) {
        // Pattern tables (CHR ROM/RAM)
        // Placeholder - would write to cartridge if CHR RAM
    } else if (addr < 0x3F00) {
        // Nametables
        nametables[addr & 0x0FFF] = value;  // Mirror down to 4KB
    } else {
        // Palette RAM
        writePaletteRam(addr, value);
    }
}

byte PPU::readPaletteRam(u32 addr) {
    addr &= 0x1F;  // Mirror down to 32 bytes

    // Addresses 0x10, 0x14, 0x18, 0x1C are mirrors of 0x00, 0x04, 0x08, 0x0C
    if (addr >= 0x10 && (addr & 0x03) == 0) {
        addr -= 0x10;
    }

    return palette[addr];
}

void PPU::writePaletteRam(u32 addr, byte value) {
    addr &= 0x1F;  // Mirror down to 32 bytes

    // Addresses 0x10, 0x14, 0x18, 0x1C are mirrors of 0x00, 0x04, 0x08, 0x0C
    if (addr >= 0x10 && (addr & 0x03) == 0) {
        addr -= 0x10;
    }

    palette[addr] = value;
}

void PPU::connectCartridge(Cartridge* cart) {
    // This function would be implemented to connect the cartridge
    // to allow the PPU to access CHR ROM/RAM
    std::cout << colors::YELLOW << "Cartridge connected to PPU" << colors::RESET << std::endl;
}

void PPU::reset() {
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

void PPU::step() {
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

        // Simple pattern: draw a gradient
        int index = (scanline * 256 + cycle) * 4;

        // Example: create a simple pattern
        byte r = 0;
        byte g = 0;
        byte b = 0;

        // Draw a simple gradient background
        r = scanline;
        g = cycle / 4;
        b = 128;

        // Store in framebuffer
        frameBuffer[index] = r;        // Red
        frameBuffer[index + 1] = g;    // Green
        frameBuffer[index + 2] = b;    // Blue
        frameBuffer[index + 3] = 255;  // Alpha
    }
}

void PPU::render() {
    // Update the texture with the frame data
    updateTexture();

    // Clear the screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Use our shader program
    glUseProgram(shaderProgram);

    // Bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glUniform1i(glGetUniformLocation(shaderProgram, "screenTexture"), 0);

    // Draw quad
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
}
