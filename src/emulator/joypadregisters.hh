#pragma once

#include "addresses.hh"
#include "imemorybusnode.hh"

#include <array>
#include <cstdint>

class JoypadRegisters : public iMemoryBusNode
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
    virtual ~JoypadRegisters() override;

    virtual bool owns(memaddr_t address) override;
    virtual uint8_t* get(memaddr_t address) override;
    virtual uint8_t read(memaddr_t address) override;
    virtual void write(memaddr_t address, uint8_t value) override;

protected:
    uint8_t JOYP;
};
