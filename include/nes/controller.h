#ifndef CONTROLLER_H
#define CONTROLLER_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../types.h"

// NES controller layout:
// 7  6  5     4      3     2     1     0
// A  B  Select Start  Up    Down  Left  Right

class Controller {
   private:
    byte buttonState;    // Current state of buttons
    byte shiftRegister;  // Shift register for serial reading
    bool strobe;         // Strobe bit from $4016 write
    GLFWwindow* window;  // Window for input handling

    // Button bit positions
    static constexpr int A_BIT = 7;
    static constexpr int B_BIT = 6;
    static constexpr int SELECT_BIT = 5;
    static constexpr int START_BIT = 4;
    static constexpr int UP_BIT = 3;
    static constexpr int DOWN_BIT = 2;
    static constexpr int LEFT_BIT = 1;
    static constexpr int RIGHT_BIT = 0;

    // Key mappings (configurable)
    int keyA = GLFW_KEY_X;
    int keyB = GLFW_KEY_Z;
    int keySelect = GLFW_KEY_RIGHT_SHIFT;
    int keyStart = GLFW_KEY_ENTER;
    int keyUp = GLFW_KEY_UP;
    int keyDown = GLFW_KEY_DOWN;
    int keyLeft = GLFW_KEY_LEFT;
    int keyRight = GLFW_KEY_RIGHT;

   public:
    Controller() : buttonState(0), shiftRegister(0), strobe(false), window(nullptr) {}

    // Connect to window for input
    void connectWindow(GLFWwindow* win) { window = win; }

    // Update controller state based on keyboard input
    void update() {
        if (window) {
            buttonState = 0;

            // Read keyboard state
            if (glfwGetKey(window, keyA) == GLFW_PRESS) buttonState |= (1 << A_BIT);
            if (glfwGetKey(window, keyB) == GLFW_PRESS) buttonState |= (1 << B_BIT);
            if (glfwGetKey(window, keySelect) == GLFW_PRESS) buttonState |= (1 << SELECT_BIT);
            if (glfwGetKey(window, keyStart) == GLFW_PRESS) buttonState |= (1 << START_BIT);
            if (glfwGetKey(window, keyUp) == GLFW_PRESS) buttonState |= (1 << UP_BIT);
            if (glfwGetKey(window, keyDown) == GLFW_PRESS) buttonState |= (1 << DOWN_BIT);
            if (glfwGetKey(window, keyLeft) == GLFW_PRESS) buttonState |= (1 << LEFT_BIT);
            if (glfwGetKey(window, keyRight) == GLFW_PRESS) buttonState |= (1 << RIGHT_BIT);

            // Handle conflicting directions
            if ((buttonState & (1 << UP_BIT)) && (buttonState & (1 << DOWN_BIT))) {
                buttonState &= ~((1 << UP_BIT) | (1 << DOWN_BIT));  // Cancel out
            }
            if ((buttonState & (1 << LEFT_BIT)) && (buttonState & (1 << RIGHT_BIT))) {
                buttonState &= ~((1 << LEFT_BIT) | (1 << RIGHT_BIT));  // Cancel out
            }
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

    // For controller configuration
    void setKeyMapping(int a, int b, int sel, int start, int up, int down, int left, int right) {
        keyA = a;
        keyB = b;
        keySelect = sel;
        keyStart = start;
        keyUp = up;
        keyDown = down;
        keyLeft = left;
        keyRight = right;
    }
};

#endif  // CONTROLLER_H
