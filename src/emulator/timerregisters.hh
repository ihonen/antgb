#pragma once

#include "addresses.hh"
#include "imemorybusnode.hh"

class TimerRegisters : public iMemoryBusNode
{
public:
    enum BitPos
    {
        InputClockSelect0 = 0,
        InputclockSelect1 = 1,
        TimerEnable = 2
    };

    TimerRegisters();
    virtual ~TimerRegisters() override;

    virtual bool owns(addr_t address) override;
    virtual uint8_t* get(addr_t address) override;
    virtual uint8_t read(addr_t address) override;
    virtual void write(addr_t address, uint8_t value) override;

protected:
    uint8_t DIV;
    uint8_t TIMA;
    uint8_t TMA;
    uint8_t TAC;
};
