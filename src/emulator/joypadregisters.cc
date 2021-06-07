#include "joypadregisters.hh"

namespace
{
    constexpr uint8_t JOYP_WRITE_MASK = 0b00110000;
    constexpr uint8_t JOYP_READ_MASK  = 0b00111111;

    inline uint8_t read_mask(memaddr_t address)
    {
        switch (address)
        {
            case JOYP_ADDR: return JOYP_READ_MASK;
        }
        return 0xFF;
    }

    inline uint8_t write_mask(memaddr_t address)
    {
        switch (address)
        {
            case JOYP_ADDR: return JOYP_WRITE_MASK;
        }
        return 0xFF;
    }
}

JoypadRegisters::JoypadRegisters()
    : JOYP(0x00)
{
}

JoypadRegisters::~JoypadRegisters() = default;

uint8_t* JoypadRegisters::get(memaddr_t address)
{
    switch (address)
    {
        case JOYP_ADDR: return &JOYP;
    }
    return nullptr;
}

uint8_t JoypadRegisters::read(memaddr_t address)
{
    if (const auto* byte = get(address))
    {
        return *byte & read_mask(address);
    }
    assert(false);
    return 0xFF;
}

void JoypadRegisters::write(memaddr_t address, uint8_t value)
{
    if (auto* byte = get(address))
    {
        *byte = value & write_mask(address);
    }
    assert(false);
}
