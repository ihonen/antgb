#include "machine.hh"

#include <cstring>

Machine::Machine()
{
    mmu = new MMU();
    cpu = new CPU(*mmu);
    ppu = new PPU(*mmu);
}

Machine::~Machine()
{
    delete cpu;
    delete mmu;
    delete ppu;
}

void Machine::load_rom(void* rom, size_t size)
{
    memcpy(mmu->mem.data, rom, size);
    cpu->restart();
}

void Machine::tick()
{
    cpu->execute();
}
