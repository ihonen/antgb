#include "joypadregisters.hh"

#include "emulator/memory/addresses.hh"

namespace
{

inline constexpr uint8_t mask(memaddr_t address)
{
    switch (address)
    {
        case JOYP_ADDR: return 0b00111111;
    }
    return 0xFF;
}

} // namespace

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
    assert(false);
    return nullptr;
}

uint8_t JoypadRegisters::read(memaddr_t address)
{
    if (auto* byte = get(address))
    {
        return *byte & mask(address);
    }
    assert(false);
    return 0xFF;
}

void JoypadRegisters::write(memaddr_t address, uint8_t value)
{
    if (auto* byte = get(address))
    {
        *byte = value & mask(address);
    }
    assert(false);
}
