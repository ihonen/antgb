#ifndef MEMORY_HH
#define MEMORY_HH

#include <cstdint>
#include <cstring>

typedef uint16_t memaddr_t;

class Memory
{
public:
    Memory()
    {
        memset(data, 0x00, SIZE);
    }

    uint8_t& operator[](uint16_t address)
    {
        return data[address];
    }

    static const size_t SIZE = 0x10000;
    static const memaddr_t BOOT_ROM_LOW = 0x0000;
    static const memaddr_t BOOT_ROM_HIGH = 0x00FF;
    static const memaddr_t ROM_BANK0_LOW = 0x0000;
    static const memaddr_t ROM_BANK0_HIGH = 0x3FFF;
    static const memaddr_t ROM_BANK_SWITCHABLE_LOW = 0x4000;
    static const memaddr_t ROM_BANK_SWITCHABLE_HIGH = 0x7FFF;
    static const memaddr_t TILE_RAM_LOW = 0x8000;
    static const memaddr_t TILE_RAM_HIGH = 0x97FF;
    static const memaddr_t BACKGROUND_MAP_LOW = 0x9800;
    static const memaddr_t BACKGROUND_MAP_HIGH = 0x9FFF;
    static const memaddr_t CARTRIDGE_LOW = 0x9800;
    static const memaddr_t CARTRIDGE_HIGH = 0xBFFF;
    static const memaddr_t WORKING_RAM_LOW = 0xC000;
    static const memaddr_t WORKING_RAM_HIGH = 0xDFFF;
    static const memaddr_t ECHO_RAM_LOW = 0xE000;
    static const memaddr_t ECHO_RAM_HIGH = 0xFDFF;
    static const memaddr_t OAM_LOW = 0xFE00;
    static const memaddr_t OAM_HIGH = 0xFE9F;
    static const memaddr_t UNUSED_LOW = 0xFEA0;
    static const memaddr_t UNUSED_HIGH = 0xFEFF;
    static const memaddr_t IO_LOW = 0xFF00;
    static const memaddr_t IO_HIGH = 0xFF7F;
    static const memaddr_t HIGH_RAM_LOW = 0xFF80;
    static const memaddr_t HIGH_RAM_HIGH = 0xFFE;
    static const memaddr_t INTERRUPT_ENABLE_FLAG = 0xFFFF;
    uint8_t data[SIZE];
};

#endif // MEMORY_HH
