#include "serial.hh"

#include "mmu.hh"

namespace antgb
{
Serial::Serial(Registers* reg, Cpu* cpu) : cpu(cpu), reg(reg)
{
    cpu_cycles_left_in_transfer = 0;
}

} // namespace antgb
