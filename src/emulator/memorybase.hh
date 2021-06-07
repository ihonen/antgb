#pragma once

#include "addresses.hh"

#include <array>

template<memaddr_t low_address, memaddr_t high_address>
class MemoryBase
{
public:

    MemoryBase()
        : memory_({0x00})
    {
    }

    virtual ~MemoryBase() = default;

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

    virtual uint8_t* get(memaddr_t address)
    {
        assert(address >= low() && address <= high());
        if (address >= low() && address <= high())
        {
            return &memory_.at(address - low());
        }
    }

    virtual uint8_t read(memaddr_t address)
    {
        assert(address >= low() && address <= high());
        const auto* byte = get(address);
        assert(byte != nullptr);
        return *byte;
    }

    virtual void write(memaddr_t address, uint8_t value)
    {
        assert(address >= low() && address <= high());
        auto* byte = get(address);
        assert(byte != nullptr);
        *byte = value;
    }
protected:
    std::array<uint8_t, size()> memory_;
};
