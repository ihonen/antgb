#include "serial.hh"

#include "memory.hh"

Serial::Serial(Memory* memory, Irc* irc_)
{
    irc = irc_;
    mem = memory;
    cpu_cycles_left_in_transfer = 0;
}

void Serial::emulate(uint64_t cpu_cycles)
{
    if (mem->serialreg.sb != 0x00 && mem->serialreg.sc == 0x81)
    {
        cout << (char)mem->serialreg.sb;
        std::cout << std::flush;
        mem->serialreg.sb = 0x00;
        cpu_cycles_left_in_transfer = CPU_CYCLES_PER_BYTE;
    }

    if (cpu_cycles_left_in_transfer <= cpu_cycles && cpu_cycles_left_in_transfer != 0)
    {
        irc->request_interrupt(Irc::SerialInterrupt);
        cpu_cycles_left_in_transfer = 0;
        mem->serialreg.sc = 0x01;
    }
    else
    {
        cpu_cycles_left_in_transfer -= cpu_cycles;
    }

}
