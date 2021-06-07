#pragma once

#include "addresses.hh"

#include <array>
#include <cstdint>

class JoypadRegisters
{
public:
    enum BitPos
    {
        Right = 0,
        Left = 1,
        Up = 2,
        Down = 3,
        ButtonA = 0,
        ButtonB = 1,
        Select = 2,
        Start = 3,
        DirectionKeysSelect = 4,
        ButtonKeysSelect = 5
    };

    JoypadRegisters();
    ~JoypadRegisters();

    uint8_t* get(memaddr_t address);
    uint8_t read(memaddr_t address);
    void write(memaddr_t address, uint8_t value);

protected:
    uint8_t JOYP;
};
