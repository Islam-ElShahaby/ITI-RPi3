#ifndef SEVENSEGMENT_HPP
#define SEVENSEGMENT_HPP

#include "IStream.hpp"
#include "OStream.hpp"
#include "Pi_gpio.hpp"
#include <vector>

// THE DIAMOND : Inherits from both IStream and OStream
class SevenSegment : public IStream, public OStream {
private:
    RPi_gpio gpio;            // Composition: use the hardware driver
    std::vector<int> pins;    // Stores the 7 GPIO pin numbers (A-G)

    // Lookup table for 7-segment patterns (0-9)
    // 1 = ON, 0 = OFF
    const int patterns[10][7] = {
        // a, b, c, d, e, f, g
        {0, 0, 0, 0, 0, 0, 1}, // 0
        {1, 0, 0, 1, 1, 1, 1}, // 1
        {0, 0, 1, 0, 0, 1, 0}, // 2
        {0, 0, 0, 0, 1, 1, 0}, // 3
        {1, 0, 0, 1, 1, 0, 0}, // 4
        {0, 1, 0, 0, 1, 0, 0}, // 5
        {0, 1, 0, 0, 0, 0, 0}, // 6
        {0, 0, 0, 1, 1, 1, 1}, // 7
        {0, 0, 0, 0, 0, 0, 0}, // 8
        {0, 0, 0, 0, 1, 0, 0}  // 9
    };

public:
    // Constructor: Takes a list of 7 GPIO pins
    SevenSegment(const std::vector<int>& segmentPins);
    
    virtual ~SevenSegment();

    // Override the OStream method to use Hardware instead of Terminal
    void cout(int digit) override;
};



#endif // SEVENSEGMENT_HPP