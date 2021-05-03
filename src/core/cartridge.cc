#include "cartridge.hh"

#include <cstring>
#include <fstream>

namespace antgb
{
Cartridge::Cartridge(void* rom)
{
    memcpy(rom0, rom, 0x4000);
    memcpy(rom1, (uint8_t*)rom + 0x4000, 0x4000);
    memset(eram, 0x00, 0x2000);
}

Cartridge::Cartridge(const std::string& filepath)
{
    std::ifstream file_in(filepath, std::ios::binary);
    uint8_t       rom[0x8000];

    if (file_in.is_open())
    {
        auto begin = file_in.tellg();
        file_in.seekg(0, std::ios::end);
        auto end = file_in.tellg();
        file_in.seekg(0, std::ios::beg);
        auto size = end - begin;
        if (size != 0x8000)
            throw std::exception();

        file_in.read(reinterpret_cast<char*>(rom), 0x8000);
        file_in.close();
    }
    else
    {
        throw std::exception();
    }

    memcpy(data, rom, 0x8000);
    memcpy(rom0, rom, 0x4000);
    memcpy(rom1, rom + 0x4000, 0x4000);
}

uint8_t Cartridge::read(addr_t address)
{
    if (address <= 0x3FFF)
        return rom0[address];
    else if (address <= 0x7FFF)
        return rom1[address - 0x4000];
    else if (address >= 0xA000 && address <= 0xBFFF)
        return eram[address - 0xA000];
    else
        throw std::exception();
}

void Cartridge::write(addr_t address, uint8_t value)
{
    if (address <= 0x3FFF)
        rom0[address] = value;
    else if (address <= 0x7FFF)
        rom1[address - 0x4000] = value;
    else if (address >= 0xA000 && address <= 0xBFFF)
        eram[address - 0xA000] = value;
    else
        throw std::exception();
}

} // namespace antgb
