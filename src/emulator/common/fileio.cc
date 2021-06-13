#include "fileio.hh"

#include <cassert>
#include <fstream>
#include <iostream>

namespace
{

    std::ifstream::pos_type file_size(const std::string& filepath)
    {
        std::ifstream in(filepath, std::ifstream::ate | std::ifstream::binary);
        return in.tellg();
    }
}

void read_binary_file(const std::string& filepath, std::vector<uint8_t>& image_out)
{
    auto size = file_size(filepath);

    assert(image_out.size() == 0);
    //image_out.reserve(static_cast<size_t>(size));

    std::ifstream file_in(filepath, std::ios::binary);
    if (file_in.is_open())
    {
        image_out = std::vector<uint8_t>(std::istreambuf_iterator<char>(file_in), {});
    }
}
