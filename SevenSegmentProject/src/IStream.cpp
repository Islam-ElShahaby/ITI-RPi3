#include "IStream.hpp"
#include <iostream>
#include <limits>

IStream::IStream() {
    std::cout << "IStream Constructed" << std::endl;
}

IStream::~IStream() {
    std::cout << "IStream Destructed" << std::endl;
}

void IStream::cin(int& outVar) {
    std::cout << "[Input] Enter a digit (0-9): ";
    int temp;
    if (std::cin >> temp) {
        if (temp >= 0 && temp <= 9) {
            outVar = temp;
            return;
        }
    }
    // Handle Error
    std::cout << "Invalid Input! Defaulting to -1." << std::endl;
    outVar = -1; 
    
    // Clear buffer
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void IStream::cin(std::string& outVar) {
    std::cout << "[Input] Enter text: ";
    std::cin >> outVar;
}