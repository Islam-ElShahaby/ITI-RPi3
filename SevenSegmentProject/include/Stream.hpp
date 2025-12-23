#ifndef STREAM_HPP
#define STREAM_HPP

#include <string>

class Stream {
public:
    
    Stream();   // Default Constructor

    virtual void cin(int& outVar) = 0;
    virtual void cin(std::string& outVar) = 0;

    virtual void cout(int digit) = 0;
    virtual void cout(const std::string& msg) = 0;
    
    // Crucial for polymorphism to work correctly in the diamond pattern
    virtual ~Stream();  // Virtual Destructor
};

// Output Operator (<<) for Integers
static Stream& operator<<(Stream& s, int val) {
    s.cout(val);
    return s;
}
// Output Operator (<<) for Strings
static Stream& operator<<(Stream& s, const std::string& val) {
    s.cout(val);
    return s;
}
// Input Operator (>>) for Integers
static Stream& operator>>(Stream& s, int& val) {
    s.cin(val);
    return s;
}
// Input Operator (>>) for Strings
static Stream& operator>>(Stream& s, std::string& val) {
    s.cin(val);
    return s;
}

#endif // STREAM_HPP