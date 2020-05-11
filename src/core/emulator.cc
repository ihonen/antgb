#include "emulator.hh"

#include "renderer.hh"
#include <iostream>
#include <cstring>
#include <thread>

using namespace std;

Emulator::Emulator()
{
    mem = new Memory();
    irc = new Irc(mem);
    ppu = new Ppu(mem, irc);
    cpu = new Cpu(mem, irc);
    joypad = new Joypad(mem, irc);
    timer_divider = new Timer(mem, irc);
    serial = new Serial(mem ,irc);
    cartridge = nullptr;
}

Emulator::~Emulator()
{
    delete mem;
    delete irc;
    delete ppu;
    delete cpu;
    delete joypad;
    delete timer_divider;
    delete serial;
    delete cartridge;
}

void Emulator::insert_cartridge(Cartridge* cartridge_)
{
    cartridge = cartridge_;
    mem->set_cartridge(cartridge);
    hard_reset();
}

void Emulator::tick()
{
    uint64_t cpu_cycles = cpu_tick();
    timer_divider->emulate(cpu_cycles);
    mem->emulate(cpu_cycles);
    ppu->step(cpu_cycles);
    serial->emulate(cpu_cycles);
    static uint64_t cpu_cycles_total = 0;
    cpu_cycles_total += cpu_cycles;

    //cout << std::dec << cpu_cycles_total << endl;
}

uint64_t Emulator::cpu_tick()
{
    uint64_t cpu_cycle_count_before = cpu->get_cycles();
    cpu->execute();
    uint64_t cpu_cycle_count_after = cpu->get_cycles();
    cpu->reset_cycles();
    return cpu_cycle_count_after - cpu_cycle_count_before;
}

void Emulator::button_pressed(Joypad::Button button)
{
    joypad->button_pressed(button);
}

void Emulator::button_released(Joypad::Button button)
{
    joypad->button_released(button);
}

void Emulator::hard_reset()
{
    mem->hard_reset();
    cpu->hard_reset();
    irc->hard_reset();
}
