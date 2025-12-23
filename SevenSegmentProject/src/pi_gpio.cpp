#include "Pi_gpio.hpp"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <algorithm>
#include <sys/stat.h> // For checking directory existence

RPi_gpio::RPi_gpio() {}

RPi_gpio::~RPi_gpio() {
    std::cout << "Cleaning up: Unexporting all pins..." << std::endl;
    for (int pin : exported_pins) {
        write_to_file("/sys/class/gpio/unexport", std::to_string(pin + PIN_OFFSET));
    }
}

bool RPi_gpio::write_to_file(const std::string& file, const std::string& value) {
    std::ofstream fs(file);
    if (!fs.is_open()) {
        std::cerr << "[Error] Could not write '" << value << "' to " << file << std::endl;
        return false;
    }
    fs << value;
    if (fs.fail()) {
        std::cerr << "[Error] Write failed during operation on " << file << std::endl;
        return false;
    }
    fs.close();
    return true;
}

bool RPi_gpio::is_pin_tracked(int pin) {
    return std::find(exported_pins.begin(), exported_pins.end(), pin) != exported_pins.end();
}

std::string RPi_gpio::get_pin_path(int pin) {
    return "/sys/class/gpio/gpio" + std::to_string(pin + PIN_OFFSET);
}

bool RPi_gpio::directory_exists(const std::string& path) {
    struct stat info;
    if (stat(path.c_str(), &info) != 0) return false;
    return (info.st_mode & S_IFDIR);
}

bool RPi_gpio::export_pin(int pin) {
    if (pin < 0) {
        std::cerr << "[Error] Invalid pin number: " << pin << std::endl;
        return false;
    }

    if (is_pin_tracked(pin)) {
        std::cout << "[Info] Pin " << pin << " is already exported." << std::endl;
        return true;
    }

    std::string export_path = "/sys/class/gpio/export";
    if (!write_to_file(export_path, std::to_string(pin + PIN_OFFSET))) {
        return false;
    }

    // Wait and Verify creation
    usleep(100000); // 100ms
    
    if (!directory_exists(get_pin_path(pin))) {
        std::cerr << "[Error] Export failed: System did not create GPIO folder for pin " << pin << std::endl;
        return false;
    }

    exported_pins.push_back(pin);
    std::cout << "[Success] Exported GPIO " << pin << std::endl;
    return true;
}

bool RPi_gpio::set_direction(int pin, const std::string& dir) {
    if (!is_pin_tracked(pin)) {
        std::cerr << "[Error] Set Direction: Pin " << pin << " not exported." << std::endl;
        return false;
    }

    if (dir != "in" && dir != "out") {
        std::cerr << "[Error] Invalid direction '" << dir << "'. Use 'in' or 'out'." << std::endl;
        return false;
    }

    std::string path = get_pin_path(pin) + "/direction";
    return write_to_file(path, dir);
}

bool RPi_gpio::set_value(int pin, int value) {
    if (!is_pin_tracked(pin)) {
        std::cerr << "[Error] Set Value: Pin " << pin << " not exported." << std::endl;
        return false;
    }
    
    // Sanitize value to strictly 0 or 1
    int safe_val = (value != 0) ? 1 : 0;

    std::string path = get_pin_path(pin) + "/value";
    return write_to_file(path, std::to_string(safe_val));
}

int RPi_gpio::read_value(int pin) {
    if (!is_pin_tracked(pin)) {
        std::cerr << "[Error] Read Value: Pin " << pin << " not exported." << std::endl;
        return -1;
    }

    std::string path = get_pin_path(pin) + "/value";
    std::ifstream fs(path);
    if (!fs.is_open()) {
        std::cerr << "[Error] Could not open " << path << " for reading." << std::endl;
        return -1;
    }
    int value;
    fs >> value;
    fs.close();
    return value;
}