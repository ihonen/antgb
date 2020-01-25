#include "machine.hh"

#include "../gfx/renderer.hh"
#include "bootrom.hh"
#include <iostream>
#include <cstring>
#include <thread>

using namespace std;

Machine::Machine()
{
    mmu = new MMU();
    irc = new IRC(*mmu);
    ppu = new PPU(mmu, irc);
    cpu = new CPU(*mmu, *irc);
    joypad = new Joypad(*mmu, *irc);
    timer_divider = new TimerDivider(*mmu, *irc);
    cartridge = nullptr;
    renderer = new Renderer(mmu);
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
    delete cartridge;
}

void Machine::insert_cartridge(Cartridge* cartridge_)
{
    cartridge = cartridge_;
    mmu->set_cartridge(cartridge);
    cpu->restart();
    cpu->set_PC(0x0100);
}

void Machine::tick()
{
    static auto last_frame_time = std::chrono::high_resolution_clock::now();
    auto now = std::chrono::high_resolution_clock::now();

    static uint64_t cycles_since_last_frame = 0;

    uint64_t cpu_cycles = cpu_tick();
    timer_divider->emulate(cpu_cycles);
    mmu->emulate(cpu_cycles);
    ppu->emulate(cpu_cycles);

    cycles_since_last_frame += cpu_cycles;

    if (cycles_since_last_frame >= 69905)
    {
        renderer->render_frame();
        cycles_since_last_frame -= 69905;
        now = std::chrono::high_resolution_clock::now();
        std::chrono::milliseconds ms_since_last_frame = std::chrono::milliseconds(std::chrono::duration_cast<std::chrono::milliseconds>((now - last_frame_time)).count());
        std::chrono::milliseconds sleep_time = std::chrono::milliseconds(17) - ms_since_last_frame;
        if (sleep_time.count() > 0)
        {
            std::this_thread::sleep_for(sleep_time);
        }
        last_frame_time = now;
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
