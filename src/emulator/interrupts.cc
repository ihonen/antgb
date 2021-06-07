#include "interrupts.hh"

#include "memorybus.hh"
#include <iostream>

Irc::Irc(uint8_t* IF, uint8_t* IE) :
    IF(IF),
    IE(IE)
{
    hard_reset();
}

void Irc::hard_reset()
{
    IME = 0x00;
}
