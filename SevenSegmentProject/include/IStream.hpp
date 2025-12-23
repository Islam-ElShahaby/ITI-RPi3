#ifndef ISTREAM_HPP
#define ISTREAM_HPP

#include "Stream.hpp"

// Inherits VIRTUALLY to prevent duplicate Stream instances in the final object
class IStream : virtual public Stream {
public:
    IStream();
    virtual ~IStream();

    void cin(int& outVar) override;
    void cin(std::string& outVar) override;
};

#endif // ISTREAM_HPP