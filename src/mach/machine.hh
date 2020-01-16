#ifndef GAMEBOY_HH
#define GAMEBOY_HH

#include "cpu.hh"
#include "memory.hh"
#include "mmu.hh"
#include "ppu.hh"
#include <cstdint>

class Machine
{
public:
    Machine();
    ~Machine();

    CPU* cpu;
    MMU* mmu;
    PPU* ppu;
};

#endif // GAMEBOY_HH
