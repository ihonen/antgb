#ifndef GAMEBOY_HH
#define GAMEBOY_HH

#include "cpu/cpu.hh"
#include "mmu/memory.hh"
#include "mmu/mmu.hh"
#include <cstdint>

class Machine
{
public:
    Machine();
    ~Machine();
private:
    CPU* cpu;
    MMU* mmu;
};

#endif // GAMEBOY_HH
