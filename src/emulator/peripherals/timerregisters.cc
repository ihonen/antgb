#include "timerregisters.hh"

namespace
{
    constexpr uint8_t DIV_WRITE_MASK  = 0b00000000;
    constexpr uint8_t DIV_READ_MASK   = 0b11111111;

    constexpr uint8_t TIMA_WRITE_MASK = 0b11111111;
    constexpr uint8_t TIMA_READ_MASK  = 0b11111111;

    constexpr uint8_t TMA_WRITE_MASK  = 0b11111111;
    constexpr uint8_t TMA_READ_MASK   = 0b11111111;

    constexpr uint8_t TAC_WRITE_MASK  = 0b00000111;
    constexpr uint8_t TAC_READ_MASK   = 0b00000111;

    uint8_t read_mask(addr_t address)
    {
        switch (address)
        {
            case DIV_ADDR:  return DIV_READ_MASK;
            case TIMA_ADDR: return TIMA_READ_MASK;
            case TMA_ADDR:  return TMA_READ_MASK;
            case TAC_ADDR:  return TAC_READ_MASK;
        }
        return 0xFF;
    }

    uint8_t write_mask(addr_t address)
    {
        switch (address)
        {
            case DIV_ADDR:  return DIV_WRITE_MASK;
            case TIMA_ADDR: return TIMA_WRITE_MASK;
            case TMA_ADDR:  return TMA_WRITE_MASK;
            case TAC_ADDR:  return TAC_WRITE_MASK;
        }
        return 0xFF;
    }
}

TimerRegisters::TimerRegisters()
    : DIV(0x00)
    , TIMA(0x00)
    , TMA(0x00)
    , TAC(0x00)
{
}

TimerRegisters::~TimerRegisters() = default;

bool TimerRegisters::owns(addr_t address)
{
    return address >= TIMER_LOW && address <= TIMER_HIGH;
}

uint8_t* TimerRegisters::get(addr_t address)
{
    assert(owns(address));
    switch (address)
    {
        case DIV_ADDR:  return &DIV;
        case TIMA_ADDR: return &TIMA;
        case TMA_ADDR:  return &TMA;
        case TAC_ADDR:  return &TAC;
    }
    return nullptr;
}

uint8_t TimerRegisters::read(addr_t address)
{
    assert(owns(address));
    if (const auto* byte = get(address))
    {
        return *byte & read_mask(address);
    }
    return 0xFF;
}

void TimerRegisters::write(addr_t address, uint8_t value)
{
    assert(owns(address));
    if (auto* byte = get(address))
    {
        *byte = value & write_mask(address);
    }
}
