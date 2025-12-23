#include "SevenSegment.hpp"
#include <iostream>

SevenSegment::SevenSegment(const std::vector<int>& segmentPins) : pins(segmentPins) {
    if (pins.size() != 7) {
        std::cerr << "[Error] SevenSegment requires exactly 7 GPIO pins." << std::endl;
        return;
    }

    std::cout << "Initializing 7-Segment Display Hardware..." << std::endl;

    // Setup each pin using your RPi_gpio class
    for (int pin : pins) {
        // Export the pin
        if (!gpio.export_pin(pin)) {
            std::cerr << "Failed to export GPIO " << pin << std::endl;
        }
        // Set direction to OUT
        if (!gpio.set_direction(pin, "out")) {
            std::cerr << "Failed to set direction for GPIO " << pin << std::endl;
        }
    }
}

SevenSegment::~SevenSegment() {
    std::cout << "SevenSegment shutting down." << std::endl;
}

void SevenSegment::cout(int digit) {
    if (digit < 0 || digit > 9) return;
    
    // Hardware Logic
    for (int i = 0; i < 7; i++) {
        gpio.set_value(pins[i], patterns[digit][i]);
    }
    
    // Optional: Also print to screen to confirm it happened
    std::cout << "[Hardware] Displaying " << digit << std::endl;
}