#ifndef TIMERDIVIDER_HH
#define TIMERDIVIDER_HH

#include "cpu.hh"
#include "irc.hh"
#include "mmu.hh"

class TimerDivider
{
public:
    enum BitPos
    {
        InputClockSelect0 = 0,
        InputclockSelect1 = 1,
        TimerEnable = 2
    };

    TimerDivider(MMU& mmu, IRC& irc);
    void emulate(uint64_t cpu_cycles);
    void emulate_divider(uint64_t cpu_cycles);
    void emulate_timer(uint64_t cpu_cycles);

    MMU& mmu;
    IRC& irc;

    struct
    {
        const uint64_t FREQ_Hz = 16384;
        const uint64_t CPU_CYCLES_PER_TICK = CPU::CLK_FREQ_Hz / FREQ_Hz;
        const memaddr_t REGISTER_ADDRESS = 0xFF04;
        uint64_t unemulated_cpu_cycles;
        uint8_t* counter;
    } divider;

    struct
    {
        const uint8_t CLK_SELECT_MASK = 0x03; // Bits 0-1
        const memaddr_t COUNTER_ADDRESS = 0xFF05;
        const memaddr_t MODULO_ADDRESS = 0xFF06;
        const memaddr_t CONTROL_ADDRESS = 0xFF07;
        uint8_t* counter;
        uint8_t* modulo;
        uint8_t* control;
        uint64_t unemulated_cpu_cycles;
        // Selected with bits [0:1] of the control register.
        const uint64_t FREQ_DIVIDER[4] =
        {
            1024, 16, 64, 256
        };
    } timer;
};

#endif // TIMERDIVIDER_HH
