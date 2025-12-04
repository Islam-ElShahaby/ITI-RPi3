#include <iostream>
#include <unistd.h>
#include "Pi_gpio_S.h"

int main() {
    RPi_gpio gpio;
    int led_arr[4] = {18,23,24,25};

    std::cout << "--- GPIO Library Test ---" << std::endl;

    // Test 1: Try setting direction before export (Should catch error)
    std::cout << "Testing Error Checking..." << std::endl;
    if (!gpio.set_direction(led_arr[0], "out")) {
        std::cout << "-> Correctly caught unexported pin." << std::endl;
    }

    // Test 2: Export
    for (int i = 0; i<4 ; i++)
    {
        if (!gpio.export_pin(led_arr[i])) {
            std::cerr << "Critical Error: Failed to export pin." << std::endl;
            return 1;
        }
    }

    // Test 3: Invalid Input
    if (!gpio.set_direction(led_arr[0], "backwards")) {
        std::cout << "-> Correctly caught invalid direction." << std::endl;
    }
    
    // Test 4: Blink
    for (int i = 0; i<4 ; i++)
    {
        gpio.set_direction(led_arr[i], "out");
    }
    std::cout << "Blinking LED..." << std::endl;
    
    int x = 10;
    int i = 0;
    int dir = 0;
    while(x>=0)
    {
        gpio.set_value(led_arr[i], 1);
        sleep(1);
        gpio.set_value(led_arr[i], 0);
        if(dir == 0) { i++; }
        else {i--;}
        if(i==4) { 
            dir = 1; 
            i = 2;
            x--;
        }
        else if(i==-1)
        {
            dir = 0;
            i = 1;
            x--;
        }
    }
    return 0;
}