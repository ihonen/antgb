#include "cartridge.hh"

#include "addresses.hh"

#include <algorithm>
#include <fstream>
#include <vector>

Cartridge::Cartridge()
    : iMemoryBusNode()
    , is_inserted_(false)
{
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

    for (size_t address = ROM0_LOW; address <= ROM0_HIGH; ++address)
    {
        rom0_.write(address, image.at(address));
    }

    for (size_t address = ROM1_LOW; address <= ROM1_HIGH; ++address)
    {
        rom1_.write(address, image.at(address));
    }
}

bool Cartridge::is_inserted()
{
    return is_inserted_;
}

void Cartridge::set_inserted(bool inserted)
{
    is_inserted_ = inserted;
}

bool Cartridge::owns(addr_t address)
{
    return rom0_.owns(address) || rom1_.owns(address) || eram_.owns(address);
}

uint8_t* Cartridge::get(addr_t address)
{
    assert(owns(address));
    if (rom0_.owns(address))
        return rom0_.get(address);
    if (rom1_.owns(address))
        return rom1_.get(address);
    if (eram_.owns(address))
        return eram_.get(address);
    return nullptr;
}

uint8_t Cartridge::read(addr_t address)
{
    assert(owns(address));
    if (rom0_.owns(address))
        return rom0_.read(address);
    if (rom1_.owns(address))
        return rom1_.read(address);
    if (eram_.owns(address))
        return eram_.read(address);

    return 0xFF;
}

void Cartridge::write(addr_t address, uint8_t value)
{
    assert(owns(address));
    if (rom0_.owns(address))
        rom0_.write(address, value);
    if (rom1_.owns(address))
        rom1_.write(address, value);
    if (eram_.owns(address))
        eram_.write(address, value);
}
