#include "bootrom.hh"

#include "emulator/common/fileio.hh"

BootRom::BootRom(const std::string& filepath)
{
    std::vector<uint8_t> image;
    read_binary_file(filepath, image);
    assert(image.size() == BOOTROM_SIZE);

    for (addr_t i = BOOTROM_LOW; i <= BOOTROM_HIGH; ++i)
    {
        write(i, image.at(i));
    }
}
