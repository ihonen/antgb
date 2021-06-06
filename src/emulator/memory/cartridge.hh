#pragma once

#include "emulator/types.hh"

#include <array>

class Cartridge
{
public:
    Cartridge();
    Cartridge(const std::string& filepath);

    ~Cartridge() = default;

    uint8_t* get(uint16_t address);
    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t value);
private:
    std::array<uint8_t, 0x4000> rom0_;
    std::array<uint8_t, 0x4000> rom1_;
    std::array<uint8_t, 0x2000> eram_;
};
