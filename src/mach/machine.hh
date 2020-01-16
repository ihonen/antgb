#ifndef GAMEBOY_HH
#define GAMEBOY_HH

#include "cpu.hh"
#include "memory.hh"
#include "mmu.hh"
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
