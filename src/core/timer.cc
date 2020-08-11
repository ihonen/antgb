#include "timer.hh"

#include "mmu.hh"

#include <iostream>

namespace antgb
{
Timer::Timer(Registers* reg, Cpu* cpu) : reg(reg), cpu(cpu)
{
    timer.unemulated_cpu_cycles = 0;
    divider.unemulated_cpu_cycles = 0;
}

} // namespace antgb
