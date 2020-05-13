#pragma once

#include "cpu.hh"
#include "interrupts.hh"
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

    static constexpr memaddr_t DIV_ADDRESS = 0xFF04;
    static constexpr memaddr_t TIMA_ADDRESS = 0xFF05;
    static constexpr memaddr_t TMA_ADDRESS = 0xFF06;
    static constexpr memaddr_t TAC_ADDRESS = 0xFF07;
    static constexpr memaddr_t LOW_ADDRESS = DIV_ADDRESS;
    static constexpr memaddr_t HIGH_ADDRESS = TAC_ADDRESS;

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
