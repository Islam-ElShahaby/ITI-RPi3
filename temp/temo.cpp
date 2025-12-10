#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <chrono>
#include <ctime>
#include <iomanip>

// Linux Serial Headers
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

struct TemperatureData {
    float celsius;
};

class Uart_Temp_Sensor
{
    private:
        int serialPort;
        std::string devicePath;

        void configureUART(termios &options) 
        {
            // Set Baud Rate to 9600
            cfsetispeed(&options, B9600);
            cfsetospeed(&options, B9600);

            // 8N1 Configuration
            options.c_cflag &= ~PARENB;
            options.c_cflag &= ~CSTOPB;
            options.c_cflag &= ~CSIZE;
            options.c_cflag |= CS8;
            options.c_cflag |= CREAD | CLOCAL;

            // Disable Canonical mode (read line-by-line), Echo, etc.
            options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

            // Input Modes (Disable software flow control)
            options.c_iflag &= ~(IXON | IXOFF | IXANY);
            options.c_iflag &= ~(ICRNL | INLCR); // Don't translate \n to \r

            // Output Modes (Raw output)
            options.c_oflag &= ~OPOST;

            // Apply settings
            tcsetattr(serialPort, TCSANOW, &options);
        }   

    public:
        Uart_Temp_Sensor(std::string portName): devicePath(portName) {
            serialPort = open(portName.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
            struct termios tty;
            if (serialPort == -1) 
            {
                std::cerr << "[Error] Unable to open UART: " << portName << std::endl;
            }
            else if (tcgetattr(serialPort, &tty) != 0) 
            {
            std::cerr << "Error getting serial port attributes." << std::endl;
            close(serialPort); // Close the port if tcgetattr fails
            serialPort = -1; // Invalidate serialPort to prevent further use
            }
            else 
            {
                configureUART(tty);
            }

        }

        ~Uart_Temp_Sensor()
        {
            if (serialPort != -1) 
            {
            close(serialPort);
            std::cout << "[System] UART connection closed." << std::endl;
            }
        }

        // Assumes the MCU sends data like "25.5\n"
        float readTemperature() {
            if (serialPort == -1) return 0.0f;

            char buffer[256];
            int bytesRead = read(serialPort, &buffer, sizeof(buffer));

            if (bytesRead > 0) {
                buffer[bytesRead] = '\0'; // Null-terminate string
                try {
                    // Convert string to float
                    return std::stof(buffer);
                } catch (...) {
                    // Handle parsing errors
                    std::cout << "[Error] Failed to parse temperature value: " << buffer << std::endl;
                    return 0.0f; 
                }
            }
            return 0.0f; // Return 0 if no data available
        }
};

class Logger {
private:
    std::string filename;
    std::ofstream logFile;

    // Helper to get current timestamp as a string
    std::string getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        // Using std::localtime which is not thread-safe. For multithreading, consider alternatives.
        ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
        return ss.str();
    }
public:
    Logger(const std::string& logFilename) : filename(logFilename) {
        logFile.open(filename, std::ios_base::app);
        if (!logFile.is_open()) {
            std::cerr << "[Error] Logger: Could not open log file: " << filename << std::endl;
        } else {
            // If the file is empty, write a CSV header.
            logFile.seekp(0, std::ios::end);
            if (logFile.tellp() == 0) {
                logFile << "Timestamp,Temperature (C)\n";
            }
        }
    }

    // Destructor ensures the file is closed.
    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    // Log the temperature data to the file.
    void logTemperature(const std::shared_ptr<TemperatureData>& data) {
        if (logFile.is_open() && data) {
            logFile << getCurrentTimestamp() << "," << data->celsius << "\n";
            logFile.flush(); // Ensure data is written immediately for live monitoring.
        }
    }
};

class Display {
public:
    void displayTemperature(std::shared_ptr<TemperatureData> data) {
        if (data) {
            std::cout << "[Display] Panel Update: " << data->celsius << " C" << std::endl;
            std::cout << "--------------------------------------" << std::endl;
        }
    }
};

int main() {
    std::unique_ptr<Uart_Temp_Sensor> sensor = std::make_unique<Uart_Temp_Sensor>("/dev/serial0");

    std::shared_ptr<TemperatureData> sharedTemp = std::make_shared<TemperatureData>();

    Logger myLogger("temperature_log.csv");
    Display myDisplay;

    std::cout << "Starting UART Temperature Monitor..." << std::endl;

    while (true) {
        // Read raw temperature from UART
        float currentTemp = sensor->readTemperature();

        // Only update if we got a valid non-zero reading
        if (currentTemp > 0.0) {
            sharedTemp->celsius = currentTemp;

            // Share data with consumers
            myLogger.logTemperature(sharedTemp);
            myDisplay.displayTemperature(sharedTemp);
        } else {
            std::cout << "[System] Waiting for data..." << std::endl;
        }

        // Add a small delay to prevent busy-waiting and reduce CPU usage
        usleep(100000); // 100ms delay
    }

    return 0;
}