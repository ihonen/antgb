#pragma once

#include "emulator/memory/addresses.hh"

#include <array>

template<memaddr_t low_address, memaddr_t high_address>
class MemoryBank
{
protected:
    std::array<uint8_t, high_address - low_address> memory_;

public:

    static constexpr memaddr_t low()
    {
        return low_address;
    }

    static constexpr memaddr_t high()
    {
        return high_address;

    }

    static constexpr size_t size()
    {
        return high_address - low_address + 1;
    }

    uint8_t* get(memaddr_t address)
    {
        if (address >= low() && address <= high())
        {
            return &memory_.at(address - low());
        }
    }

    uint8_t read(memaddr_t address)
    {
        const auto* byte = get(address);
        assert(byte != nullptr);
        return *byte;
    }

    void write(memaddr_t address, uint8_t value)
    {
        auto* byte = get(address);
        assert(byte != nullptr);
        *byte = value;
    }
};
