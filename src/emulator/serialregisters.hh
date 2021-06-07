#pragma once

#include "addresses.hh"

class SerialRegisters
{
public:
    SerialRegisters();
    ~SerialRegisters();

    uint8_t* get(memaddr_t address);
    uint8_t read(memaddr_t address);
    void write(memaddr_t address, uint8_t value);

protected:
    uint8_t SB;
    uint8_t SC;
};
