#include "Stream.hpp"
#include <iostream>

Stream::Stream() {
    std::cout << "Stream Constructed" << std::endl;
}

Stream::~Stream() {
    std::cout << "Stream Destructed" << std::endl;
}