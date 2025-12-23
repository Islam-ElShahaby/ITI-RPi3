#ifndef OSTREAM_HPP
#define OSTREAM_HPP

#include "Stream.hpp"

// Inherits VIRTUALLY to share the single Stream base instance
class OStream : virtual public Stream {
public:
    OStream();
    virtual ~OStream();

    void cout(int digit) override;
    void cout(const std::string& msg) override;
};

#endif // OSTREAM_HPP