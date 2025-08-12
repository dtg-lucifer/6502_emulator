#ifndef SIMPLIFIED_CONTROLLER_H
#define SIMPLIFIED_CONTROLLER_H

#include "../types.h"

// NES controller layout:
// 7  6  5     4      3     2     1     0
// A  B  Select Start  Up    Down  Left  Right

class SimplifiedController {
   private:
    byte buttonState;    // Current state of buttons
    byte shiftRegister;  // Shift register for serial reading
    bool strobe;         // Strobe bit from $4016 write

    // Button bit positions
    static constexpr int A_BIT = 7;
    static constexpr int B_BIT = 6;
    static constexpr int SELECT_BIT = 5;
    static constexpr int START_BIT = 4;
    static constexpr int UP_BIT = 3;
    static constexpr int DOWN_BIT = 2;
    static constexpr int LEFT_BIT = 1;
    static constexpr int RIGHT_BIT = 0;

   public:
    SimplifiedController() : buttonState(0), shiftRegister(0), strobe(false) {}

    // Update controller state (would normally read from keyboard)
    void update() {
        // For testing, simulate random button presses
        static int counter = 0;
        counter++;

        if (counter % 60 == 0) {  // Every ~60 updates
            // Simulate pressing A button
            buttonState |= (1 << A_BIT);
        } else {
            buttonState &= ~(1 << A_BIT);
        }

        if (counter % 120 == 0) {  // Every ~120 updates
            // Simulate pressing B button
            buttonState |= (1 << B_BIT);
        } else {
            buttonState &= ~(1 << B_BIT);
        }
    }

    // CPU interface
    void write(byte data) {
        strobe = data & 0x01;
        if (strobe) {
            shiftRegister = buttonState;
        }
    }

    byte read() {
        byte result = 0;

        // Get the LSB of the shift register
        result = shiftRegister & 0x01;

        // If strobe is clear, shift the register
        if (!strobe) {
            shiftRegister = (shiftRegister >> 1) | 0x80;  // Shift and fill with 1s
        }

        return result;
    }
};

#endif  // SIMPLIFIED_CONTROLLER_H
