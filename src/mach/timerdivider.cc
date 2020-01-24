#include "timerdivider.hh"

#include <iostream>
using namespace std;

TimerDivider::TimerDivider(MMU& mmu_, IRC& irc_) :
    mmu(mmu_),
    irc(irc_)
{
    timer.counter = &mmu.mem[timer.COUNTER_ADDRESS];
    timer.modulo = &mmu.mem[timer.MODULO_ADDRESS];
    timer.control = &mmu.mem[timer.CONTROL_ADDRESS];
    timer.unemulated_cpu_cycles = 0;

    divider.counter = &mmu.mem[divider.REGISTER_ADDRESS];
    divider.unemulated_cpu_cycles = 0;
}

void TimerDivider::emulate(uint64_t cpu_cycles)
{
    emulate_divider(cpu_cycles);
    emulate_timer(cpu_cycles);
}

void TimerDivider::emulate_divider(uint64_t cpu_cycles)
{
    divider.unemulated_cpu_cycles += cpu_cycles;
    while (divider.unemulated_cpu_cycles <= divider.CPU_CYCLES_PER_TICK)
    {
        ++(*divider.counter);
        divider.unemulated_cpu_cycles -= divider.CPU_CYCLES_PER_TICK;
    }
}

void TimerDivider::emulate_timer(uint64_t cpu_cycles)
{
    timer.unemulated_cpu_cycles += cpu_cycles;

    if (!(*timer.control & TimerEnable))
    {
        timer.unemulated_cpu_cycles = 0;
        return;
    }

    uint64_t clk_select = *timer.control & (timer.CLK_SELECT_MASK);
    uint64_t cpu_cycles_per_tick = CPU::CLK_FREQ_Hz / timer.FREQ_DIVIDER[clk_select];

    while (timer.unemulated_cpu_cycles >= cpu_cycles_per_tick)
    {
        cout << *timer.counter << endl;
        if (*timer.counter == 0xFF)
        {
            *timer.counter = *timer.modulo;
            irc.request_interrupt(IRC::TimerInterrupt);
        }
        else
        {
            ++(*timer.counter);
        }
        timer.unemulated_cpu_cycles -= cpu_cycles_per_tick;
    }
}
