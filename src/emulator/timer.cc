#include "timer.hh"

#include "memorybus.hh"

#include <iostream>

Timer::Timer(TimerRegisters& reg, Cpu* cpu) :
    reg(reg),
    cpu(cpu)
{
    timer.unemulated_cpu_cycles = 0;
    divider.unemulated_cpu_cycles = 0;
}
