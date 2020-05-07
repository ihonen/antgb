#pragma once

#include "cpu.hh"
#include "interrupts.hh"
#include "memory.hh"

class Timer
{
public:
    enum BitPos
    {
        InputClockSelect0 = 0,
        InputclockSelect1 = 1,
        TimerEnable = 2
    };

    Timer(Memory* mem, Irc* irc);
    void emulate(uint64_t cpu_cycles);
    void emulate_divider(uint64_t cpu_cycles);
    void emulate_timer(uint64_t cpu_cycles);

    Memory* mem;
    Irc* irc;

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
