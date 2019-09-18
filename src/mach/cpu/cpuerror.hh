#ifndef CPUERROR_HH
#define CPUERROR_HH

#include <cstdint>
#include <exception>

class OpcodeError : public std::exception
{
public:
    OpcodeError(uint16_t addr, uint8_t val) : addr_(addr), val_(val) {}
    uint16_t address() { return addr_; }
    uint8_t value() { return val_; }
private:
    uint16_t addr_;
    uint8_t val_;
};

#endif // CPUERROR_HH
