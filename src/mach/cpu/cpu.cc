#include "cpu.hh"
#include <cassert>

void Cpu::execute_next()
{
    (this->*(OP_INFO[mem[*PC]].handler))();
}
