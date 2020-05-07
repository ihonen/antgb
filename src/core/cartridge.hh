#pragma once

#include "types.hh"
#include <string>

class Cartridge
{
public:
    Cartridge();
    uint8_t data[0x8000];
    size_t size;
};
