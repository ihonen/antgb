#pragma once

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
