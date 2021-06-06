#include "serialregisters.hh"

#include "emulator/memory/addresses.hh"

namespace
{

constexpr uint8_t SC_MASK = 0b10000011;

inline uint8_t mask(memaddr_t address)
{
    switch (address)
    {
        case SC_ADDR: return SC_MASK;
    }
    return 0xFF;
}

} // namespace

SerialRegisters::SerialRegisters()
    : SB(0x00)
    , SC(0x00)
{
}

SerialRegisters::~SerialRegisters() = default;

uint8_t* SerialRegisters::get(memaddr_t address)
{
    switch (address)
    {
        case SB_ADDR: return &SB;
        case SC_ADDR: return &SC;
    }
    assert(false);
    return nullptr;
}

uint8_t SerialRegisters::read(memaddr_t address)
{
    if (auto* byte = get(address))
    {
        return *byte & mask(address);
    }
    assert(false);
    return 0xFF;
}

void SerialRegisters::write(memaddr_t address, uint8_t value)
{
    if (auto* byte = get(address))
    {
        *byte = value & mask(address);
    }
    assert(false);
}
