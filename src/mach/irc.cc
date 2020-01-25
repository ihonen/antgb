#include "irc.hh"

#include <iostream>

using namespace std;

IRC::IRC(MMU& mmu_) :
    mmu(mmu_)
{
    interrupt_enable = mmu.get(0xFFFF);
    interrupt_flag = mmu.get(0xFF0F);
}

bool IRC::has_active_requests()
{
    return ime_flag_get() && (*interrupt_flag & 0x1F) != 0;
}

IRC::InterruptInfo IRC::accept_next_request()
{
    for (InterruptSource i = VBlankInterrupt; i < JoypadInterrupt; i = (InterruptSource)((int)i + 1))
    {
        if (interrupt_enabled(i) && interrupt_requested(i))
        {
            ime_flag_clear();
            clear_interrupt(i);
            return {(InterruptSource)i, VECTOR_ADDRESS[i]};
        }
    }

    return {NoInterrupt, 0x0000};
}

uint8_t IRC::ime_flag_get()
{
    return interrupt_master_enable;
}

void IRC::ime_flag_set()
{
    interrupt_master_enable = 0x01;
}

void IRC::ime_flag_clear()
{
    interrupt_master_enable = 0x00;
}

void IRC::request_interrupt(int source)
{
    cout << "IR: " << source << endl;
    *interrupt_flag |= 0x01 << source;
}

bool IRC::interrupt_requested(int source)
{
    return (*interrupt_flag & (0x01 << source)) != 0;
}

bool IRC::interrupt_enabled(int source)
{
    return (*interrupt_enable & (0x01 << source)) != 0;
}

void IRC::clear_interrupt(int source)
{
    *interrupt_flag &= ~(0x01 << source);
}

void IRC::disable_interrupt(int source)
{
    *interrupt_enable &= ~(0x01 << source);
}

void IRC::enable_interrupt(int source)
{
    *interrupt_enable |= 0x01 << source;
}
