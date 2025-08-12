#ifndef APU_H
#define APU_H

#include <SDL2/SDL.h>

#include <queue>
#include <vector>

#include "../types.h"

// NES APU (Audio Processing Unit) specs:
// - 5 audio channels: 2 pulse waves, triangle wave, noise, and DMC (Delta Modulation Channel)
// - CPU clock divided by 12 for timing

class APU {
   private:
    // APU registers
    // $4000-4003: Pulse 1 channel
    byte pulse1_duty_volume;        // $4000: DDLC VVVV (Duty, Loop, Constant, Volume/Envelope)
    byte pulse1_sweep;              // $4001: EPPP NSSS (Enable, Period, Negate, Shift)
    byte pulse1_timer_low;          // $4002: TTTT TTTT (Timer low 8 bits)
    byte pulse1_length_timer_high;  // $4003: LLLL LTTT (Length counter load, Timer high 3 bits)

    // $4004-4007: Pulse 2 channel
    byte pulse2_duty_volume;
    byte pulse2_sweep;
    byte pulse2_timer_low;
    byte pulse2_length_timer_high;

    // $4008-400B: Triangle channel
    byte triangle_linear;             // $4008: CRRR RRRR (Control, Linear counter reload)
    byte triangle_unused;             // $4009: Unused
    byte triangle_timer_low;          // $400A: TTTT TTTT (Timer low 8 bits)
    byte triangle_length_timer_high;  // $400B: LLLL LTTT (Length counter, Timer high bits)

    // $400C-400F: Noise channel
    byte noise_volume;  // $400C: --LC VVVV (Loop, Constant, Volume/Envelope)
    byte noise_unused;  // $400D: Unused
    byte noise_period;  // $400E: L--- PPPP (Loop noise, Period index)
    byte noise_length;  // $400F: LLLL L--- (Length counter load)

    // $4010-4013: DMC channel
    byte dmc_flags_rate;      // $4010: IL-- RRRR (IRQ enable, Loop, Rate index)
    byte dmc_direct_load;     // $4011: -DDD DDDD (Direct load)
    byte dmc_sample_address;  // $4012: AAAA AAAA (Sample address)
    byte dmc_sample_length;   // $4013: LLLL LLLL (Sample length)

    // $4015: Status/Control
    byte status_control;  // $4015: ---D NT21 (DMC, Noise, Triangle, Pulse2, Pulse1)

    // $4017: Frame counter
    byte frame_counter;  // $4017: MI-- ---- (Mode, IRQ inhibit)

    // Internal state
    bool frameInterrupt;   // Set when frame interrupt occurs
    bool dmcInterrupt;     // Set when DMC interrupt occurs
    int frameCounterTick;  // Current tick of frame counter

    // Audio output
    int sampleRate;                  // Audio sample rate (typically 44100Hz)
    float timePerSample;             // Time per audio sample in CPU cycles
    float timeAcc;                   // Time accumulator for sample generation
    std::vector<float> audioBuffer;  // Buffer for audio samples

    // SDL Audio
    SDL_AudioDeviceID audioDevice;
    SDL_AudioSpec audioSpec;

    // Helper methods
    void initSDLAudio();
    void mixSamples();
    float pulse1Output();
    float pulse2Output();
    float triangleOutput();
    float noiseOutput();
    float dmcOutput();

   public:
    APU();
    ~APU();

    // APU interface
    byte read(u32 addr);
    void write(u32 addr, byte value);

    // APU operations
    void reset();
    void step(int cpuCycles);

    // IRQ status
    bool irqAsserted();
    void clearIRQ();
};

#endif  // APU_H
