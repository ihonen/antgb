#include "timerregisters.hh"

#include "emulator/memory/addresses.hh"

namespace
{

constexpr uint8_t TAC_MASK = 0b00000111;

inline uint8_t mask(memaddr_t address)
{
    switch (address)
    {
        case TAC_ADDR: return TAC_MASK;
    }
    return 0xFF;
}

} // namespace

TimerRegisters::TimerRegisters()
    : DIV(0x00)
    , TIMA(0x00)
    , TMA(0x00)
    , TAC(0x00)
{
}

TimerRegisters::~TimerRegisters() = default;

uint8_t* TimerRegisters::get(memaddr_t address)
{
    switch (address)
    {
        case DIV_ADDR: return &DIV;
        case TIMA_ADDR: return &TIMA;
        case TMA_ADDR: return &TMA;
        case TAC_ADDR: return &TAC;
    }
    assert(false);
    return nullptr;
}

uint8_t TimerRegisters::read(memaddr_t address)
{
    if (auto* byte = get(address))
    {
        return *byte & mask(address);
    }
    assert(false);
    return 0xFF;
}

void TimerRegisters::write(memaddr_t address, uint8_t value)
{
    if (auto* byte = get(address))
    {
        *byte = value & mask(address);
    }
    assert(false);
}
