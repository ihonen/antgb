#pragma once

#include "emulator/types.hh"

class JoypadRegisters
{
public:
    JoypadRegisters();
    ~JoypadRegisters();

    uint8_t* get(memaddr_t address);
    uint8_t read(memaddr_t address);
    void write(memaddr_t address, uint8_t value);

    const uint8_t& get_JOYP();
    uint8_t read_JOYP();
    void write_JOYP();

    bool read_right_or_A();
    void clear_right_or_A();
    void set_right_or_A();

protected:
    uint8_t JOYP;
};
