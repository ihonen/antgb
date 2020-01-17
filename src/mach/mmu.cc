#include "mmu.hh"

MMU::MMU()
{
    mem = Memory();
}

uint8_t MMU::read(memaddr_t address)
{
    if (!can_read(address)) return 0xFF;
    return mem[address];
}

bool MMU::write(memaddr_t address, uint8_t value)
{
    if (!can_write(address)) return false;
    mem[address] = value;
    return true;
}

bool MMU::can_read(memaddr_t address)
{
    if ((address >= 0xFEA0 && address < 0xFF00)
        || (address >= 0xFF4C && address < 0xFF80))
    {
        return false;
    }
    return true;
}

bool MMU::can_write(memaddr_t address)
{
    if (!can_read(address)) return false;
    return true;
}
