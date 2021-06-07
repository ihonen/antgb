#include "serialregisters.hh"

namespace
{
    constexpr uint8_t SB_WRITE_MASK = 0b11111111;
    constexpr uint8_t SB_READ_MASK  = 0b11111111;

    constexpr uint8_t SC_WRITE_MASK = 0b10000011;
    constexpr uint8_t SC_READ_MASK  = 0b10000011;

    uint8_t read_mask(memaddr_t address)
    {
        switch (address)
        {
            case SB_ADDR: return SB_READ_MASK;
            case SC_ADDR: return SC_READ_MASK;
        }
        return 0xFF;
    }

    uint8_t write_mask(memaddr_t address)
    {
        switch (address)
        {
            case SB_ADDR: return SB_WRITE_MASK;
            case SC_ADDR: return SC_WRITE_MASK;
        }
        return 0xFF;
    }
}

SerialRegisters::SerialRegisters()
    : SB(0x00)
    , SC(0x00)
{
}

SerialRegisters::~SerialRegisters() = default;

bool SerialRegisters::owns(memaddr_t address)
{
    return address >= SERIAL_LOW && address <= SERIAL_HIGH;
}

uint8_t* SerialRegisters::get(memaddr_t address)
{
    assert(owns(address));
    switch (address)
    {
        case SB_ADDR: return &SB;
        case SC_ADDR: return &SC;
    }
    return nullptr;
}

uint8_t SerialRegisters::read(memaddr_t address)
{
    assert(owns(address));
    if (const auto* byte = get(address))
    {
        return *byte & read_mask(address);
    }
    return 0xFF;
}

void SerialRegisters::write(memaddr_t address, uint8_t value)
{
    assert(owns(address));
    if (auto* byte = get(address))
    {
        *byte = value & write_mask(address);
    }
}
