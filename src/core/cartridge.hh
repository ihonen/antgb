#pragma once

#include "types.hh"

#include <cstdint>
#include <string>

namespace antgb
{
class Cartridge
{
public:
    Cartridge(void* rom);
    Cartridge(const std::string& filepath);

    uint8_t read(addr_t address);
    void    write(addr_t address, uint8_t value);

    // TODO: Get rid of.
    uint8_t data[0x8000];

private:
    uint8_t rom0[0x4000];
    uint8_t rom1[0x4000];
    uint8_t eram[0x2000];
};

} // namespace antgb
