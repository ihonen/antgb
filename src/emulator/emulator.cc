#include "emulator.hh"

#include "emulator/memory/addresses.hh"
#include "emulator/memory/cartridge.hh"
#include "emulator/cpu/cpu.hh"
#include "emulator/cpu/cpuregisters.hh"
#include "emulator/peripherals/joypad.hh"
#include "emulator/memory/memorybus.hh"
#include "emulator/ppu/ppu.hh"
#include "emulator/ppu/ppuregisters.hh"
#include "emulator/peripherals/serial.hh"
#include "emulator/peripherals/timer.hh"

Emulator::Emulator()
    : joypad_press_callback(nullptr)
    , render_callback(nullptr)
    , serial_callback(nullptr)
{
    bootrom = std::make_unique<BootRom>("bootix_dmg.bin");
    hram = std::make_unique<Hram>();
    oam = std::make_unique<Oam>();
    vram = std::make_unique<Vram>();
    wram0 = std::make_unique<Wram0>();
    wram1 = std::make_unique<Wram1>();
    echoram = std::make_unique<EchoRam>(*wram0, *wram1);

    cartridge = std::make_unique<Cartridge>();

    apu_registers = std::make_unique<ApuRegisters>();
    cpu_registers = std::make_unique<CpuRegisters>();
    joypad_registers = std::make_unique<JoypadRegisters>();
    ppu_registers = std::make_unique<PpuRegisters>();
    serial_registers = std::make_unique<SerialRegisters>();
    timer_registers = std::make_unique<TimerRegisters>();

    mem = std::make_unique<MemoryBus>(
        *bootrom,
        *echoram,
        *hram,
        *oam,
        *vram,
        *wram0,
        *wram1,
        *cartridge,
        *apu_registers,
        *cpu_registers,
        *joypad_registers,
        *ppu_registers,
        *serial_registers,
        *timer_registers
    );

    interrupts = std::make_unique<Interrupts>(*cpu_registers);

    timer = std::make_unique<Timer>(*timer_registers, *interrupts);

    cpu = std::make_unique<Cpu>(*cpu_registers, *interrupts, *mem, *timer);
    dma = std::make_unique<Dma>(*mem);
    joypad = std::make_unique<Joypad>(*joypad_registers, *interrupts);
    ppu = std::make_unique<Ppu>(*ppu_registers, *interrupts, *mem, *dma);
    serial = std::make_unique<Serial>(*serial_registers, *interrupts);
}

Emulator::~Emulator() = default;

void Emulator::set_joypad_press_callback(iFrontend::JoypadCallback callback)
{
    joypad_press_callback = callback;
    joypad->set_joypad_press_callback(callback);
}

void Emulator::set_joypad_release_callback(iFrontend::JoypadCallback callback)
{
    joypad_release_callback = callback;
    joypad->set_joypad_release_callback(callback);
}

void Emulator::set_render_callback(iFrontend::RenderCallback callback)
{
    render_callback = callback;
    ppu->set_render_callback(callback);
}

void Emulator::set_serial_callback(iFrontend::SerialCallback callback)
{
    serial_callback = callback;
    serial->set_serial_callback(callback);
}

void Emulator::load_rom(const std::string& filepath)
{
    *cartridge = Cartridge(filepath);
    cartridge->set_inserted(true);

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
    cpu->post_bootram_reset();
}
