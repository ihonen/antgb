#include "cpu.hh"
#include <cassert>

void Cpu::disable_interrupts()
{
    IME_flag_reset();
    DI_status = IntStatusChange::NOT_SCHEDULED;
}

void Cpu::enable_interrupts()
{
    IME_flag_set();
    EI_status = IntStatusChange::NOT_SCHEDULED;
}

void Cpu::execute(const uint8_t* instruction)
{
    curr_op = instruction;
    op_success = true;

    if (DI_status == IntStatusChange::SCHEDULED)
        DI_status = IntStatusChange::TRIGGERED;
    else if (EI_status == IntStatusChange::SCHEDULED)
        EI_status = IntStatusChange::TRIGGERED;

    void (Cpu::*handler)();
    handler = (*curr_op == 0xCB) ?
              CB_OP_INFO[*(curr_op + 1)].handler :
              OP_INFO[*(curr_op)].handler;

    if (handler) (this->*handler)();

    if (DI_status == IntStatusChange::TRIGGERED) disable_interrupts();
    else if (EI_status == IntStatusChange::TRIGGERED) enable_interrupts();
}

uint8_t Cpu::extract_immediate8()
{
    return curr_op[1];
}

uint16_t Cpu::extract_immediate16()
{
    return (static_cast<uint16_t>(curr_op[1])) |
           (static_cast<uint16_t>(curr_op[2]) << 8);
}

void Cpu::invalid_opcode()
{
    throw OpcodeError(*PC, mem[*PC]);
}
