#include "serial.hh"

#include "emulator/memory/memorybus.hh"

Serial::Serial(Registers* reg, Cpu* cpu) :
    cpu(cpu),
    reg(reg)
{
    cpu_cycles_left_in_transfer = 0;
}
