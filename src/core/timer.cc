#include "timer.hh"

#include "memory.hh"

#include <iostream>
using namespace std;

Timer::Timer(Memory* memory, Irc* irc_) :
    mem(memory),
    irc(irc_)
{
    timer.unemulated_cpu_cycles = 0;
    divider.unemulated_cpu_cycles = 0;
}

void Timer::emulate(uint64_t cpu_cycles)
{
    emulate_divider(cpu_cycles);
    emulate_timer(cpu_cycles);
}

void Timer::emulate_divider(uint64_t cpu_cycles)
{
    divider.unemulated_cpu_cycles += cpu_cycles;
    while (divider.unemulated_cpu_cycles <= divider.CPU_CYCLES_PER_TICK)
    {
        ++(mem->timerreg.div);
        divider.unemulated_cpu_cycles -= divider.CPU_CYCLES_PER_TICK;
    }
}

void Timer::emulate_timer(uint64_t cpu_cycles)
{
    timer.unemulated_cpu_cycles += cpu_cycles;

    if ((mem->timerreg.tac & (0x01 << TimerEnable)) == 0x00)
    {
        timer.unemulated_cpu_cycles = 0;
        return;
    }

    uint64_t clk_select = mem->timerreg.tac & (timer.CLK_SELECT_MASK);
    uint64_t cpu_cycles_per_tick = timer.FREQ_DIVIDER[clk_select];

    while (timer.unemulated_cpu_cycles >= cpu_cycles_per_tick)
    {
        timer.unemulated_cpu_cycles -= cpu_cycles_per_tick;
        if (mem->timerreg.tima == 0xFF)
        {
            mem->timerreg.tima = mem->timerreg.tma;
            irc->request_interrupt(Irc::TimerInterrupt);
            return;
        }
        else
        {
            ++(mem->timerreg.tima);
        }
    }
}
