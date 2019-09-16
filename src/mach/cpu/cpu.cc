#include "cpu.hh"
#include <cassert>

void Cpu::execute(uint8_t* opcode)
{
    curr_op = opcode;
    void (Cpu::*handler)();
    handler = (*curr_op == 0xCB) ?
              CB_OP_INFO[*(curr_op + 1)].handler :
              OP_INFO[*(curr_op)].handler;

    if (handler) (this->*handler)();
}
