#include "nes/apu.h"

#include <iostream>

#include "colors.h"

APU::APU()
    : pulse1_duty_volume(0),
      pulse1_sweep(0),
      pulse1_timer_low(0),
      pulse1_length_timer_high(0),
      pulse2_duty_volume(0),
      pulse2_sweep(0),
      pulse2_timer_low(0),
      pulse2_length_timer_high(0),
      triangle_linear(0),
      triangle_unused(0),
      triangle_timer_low(0),
      triangle_length_timer_high(0),
      noise_volume(0),
      noise_unused(0),
      noise_period(0),
      noise_length(0),
      dmc_flags_rate(0),
      dmc_direct_load(0),
      dmc_sample_address(0),
      dmc_sample_length(0),
      status_control(0),
      frame_counter(0),
      frameInterrupt(false),
      dmcInterrupt(false),
      frameCounterTick(0),
      sampleRate(44100),
      timePerSample(0),
      timeAcc(0),
      audioDevice(0) {
    // Initialize audio
    try {
        initSDLAudio();
        std::cout << colors::GREEN << "APU initialized with SDL audio." << colors::RESET << std::endl;
    } catch (const std::exception& e) {
        std::cerr << colors::RED << "Failed to initialize APU: " << e.what() << colors::RESET << std::endl;
    }
}

APU::~APU() {
    // Clean up SDL audio
    if (audioDevice > 0) {
        SDL_CloseAudioDevice(audioDevice);
    }
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void APU::initSDLAudio() {
    // Initialize SDL audio subsystem
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL audio initialization failed: " << SDL_GetError() << std::endl;
        return;
    }

    // Set up audio specifications
    SDL_AudioSpec want;
    SDL_memset(&want, 0, sizeof(want));
    want.freq = sampleRate;
    want.format = AUDIO_F32;
    want.channels = 1;
    want.samples = 1024;
    want.callback = nullptr;  // Use queue mode instead of callback

    // Open audio device
    audioDevice = SDL_OpenAudioDevice(nullptr, 0, &want, &audioSpec, 0);
    if (audioDevice == 0) {
        std::cerr << "Failed to open audio device: " << SDL_GetError() << std::endl;
        return;
    }

    // Start audio playback
    SDL_PauseAudioDevice(audioDevice, 0);

    // Calculate time per sample in CPU cycles
    // NES CPU runs at ~1.79MHz, so that's our time base
    timePerSample = 1789773.0f / static_cast<float>(audioSpec.freq);
    audioBuffer.resize(1024, 0.0f);  // Preallocate audio buffer
}

void APU::reset() {
    // Reset APU registers
    pulse1_duty_volume = 0;
    pulse1_sweep = 0;
    pulse1_timer_low = 0;
    pulse1_length_timer_high = 0;
    pulse2_duty_volume = 0;
    pulse2_sweep = 0;
    pulse2_timer_low = 0;
    pulse2_length_timer_high = 0;
    triangle_linear = 0;
    triangle_unused = 0;
    triangle_timer_low = 0;
    triangle_length_timer_high = 0;
    noise_volume = 0;
    noise_unused = 0;
    noise_period = 0;
    noise_length = 0;
    dmc_flags_rate = 0;
    dmc_direct_load = 0;
    dmc_sample_address = 0;
    dmc_sample_length = 0;
    status_control = 0;
    frame_counter = 0;

    // Reset internal state
    frameInterrupt = false;
    dmcInterrupt = false;
    frameCounterTick = 0;
    timeAcc = 0.0f;

    std::cout << colors::YELLOW << "APU reset" << colors::RESET << std::endl;
}

byte APU::read(u32 addr) {
    // Only $4015 is readable
    if (addr == 0x4015) {
        byte result = status_control & 0x1F;  // Status of channels
        if (frameInterrupt) result |= 0x40;
        if (dmcInterrupt) result |= 0x80;
        frameInterrupt = false;  // Reading clears frame interrupt
        return result;
    }

    return 0;
}

void APU::write(u32 addr, byte value) {
    switch (addr) {
        // Pulse 1 channel
        case 0x4000:
            pulse1_duty_volume = value;
            break;
        case 0x4001:
            pulse1_sweep = value;
            break;
        case 0x4002:
            pulse1_timer_low = value;
            break;
        case 0x4003:
            pulse1_length_timer_high = value;
            break;

        // Pulse 2 channel
        case 0x4004:
            pulse2_duty_volume = value;
            break;
        case 0x4005:
            pulse2_sweep = value;
            break;
        case 0x4006:
            pulse2_timer_low = value;
            break;
        case 0x4007:
            pulse2_length_timer_high = value;
            break;

        // Triangle channel
        case 0x4008:
            triangle_linear = value;
            break;
        case 0x400A:
            triangle_timer_low = value;
            break;
        case 0x400B:
            triangle_length_timer_high = value;
            break;

        // Noise channel
        case 0x400C:
            noise_volume = value;
            break;
        case 0x400E:
            noise_period = value;
            break;
        case 0x400F:
            noise_length = value;
            break;

        // DMC channel
        case 0x4010:
            dmc_flags_rate = value;
            dmcInterrupt = dmcInterrupt && (value & 0x80);  // Clear DMC IRQ if bit 7 is clear
            break;
        case 0x4011:
            dmc_direct_load = value;
            break;
        case 0x4012:
            dmc_sample_address = value;
            break;
        case 0x4013:
            dmc_sample_length = value;
            break;

        // Control
        case 0x4015:
            status_control = value;
            // Implement channel enabling/disabling here
            break;

        // Frame counter
        case 0x4017:
            frame_counter = value;
            if (value & 0x40) {
                frameInterrupt = false;  // Clear frame interrupt flag
            }
            break;
    }
}

void APU::step(int cpuCycles) {
    // Accumulate time
    timeAcc += static_cast<float>(cpuCycles);

    // Check if we need to generate a sample
    if (timeAcc >= timePerSample) {
        timeAcc -= timePerSample;

        // Mix all channel outputs
        mixSamples();
    }

    // Frame sequencer logic would go here
    // This would handle envelope updates, length counter clocks, etc.
}

void APU::mixSamples() {
    // In a real implementation, this would calculate the output of each channel
    // and mix them together according to the NES mixing formula.
    // For now, we'll just generate a simple sine wave.

    static float phase = 0.0f;
    const float frequency = 440.0f;  // A4 note
    const float twoPi = 6.28318f;

    // Simple sine wave as placeholder
    float sample = 0.3f * sinf(phase);
    phase += twoPi * frequency / static_cast<float>(sampleRate);
    if (phase > twoPi) {
        phase -= twoPi;
    }

    // In a real implementation, we'd add the outputs from all channels:
    // sample = 0.25f * (pulse1Output() + pulse2Output() + triangleOutput() + noiseOutput() + dmcOutput());

    // Add sample to buffer
    static int bufferPos = 0;
    audioBuffer[bufferPos++] = sample;

    // If buffer is full, send to SDL
    if (bufferPos >= audioBuffer.size()) {
        // Queue audio data
        SDL_QueueAudio(audioDevice, audioBuffer.data(), sizeof(float) * audioBuffer.size());
        bufferPos = 0;
    }
}

bool APU::irqAsserted() {
    return (frameInterrupt && (frame_counter & 0x40) == 0) || (dmcInterrupt && (dmc_flags_rate & 0x80));
}

void APU::clearIRQ() {
    frameInterrupt = false;
    dmcInterrupt = false;
}

// Placeholder channel output functions
float APU::pulse1Output() {
    return 0.0f;
}
float APU::pulse2Output() {
    return 0.0f;
}
float APU::triangleOutput() {
    return 0.0f;
}
float APU::noiseOutput() {
    return 0.0f;
}
float APU::dmcOutput() {
    return 0.0f;
}
