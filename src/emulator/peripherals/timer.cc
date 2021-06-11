#include "timer.hh"

#include "emulator/memory/memorybus.hh"

#include <iostream>

Timer::Timer(TimerRegisters& reg, Interrupts& interrupts) :
    reg(reg),
    interrupts(interrupts)
{
    timer.unemulated_cpu_cycles = 0;
    divider.unemulated_cpu_cycles = 0;
}
