#pragma once

#include "emulator/types.hh"

class TimerRegisters
{
public:
    TimerRegisters();
    ~TimerRegisters();

    uint8_t* get(memaddr_t address);
    uint8_t read(memaddr_t address);
    void write(memaddr_t address, uint8_t value);

protected:
    uint8_t DIV;
    uint8_t TIMA;
    uint8_t TMA;
    uint8_t TAC;
};
