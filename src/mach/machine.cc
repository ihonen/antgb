#include "machine.hh"

#include <iostream>
#include <cstring>

using namespace std;

Machine::Machine()
{
    mmu = new MMU();
    irc = new IRC(*mmu);
    cpu = new CPU(*mmu, *irc);
    ppu = new PPU(*mmu, *irc);
    joypad = new Joypad(*mmu, *irc);
    timer_divider = new TimerDivider(*mmu, *irc);
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
    uint64_t cpu_cycles = cpu_tick();
    timer_divider->emulate(cpu_cycles);
    ppu->emulate(cpu_cycles);
}

uint64_t Machine::cpu_tick()
{
    uint64_t cpu_cycle_count_before = cpu->get_cycles();
    cpu->execute();
    uint64_t cpu_cycle_count_after = cpu->get_cycles();
    cpu->reset_cycles();
    return cpu_cycle_count_after - cpu_cycle_count_before;
}

void Machine::button_pressed(Joypad::Button button)
{
    joypad->button_pressed(button);
}

void Machine::button_released(Joypad::Button button)
{
    joypad->button_released(button);
}
