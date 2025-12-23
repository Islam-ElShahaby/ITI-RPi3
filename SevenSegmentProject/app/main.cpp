#include "SevenSegment.hpp"
#include <vector>
#include <string>

int main() {
    // 1. Setup
    std::vector<int> pins = {17, 27, 22, 23, 24, 25, 29}; 
    
    // Create the pointer
    Stream* displayPtr = new SevenSegment(pins);

    // 2. Create the Alias for Syntax Sugar
    Stream& io = *displayPtr; 

    int number;
    std::string text;

    // 3. Run the loop
    while(true) {
        io << "Enter a number (0-9) to display, or -1 to quit: ";
        io >> number;

        if (number == -1) break;

        // This goes to Hardware because it's an INT
        io << number; 

        io << "Enter a message to print to terminal: ";
        io >> text;
        
        // This goes to Terminal because it's a STRING
        io << "You said: " << text;
    }

    delete displayPtr;
    return 0;
}