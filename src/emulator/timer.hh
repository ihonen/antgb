#pragma once

#include "cpu.hh"
#include "types.hh"

class Timer
{
public:
    struct Registers
    {
        uint8_t div;
        uint8_t tima;
        uint8_t tma;
        uint8_t tac;
    };

    enum BitPos
    {
        InputClockSelect0 = 0,
        InputclockSelect1 = 1,
        TimerEnable = 2
    };

    Timer(Registers* reg, Cpu* cpu);
    inline void emulate(uint64_t cpu_cycles);
    inline void emulate_divider(uint64_t cpu_cycles);
    inline void emulate_timer(uint64_t cpu_cycles);

    Registers* reg;
    Cpu* cpu;

    struct
    {
        const uint64_t FREQ_Hz = 16384;
        const uint64_t CPU_CYCLES_PER_TICK = Cpu::CLK_FREQ_Hz / FREQ_Hz;
        const memaddr_t REGISTER_ADDRESS = 0xFF04;
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
        ++(reg->div);
        divider.unemulated_cpu_cycles -= divider.CPU_CYCLES_PER_TICK;
    }
}

FORCE_INLINE void Timer::emulate_timer(uint64_t cpu_cycles)
{
    timer.unemulated_cpu_cycles += cpu_cycles;

    if ((reg->tac & (0x01 << TimerEnable)) == 0x00)
    {
        timer.unemulated_cpu_cycles = 0;
        return;
    }

    uint64_t clk_select = reg->tac & (timer.CLK_SELECT_MASK);
    uint64_t cpu_cycles_per_tick = timer.FREQ_DIVIDER[clk_select];

    while (timer.unemulated_cpu_cycles >= cpu_cycles_per_tick)
    {
        timer.unemulated_cpu_cycles -= cpu_cycles_per_tick;
        if (reg->tima == 0xFF)
        {
            reg->tima = reg->tma;
            cpu->request_interrupt(Cpu::TimerInterrupt);
            return;
        }
        else
        {
            ++(reg->tima);
        }
    }
}
