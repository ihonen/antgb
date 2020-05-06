#include "timer.hh"

#include <iostream>
using namespace std;

Timer::Timer(Memory* memory, InterruptController* irc_) :
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
        ++(mem->hff04_div);
        divider.unemulated_cpu_cycles -= divider.CPU_CYCLES_PER_TICK;
    }
}

void Timer::emulate_timer(uint64_t cpu_cycles)
{
    timer.unemulated_cpu_cycles += cpu_cycles;

    if ((mem->hff07_tac & (0x01 << TimerEnable)) == 0x00)
    {
        timer.unemulated_cpu_cycles = 0;
        return;
    }

    uint64_t clk_select = mem->hff07_tac & (timer.CLK_SELECT_MASK);
    uint64_t cpu_cycles_per_tick = timer.FREQ_DIVIDER[clk_select];

    while (timer.unemulated_cpu_cycles >= cpu_cycles_per_tick)
    {
        timer.unemulated_cpu_cycles -= cpu_cycles_per_tick;
        if (mem->hff05_tima == 0xFF)
        {
            mem->hff05_tima = mem->hff06_tma;
            irc->request_interrupt(InterruptController::TimerInterrupt);
            return;
        }
        else
        {
            ++(mem->hff05_tima);
        }
    }
}
