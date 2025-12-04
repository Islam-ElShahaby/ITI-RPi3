#ifndef RPI_GPIO_H
#define RPI_GPIO_H

#include <string>
#include <vector>

class RPi_gpio {
private:
    std::vector<int> exported_pins; 
    const int PIN_OFFSET = 512; // Change to 0 for older Pi's, 512 for Pi 5/newer kernels

    bool write_to_file(const std::string& file, const std::string& value);
    bool is_pin_tracked(int pin);
    std::string get_pin_path(int pin);
    bool directory_exists(const std::string& path);

public:
    RPi_gpio();
    ~RPi_gpio();

    // Returns true on success, false on failure
    bool export_pin(int pin);
    bool set_direction(int pin, const std::string& dir);
    bool set_value(int pin, int value);
    
    // Returns value (0/1) or -1 on error
    int read_value(int pin);
};

#endif