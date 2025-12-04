#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <unistd.h>
#include <algorithm> // Needed for std::find didn't want to use for loop just lazy

class RPi_gpio {
private:
    // List to keep track of pins we have exported
    std::vector<int> exported_pins; 
    
    // ofset for the pins "special thanks to El-Kashef for this"
    const int PIN_OFFSET = 512;

    // Helper to write to sysfs files
    bool write_to_file(const std::string& file, const std::string& value) {
        std::ofstream fs(file);
        if (!fs.is_open()) {
            std::cerr << "Error: Could not open file " << file << std::endl;
            return false;
        }
        fs << value;
        fs.close();
        return true;
    }

    // Helper to check if we are tracking this pin
    bool is_pin_tracked(int pin) {
        return std::find(exported_pins.begin(), exported_pins.end(), pin) != exported_pins.end();
    }

    // Helper to get the full path for a pin
    std::string get_pin_path(int pin) {
        return "/sys/class/gpio/gpio" + std::to_string(pin + PIN_OFFSET);
    }

public:
    RPi_gpio() {
        // Constructor doesn't need to do anything now
    }

    ~RPi_gpio() {
        std::cout << "Cleaning up: Unexporting all pins..." << std::endl;
        for (int pin : exported_pins) {
            write_to_file("/sys/class/gpio/unexport", std::to_string(pin + PIN_OFFSET));
        }
    }

    // Function to export a GPIO pin
    void export_pin(int pin) {
        // SAFETY CHECK: Only proceed if pin exported
        if (is_pin_tracked(pin)) {
            std::cout << "Warning: Pin " << pin << " is already exported." << std::endl;
            return;
        }

        // Export the pin using the offset
        std::string export_path = "/sys/class/gpio/export";
        if (write_to_file(export_path, std::to_string(pin + PIN_OFFSET))) {
            // Add to our tracking 
            exported_pins.push_back(pin);
            // Wait for the OS to create the folder (crucial) 
            // shit got ugly before this "special thanks to deleted user for this"
            usleep(100000); // 100ms
            std::cout << "Success: Exported GPIO " << pin << std::endl;
        }
    }

    void set_direction(int pin, const std::string& dir) {
        // SAFETY CHECK: Only proceed if pin exported
        if (!is_pin_tracked(pin)) {
            std::cerr << "Error: Cannot set direction. GPIO " << pin << " is not exported!" << std::endl;
            return;
        }

        std::string path = get_pin_path(pin) + "/direction";
        write_to_file(path, dir);
    }

    void set_value(int pin, int value) {
        // SAFETY CHECK: Only proceed if pin is in our vector
        if (!is_pin_tracked(pin)) {
            std::cerr << "Error: Cannot set value. GPIO " << pin << " is not exported!" << std::endl;
            return;
        }

        std::string path = get_pin_path(pin) + "/value";
        write_to_file(path, std::to_string(value));
    }

    int read_value(int pin) {
        // SAFETY CHECK: Only proceed if pin exported
        if (!is_pin_tracked(pin)) {
            std::cerr << "Error: Cannot read value. GPIO " << pin << " is not exported!" << std::endl;
            return -1;
        }

        std::string path = get_pin_path(pin) + "/value";
        std::ifstream fs(path);
        if (!fs.is_open()) {
            std::cerr << "Error: Could not open file " << path << std::endl;
            return -1;
        }
        int value;
        fs >> value;
        fs.close();
        return value;
    }
};

int main() {
    // Create the controller
    RPi_gpio gpio_controller;

    int myLed = 18;
    int genericPin = 23;

    // Try to use a pin WITHOUT exporting it (Should fail)
    std::cout << "--- Test 1: Fail Check ---" << std::endl;
    gpio_controller.set_direction(myLed, "out"); 

    // Export the pins properly
    std::cout << "\n--- Test 2: Exporting ---" << std::endl;
    gpio_controller.export_pin(myLed);
    gpio_controller.export_pin(genericPin);

    // Set Directions
    gpio_controller.set_direction(myLed, "out");
    gpio_controller.set_direction(genericPin, "out");

    // Blink Loop
    std::cout << "\n--- Test 3: Blinking ---" << std::endl;
    for(int i = 0; i < 3; i++) {
        std::cout << "LED ON" << std::endl;
        gpio_controller.set_value(myLed, 1);
        gpio_controller.set_value(genericPin, 0);
        sleep(1);
        
        std::cout << "LED OFF" << std::endl;
        gpio_controller.set_value(myLed, 0);
        gpio_controller.set_value(genericPin, 1);
        sleep(1);
    }

    std::cout << "\nExiting program (Destructor will clean up pins)..." << std::endl;
    return 0;
}