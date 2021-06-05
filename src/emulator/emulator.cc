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

    cartridge = nullptr;
    cpu = std::make_unique<Cpu>(mem.get(), mem->get(IE_ADDR), mem->get(IF_ADDR));
    joypad = std::make_unique<Joypad>(mem.get(), cpu.get());
    ppu = std::make_unique<Ppu>(mem.get(), reinterpret_cast<Ppu::Registers*>(mem->get(PPU_LOW)), cpu.get());
    serial = std::make_unique<Serial>(reinterpret_cast<Serial::Registers*>(mem->get(SERIAL_LOW)), cpu.get());
    timer_divider = std::make_unique<Timer>(reinterpret_cast<Timer::Registers*>(mem->get(TIMER_LOW)), cpu.get());
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
}
