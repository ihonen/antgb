#include "cpu.hh"

#include <iostream>
using namespace std;

void CPU::jump_to_isr(memaddr_t vector_address)
{
    if (vector_address == 0x00) return;

    cout << "Jumping to interrupt vector @ " << vector_address << endl;

    is_interrupted = true;
    irc.ime_flag_clear();
    PUSH_r16(PC);
    PC = vector_address;
    curr_instr = &mmu.mem[PC];
    DI_status = IMEStatus::DO_NOTHING;
    EI_status = IMEStatus::DO_NOTHING;
    clock_cycles += 20;
}
