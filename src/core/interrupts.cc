#include "interrupts.hh"

#include "memory.hh"
#include <iostream>

using namespace std;

Irc::Irc(Memory* memory) :
    mem(memory)
{
    hard_reset();
}

void Irc::hard_reset()
{
    interrupt_master_enable = 0x00;
}

bool Irc::has_pending_requests()
{
    return (mem->hff0f_if & 0x1F) != 0;
}

Irc::InterruptInfo Irc::next_request()
{
    for (InterruptId i = VBlankInterrupt; i < JoypadInterrupt; i = (InterruptId)((int)i + 1))
    {
        if (interrupt_enabled(i) && interrupt_requested(i))
        {
            return {(InterruptId)i, VECTOR_ADDRESS[i]};
        }
    }

    return {NoInterrupt, 0x0000};
}

uint8_t Irc::ime_flag_get()
{
    return interrupt_master_enable;
}

void Irc::ime_flag_set()
{
    interrupt_master_enable = 0x01;
}

void Irc::ime_flag_clear()
{
    interrupt_master_enable = 0x00;
}

void Irc::request_interrupt(int source)
{
    /*
    switch (source)
    {
        case VBlankInterrupt:
            cerr << "Vblank IRQ" << endl;
            break;
        case LcdStatInt:
            cerr << "LCD STAT IRQ" << endl;
            break;
        case JoypadInterrupt:
            cerr << "Joypad IRQ" << endl;
            break;
        case TimerInterrupt:
            cerr << "Timer IRQ" << endl;
            break;
        case SerialInterrupt:
            cerr << "Serial IRQ" << endl;
            break;
    }
    */

    mem->hff0f_if |= 0x01 << source;
}

bool Irc::interrupt_requested(int source)
{
    return (mem->hff0f_if & (0x01 << source)) != 0;
}

bool Irc::interrupt_enabled(int source)
{
    return (*mem->hffff_ie & (0x01 << source)) != 0;
}

void Irc::clear_interrupt(int source)
{
    mem->hff0f_if &= ~(0x01 << source);
}

void Irc::disable_interrupt(int source)
{
    *mem->hffff_ie &= ~(0x01 << source);
}

void Irc::enable_interrupt(int source)
{
    *mem->hffff_ie |= 0x01 << source;
}
