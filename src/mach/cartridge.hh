#ifndef CARTRIDGE_HH
#define CARTRIDGE_HH

#include <cstdint>
#include <string>

class Cartridge
{
public:
    Cartridge();
    uint8_t data[0x8000];
    size_t size;
};

#endif // CARTRIDGE_HH
