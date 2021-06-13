#pragma once

#include <cstdint>
#include <string>
#include <vector>

void read_binary_file(const std::string& filepath, std::vector<uint8_t>& image_out);
