#include "joypadregisters.hh"

namespace
{
    constexpr uint8_t JOYP_WRITE_MASK = 0b00110000;
    constexpr uint8_t JOYP_READ_MASK  = 0b00111111;

    inline uint8_t read_mask(addr_t address)
    {
        switch (address)
        {
            case JOYP_ADDR: return JOYP_READ_MASK;
        }
        return 0xFF;
    }

    inline uint8_t write_mask(addr_t address)
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

bool JoypadRegisters::owns(addr_t address)
{
    return address == JOYP_ADDR;
}

uint8_t* JoypadRegisters::get(addr_t address)
{
    assert(owns(address));
    switch (address)
    {
        case JOYP_ADDR: return &JOYP;
    }
    return nullptr;
}

uint8_t JoypadRegisters::read(addr_t address)
{
    assert(owns(address));
    if (const auto* byte = get(address))
    {
        return *byte & read_mask(address);
    }
    return 0xFF;
}

void JoypadRegisters::write(addr_t address, uint8_t value)
{
    assert(owns(address));
    if (auto* byte = get(address))
    {
        *byte = value & write_mask(address);
    }
}
