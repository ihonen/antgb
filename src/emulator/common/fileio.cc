#include "fileio.hh"

#include <cstring>
#include <fstream>
#include <vector>
#include <iostream>

// TODO: Get rid of.
void load_rom(std::string& filepath, uint8_t* memory)
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

    memcpy(memory, image.data(), image.size());
}
