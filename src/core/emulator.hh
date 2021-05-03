#pragma once

#include "cartridge.hh"
#include "cpu.hh"
#include "irenderer.hh"
#include "joypad.hh"
#include "macros.hh"
#include "mmu.hh"
#include "ppu.hh"
#include "serial.hh"
#include "timer.hh"
#include "types.hh"

namespace antgb
{
class Emulator
{
public:
    Emulator(iRenderer* gui_renderer = nullptr);
    ~Emulator();
    void       load_rom(const void* rom, size_t size);
    inline int execute_next();
    void       button_pressed(JoypadButton button);
    void       button_released(JoypadButton button);
    void       reset_emulation();

    iRenderer* renderer;

    Cpu*       cpu;
    Mmu*       mem;
    Ppu*       ppu;
    Joypad*    joypad;
    Timer*     timer_divider;
    Serial*    serial;
    Cartridge* cartridge;
};

ANTGB_FORCEINLINE int Emulator::execute_next()
{
    uint64_t cpu_cycle_count_before = cpu->get_cycles();
    cpu->execute();
    uint64_t cpu_cycle_count_after = cpu->get_cycles();
    int      clock_cycles = cpu_cycle_count_after - cpu_cycle_count_before;

    timer_divider->emulate(clock_cycles);
    mem->emulate(clock_cycles);
    ppu->step(clock_cycles);
    serial->emulate(clock_cycles);

    return clock_cycles;
}

} // namespace antgb
