#include "OStream.hpp"
#include <iostream>

OStream::OStream() {
    std::cout << "OStream Constructed" << std::endl;
}

OStream::~OStream() {
    std::cout << "OStream Destructed" << std::endl;
}

void OStream::cout(int digit) {
    // Default behavior
    std::cout << "Terminal Output (Int): " << digit << std::endl;
}

void OStream::cout(const std::string& msg) {
    // Strings always go to terminal in this design
    std::cout << msg << std::endl;
}