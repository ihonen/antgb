#include "interrupts.hh"

#include <iostream>

using namespace std;

InterruptController::InterruptController(Memory* memory) :
    mem(memory)
{
    hard_reset();
}

void InterruptController::hard_reset()
{
    interrupt_master_enable = 0x00;
}

bool InterruptController::has_pending_requests()
{
    return (mem->hff0f_if & 0x1F) != 0;
}

InterruptController::InterruptInfo InterruptController::next_request()
{
    for (InterruptId i = VBlankInterrupt; i < JoypadInterrupt; i = (InterruptId)((int)i + 1))
    {
        if (interrupt_enabled(i) && interrupt_requested(i))
        {
            return {(InterruptId)i, VECTOR_ADDRESS[i]};
        }
    }

    return {NoInterrupt, 0x0000};
}

uint8_t InterruptController::ime_flag_get()
{
    return interrupt_master_enable;
}

void InterruptController::ime_flag_set()
{
    interrupt_master_enable = 0x01;
}

void InterruptController::ime_flag_clear()
{
    interrupt_master_enable = 0x00;
}

void InterruptController::request_interrupt(int source)
{
    mem->hff0f_if |= 0x01 << source;
}

bool InterruptController::interrupt_requested(int source)
{
    return (mem->hff0f_if & (0x01 << source)) != 0;
}

bool InterruptController::interrupt_enabled(int source)
{
    return (*mem->hffff_ie & (0x01 << source)) != 0;
}

void InterruptController::clear_interrupt(int source)
{
    mem->hff0f_if &= ~(0x01 << source);
}

void InterruptController::disable_interrupt(int source)
{
    *mem->hffff_ie &= ~(0x01 << source);
}

void InterruptController::enable_interrupt(int source)
{
    *mem->hffff_ie |= 0x01 << source;
}
