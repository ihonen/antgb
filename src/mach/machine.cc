#include "machine.hh"

#include "../gfx/renderer.hh"
#include <iostream>
#include <cstring>
#include <thread>

using namespace std;

Machine::Machine()
{
    mem = new Memory();
    irc = new InterruptController(mem);
    ppu = new Ppu(mem, irc);
    cpu = new Cpu(mem, irc);
    joypad = new Joypad(mem, irc);
    timer_divider = new Timer(mem, irc);
    cartridge = nullptr;
    renderer = new Renderer(mem);
}

Machine::~Machine()
{
    delete mem;
    delete irc;
    delete ppu;
    delete cpu;
    delete joypad;
    delete timer_divider;
    delete renderer;
    delete cartridge;
}

void Machine::insert_cartridge(Cartridge* cartridge_)
{
    cartridge = cartridge_;
    mem->set_cartridge(cartridge);
    hard_reset();
}

void Machine::tick()
{

    uint64_t cpu_cycles = cpu_tick();
    timer_divider->emulate(cpu_cycles);
    mem->emulate(cpu_cycles);
    ppu->emulate(cpu_cycles);

    static uint64_t cycles_since_last_frame = 0;
    cycles_since_last_frame += cpu_cycles;
    if (cycles_since_last_frame >= 70224)
    {
        renderer->render_frame();
        cycles_since_last_frame -= 70224;
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
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

void Machine::hard_reset()
{
    mem->hard_reset();
    cpu->hard_reset();
    irc->hard_reset();
}
