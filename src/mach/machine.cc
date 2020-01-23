#include "machine.hh"

#include "../gfx/renderer.hh"
#include <iostream>
#include <cstring>

using namespace std;

Machine::Machine()
{
    mmu = new MMU();
    irc = new IRC(*mmu);
    ppu = new PPU(*mmu, *irc);
    cpu = new CPU(*mmu, *irc);
    joypad = new Joypad(*mmu, *irc);
    timer_divider = new TimerDivider(*mmu, *irc);
    renderer = new Renderer(&ppu->reg, mmu->mem.data);
}

Machine::~Machine()
{
    delete mmu;
    delete irc;
    delete ppu;
    delete cpu;
    delete joypad;
    delete timer_divider;
    delete renderer;
}

void Machine::load_rom(void* rom, size_t size)
{
    memcpy(mmu->mem.data, rom, size);
    cpu->restart();
}

void Machine::tick()
{
    /*
    uint64_t cpu_cycles = cpu_tick();
    timer_divider->emulate(cpu_cycles);
    mmu->emulate(cpu_cycles);
    ppu->emulate(cpu_cycles);
    */
    static size_t i = 0;
    ++i;

    if (i == 4)
    {
        renderer->render_frame();
        i = 0;
    }
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
