#pragma once

#include "emulator/cpu/cpu.hh"
#include "timerregisters.hh"

class Timer
{
public:
    Timer(TimerRegisters& reg, Interrupts& interrupts);
    inline void emulate(uint64_t cpu_cycles);
    inline void emulate_divider(uint64_t cpu_cycles);
    inline void emulate_timer(uint64_t cpu_cycles);

    TimerRegisters& reg;
    Interrupts& interrupts;

    struct
    {
        const uint64_t FREQ_Hz = 16384;
        const uint64_t CPU_CYCLES_PER_TICK = Cpu::CLK_FREQ_Hz / FREQ_Hz;
        const addr_t REGISTER_ADDRESS = 0xFF04;
        uint64_t unemulated_cpu_cycles;
    } divider;

    struct
    {
        const uint8_t CLK_SELECT_MASK = 0x03; // Bits 0-1
        uint64_t unemulated_cpu_cycles;
        // Selected with bits [0:1] of the control register.
        const uint64_t FREQ_DIVIDER[4] =
        {
            1024, 16, 64, 256
        };
    } timer;
};

FORCE_INLINE void Timer::emulate(uint64_t cpu_cycles)
{
    emulate_divider(cpu_cycles);
    emulate_timer(cpu_cycles);
}

FORCE_INLINE void Timer::emulate_divider(uint64_t cpu_cycles)
{
    divider.unemulated_cpu_cycles += cpu_cycles;
    while (divider.unemulated_cpu_cycles <= divider.CPU_CYCLES_PER_TICK)
    {
        reg.write(DIV_ADDR, reg.read(DIV_ADDR) + 1);
        divider.unemulated_cpu_cycles -= divider.CPU_CYCLES_PER_TICK;
    }
}

FORCE_INLINE void Timer::emulate_timer(uint64_t cpu_cycles)
{
    timer.unemulated_cpu_cycles += cpu_cycles;

    if ((reg.read(TAC_ADDR) & (0x01 << TimerRegisters::TimerEnable)) == 0x00)
    {
        timer.unemulated_cpu_cycles = 0;
        return;
    }

    uint64_t clk_select = reg.read(TAC_ADDR) & (timer.CLK_SELECT_MASK);
    uint64_t cpu_cycles_per_tick = timer.FREQ_DIVIDER[clk_select];

    while (timer.unemulated_cpu_cycles >= cpu_cycles_per_tick)
    {
        timer.unemulated_cpu_cycles -= cpu_cycles_per_tick;
        if (reg.read(TIMA_ADDR) == 0xFF)
        {
            reg.write(TIMA_ADDR, reg.read(TMA_ADDR));
            interrupts.request_interrupt(Interrupts::Timer);
            return;
        }
        else
        {
            reg.write(TIMA_ADDR, reg.read(TIMA_ADDR) + 1);
        }
    }
}
