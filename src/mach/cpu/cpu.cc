#include "cpu.hh"
#include <cassert>

uint8_t Cpu::get_IME()
{
    return mem[0xFFFF];
}

void Cpu::set_IME()
{
    mem[0xFFFF] = 0x01;
}

void Cpu::clear_IME()
{
    mem[0xFFFF] = 0x00;
}

void Cpu::disable_interrupts()
{
    clear_IME();
    DI_status = IntStatusChange::NOT_SCHEDULED;
}

void Cpu::enable_interrupts()
{
    set_IME();
    EI_status = IntStatusChange::NOT_SCHEDULED;
}

void Cpu::execute(uint8_t* opcode)
{
    if (DI_status == IntStatusChange::SCHEDULED)
    {
        DI_status = IntStatusChange::TRIGGER;
    }
    else if (EI_status == IntStatusChange::SCHEDULED)
    {
        DI_status = IntStatusChange::TRIGGER;
    }

    curr_op = opcode;
    void (Cpu::*handler)();
    handler = (*curr_op == 0xCB) ?
              CB_OP_INFO[*(curr_op + 1)].handler :
              OP_INFO[*(curr_op)].handler;

    op_success = true;
    if (handler) (this->*handler)();

    if (DI_status == IntStatusChange::TRIGGER) disable_interrupts();
    else if (EI_status == IntStatusChange::TRIGGER) enable_interrupts();
}
