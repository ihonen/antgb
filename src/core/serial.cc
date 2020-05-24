#include "serial.hh"

#include "memory.hh"

namespace antgb
{

Serial::Serial(Registers* reg, Irc* irc_) :
    irc(irc_),
    reg(reg)
{
    cpu_cycles_left_in_transfer = 0;
}

} // namespace antgb
