#include "serial.hh"

#include "emulator/memory/memorybus.hh"

Serial::Serial(SerialRegisters& reg, Interrupts& interrupts, iFrontend* frontend) :
    interrupts(interrupts),
    reg(reg),
    frontend_(frontend)
{
    cpu_cycles_left_in_transfer = 0;
}

void Serial::set_frontend(iFrontend* frontend)
{
    frontend_ = frontend;
}
