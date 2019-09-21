#ifndef GAMEBOY_HH
#define GAMEBOY_HH

#include <cstdint>

class GBMachine
{
public:
    GBMachine();
    class CPU;
    class MMU;
    class Peripheral;
    class PPU;
private:
    static const uint16_t PPU_IO_BASE_ADDR = 0xFF40;
    CPU* cpu;
    MMU* mmu;
    PPU* lcd;
};

#endif // GAMEBOY_HH
