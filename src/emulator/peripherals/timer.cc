#include "timer.hh"

#include "emulator/memory/memorybus.hh"

#include <iostream>

Timer::Timer(TimerRegisters& reg, Interrupts& interrupts)
    : reg_(reg)
    , interrupts_(interrupts)
    , divider_unemulated_tcycles_(0)
    , timer_unemulated_tcycles_(0)
    , is_stopped_(false)
{
}

void Timer::set_stopped(bool stopped)
{
    is_stopped_ = stopped;
    if (is_stopped_)
    {
        reg_.write(DIV_ADDR, 0x00);
        timer_unemulated_tcycles_ = 0;
        divider_unemulated_tcycles_ = 0;
    }
}
