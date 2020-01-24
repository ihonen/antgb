#include "mmu.hh"

#include <iostream>

using namespace std;

MMU::MMU()
{
    mem = Memory();
    bootrom = BootROM();
    is_bootrom_enabled = true;
    clear_dma_status();
}

uint8_t MMU::read(memaddr_t address)
{
    /*
    if (!can_read(address)) return 0xFF;
    */

    if (address <= 0x0133 && is_bootrom_enabled)
    {
        return bootrom.data[address];
    }

    return mem[address];
}

bool MMU::write(memaddr_t address, uint8_t value)
{   /*
    if (!can_write(address)) return false;
    */

    if (address <= 0x00FF && is_bootrom_enabled)
    {
        return false;
    }

    // Disable boot ROM on first write to 0xFF50.
    if (address == 0xFF50)
    {
        is_bootrom_enabled = false;
    }

    mem[address] = value;

    return true;
}

/*
bool MMU::can_read(memaddr_t address)
{
    if ((address >= 0xFEA0 && address < 0xFF00)
        || (address >= 0xFF4C && address < 0xFF80))
    {
        return false;
    }
    else if (is_in_locked_region(address))
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
*/

void MMU::launch_oam_dma(memaddr_t destination, memaddr_t source, memaddr_t size)
{
    dma.unemulated_cpu_cycles = 0;
    dma.cpu_cycles_left = 640;
    dma.dest_pointer = destination;
    dma.src_pointer = source;
    dma.src_low = source;
    dma.src_high = source + size - 1;
    dma.size = size;

    lock_region(0x00, 0xFF7F);
}

bool MMU::is_in_locked_region(memaddr_t address)
{
    for (auto& region : locked_regions)
    {
        if (region.first <= address && region.second >= address)
        {
            return true;
        }
    }
    return false;
}

void MMU::lock_region(memaddr_t low, memaddr_t high)
{
    locked_regions.push_back({low, high});
}

void MMU::unlock_region(memaddr_t low, memaddr_t high)
{
    auto it = locked_regions.begin();
    while (it != locked_regions.end())
    {
        if (it->first == low && it->second == high)
        {
            locked_regions.erase(it);
            break;
        }
        ++it;
    }
}

void MMU::emulate(uint64_t cpu_cycles)
{
    emulate_oam_dma(cpu_cycles);
}

void MMU::emulate_oam_dma(uint64_t cpu_cycles)
{
    dma.unemulated_cpu_cycles += cpu_cycles;

    while (dma.unemulated_cpu_cycles >= 4 && dma.cpu_cycles_left >= 4)
    {
        mem[dma.dest_pointer] = mem[dma.src_pointer];
        dma.unemulated_cpu_cycles -= 4;
        dma.cpu_cycles_left -= 4;
    }

    if (dma.cpu_cycles_left < 4)
    {
        end_oam_dma();
    }
}

void MMU::end_oam_dma()
{
    clear_dma_status();

    unlock_region(0x0000, 0xFF7F);
}

void MMU::clear_dma_status()
{
    dma.unemulated_cpu_cycles = 0;
    dma.cpu_cycles_left = 0;
    dma.src_pointer = 0x0000;
    dma.dest_pointer = 0x0000;
    dma.src_low = 0x0000;
    dma.src_high = 0x0000;
    dma.size = 0;
}
