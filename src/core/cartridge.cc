#include "cartridge.hh"

#include <cstring>

Cartridge::Cartridge()
{
    memset(data, 0x00, 0x8000);
    size = 0;
}
