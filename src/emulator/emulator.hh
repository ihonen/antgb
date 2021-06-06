#pragma once

#include "debugger/iemulator.hh"

#include "macros.hh"
#include "emulator/memory/memorybus.hh"
#include "emulator/memory/bootrom.hh"
#include "emulator/memory/hram.hh"
#include "emulator/memory/oam.hh"
#include "emulator/memory/vram.hh"
#include "emulator/memory/wram0.hh"
#include "emulator/memory/wram1.hh"

#include <memory>

class Cartridge;

class ApuRegisters;
class CpuRegisters;
class JoypadRegisters;
class SerialRegisters;
class TimerRegisters;

class Cpu;
class Joypad;
class MemoryBus;
class Ppu;
class Serial;
class Timer;

class Emulator : public iEmulator
{
public:
    Emulator();
    virtual ~Emulator() override;

    virtual void set_frontend(iFrontend* frontend) override;

    virtual void load_rom(const std::string& filepath) override;
    inline virtual int execute_next() override;
    virtual void button_pressed(JoypadButton button) override;
    virtual void button_released(JoypadButton button) override;
    virtual void reset_emulation() override;

    inline virtual uint16_t read(regid_t register_id) override;
    inline virtual void write(regid_t register_id, uint16_t value) override;

    std::unique_ptr<BootRom> bootrom_;
    std::unique_ptr<Hram> hram_;
    std::unique_ptr<Oam> oam_;
    std::unique_ptr<Vram> vram_;
    std::unique_ptr<Wram0> wram0_;
    std::unique_ptr<Wram1> wram1_;

    std::unique_ptr<Cartridge> cartridge_;

    std::unique_ptr<ApuRegisters> apu_registers_;
    std::unique_ptr<CpuRegisters> cpu_registers_;
    std::unique_ptr<JoypadRegisters> joypad_registers_;
    std::unique_ptr<PpuRegisters> ppu_registers_;
    std::unique_ptr<SerialRegisters> serial_registers_;
    std::unique_ptr<TimerRegisters> timer_registers_;

    std::unique_ptr<MemoryBus> memory_bus_;

    std::unique_ptr<Cpu> cpu_;
    std::unique_ptr<Joypad> joypad_;
    std::unique_ptr<Ppu> ppu_;
    std::unique_ptr<Serial> serial_;
    std::unique_ptr<Timer> timer_;
};
