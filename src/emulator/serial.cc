#include "serial.hh"

#include "memory.hh"

Serial::Serial(SerialRegisters& reg, Cpu* cpu) :
    cpu(cpu),
    reg(reg)
{
    cpu_cycles_left_in_transfer = 0;
}
