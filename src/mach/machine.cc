#include "machine.hh"

Machine::Machine()
{
    mmu = new MMU();
    cpu = new CPU(*mmu);
}

Machine::~Machine()
{
    delete cpu;
    delete mmu;
}
