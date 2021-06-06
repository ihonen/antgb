#include "cartridge.hh"

#include <algorithm>
#include <fstream>
#include <vector>

Cartridge::Cartridge()
{
    rom0_ = {};
    rom1_ = {};
    eram_ = {};
}

Cartridge::Cartridge(const std::string& filepath)
{
    std::vector<uint8_t> image;

    std::ifstream file_in(filepath, std::ios::binary);
    if (file_in.is_open())
    {
        image = std::vector<uint8_t>(std::istreambuf_iterator<char>(file_in), {});
    }

    if (image.size() != 0x8000)
    {
        std::cerr << "WARNING: Only ROM size 0x8000 is supported." << std::endl;
    }

    for (size_t i = 0x0000; i <= 0x3FFF; ++i)
    {
        rom0_.at(i) = image.at(i);
    }

    for (size_t i = 0x4000; i <= 0x7FFF; ++i)
    {
        rom1_.at(i - 0x4000) = image.at(i);
    }

    eram_ = {0x00};
}

uint8_t* Cartridge::get(uint16_t address)
{
    if (address <= 0x3FFF)
    {
        return &rom0_.at(address - 0x0000);
    }
    else if (address >= 0x4000 && address <= 0x7FFF)
    {
        return &rom1_.at(address - 0x4000);
    }
    else if (address >= 0xA000 && address <= 0xBFFF)
    {
        return &eram_.at(address - 0xA000);
    }

    return nullptr;
}

uint8_t Cartridge::read(uint16_t address)
{
    if (uint8_t* byte = get(address))
    {
        return *byte;
    }

    return 0xFF;
}

void Cartridge::write(uint16_t address, uint8_t value)
{
    if (uint8_t* byte = get(address))
    {
        *byte = value;
    }
}
