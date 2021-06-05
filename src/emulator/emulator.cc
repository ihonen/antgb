#include "emulator.hh"

#include "addresses.hh"
#include "cartridge.hh"
#include "cpu.hh"
#include "joypad.hh"
#include "memory.hh"
#include "ppu.hh"
#include "serial.hh"
#include "timer.hh"

Emulator::Emulator()
{
    mem = std::make_unique<Memory>();
    irc = std::make_unique<Irc>(mem->get(IF_ADDR), mem->get(IE_ADDR));

    cartridge = nullptr;
    cpu = std::make_unique<Cpu>(mem.get(), irc.get());
    joypad = std::make_unique<Joypad>(mem.get(), irc.get());
    ppu = std::make_unique<Ppu>(mem.get(), reinterpret_cast<Ppu::Registers*>(mem->get(PPU_LOW_ADDR)), irc.get());
    serial = std::make_unique<Serial>(reinterpret_cast<Serial::Registers*>(mem->get(SERIAL_LOW_ADDR)), irc.get());
    timer_divider = std::make_unique<Timer>(reinterpret_cast<Timer::Registers*>(mem->get(TIMER_LOW_ADDR)), irc.get());
}

Emulator::~Emulator() = default;

void Emulator::set_frontend(iFrontend* frontend)
{
    ppu->set_frontend(frontend);
}

void Emulator::load_rom(const std::string& filepath)
{
    cartridge = std::make_unique<Cartridge>(filepath);

    mem->set_cartridge(cartridge.get());
    reset_emulation();
}

void Emulator::button_pressed(JoypadButton button)
{
    joypad->button_pressed(button);
}

void Emulator::button_released(JoypadButton button)
{
    joypad->button_released(button);
}

void Emulator::reset_emulation()
{
    mem->hard_reset();
    cpu->hard_reset();
    irc->hard_reset();
}
