#include "emulator.hh"

#include "addresses.hh"
#include "cartridge.hh"
#include "cpu.hh"
#include "cpuregisters.hh"
#include "joypad.hh"
#include "memory.hh"
#include "ppu.hh"
#include "ppuregisters.hh"
#include "serial.hh"
#include "timer.hh"

Emulator::Emulator()
{
    apu_registers = std::make_unique<ApuRegisters>();
    cpu_registers = std::make_unique<CpuRegisters>();
    joypad_registers = std::make_unique<JoypadRegisters>();
    ppu_registers = std::make_unique<PpuRegisters>();

    mem = std::make_unique<Memory>(
        *apu_registers,
        *cpu_registers,
        *joypad_registers,
        *ppu_registers
    );

    cartridge = nullptr;
    cpu = std::make_unique<Cpu>(mem.get(), *cpu_registers);
    joypad = std::make_unique<Joypad>(*joypad_registers, cpu.get());
    ppu = std::make_unique<Ppu>(mem.get(), *ppu_registers, cpu.get());
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
