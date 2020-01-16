#include "machine.hh"

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
