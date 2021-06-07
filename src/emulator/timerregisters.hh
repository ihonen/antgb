#pragma once

#include "addresses.hh"

class TimerRegisters
{
public:
    enum BitPos
    {
        InputClockSelect0 = 0,
        InputclockSelect1 = 1,
        TimerEnable = 2
    };

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
