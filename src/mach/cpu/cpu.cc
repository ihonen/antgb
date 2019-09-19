#include "cpu.hh"
#include <cassert>

void CPU::execute(const uint8_t* instruction)
{
    if (!instruction) instruction = &mem[PC];

    curr_instr = instruction;

    if (DI_status == IMEStatus::RESET_NEXT_CYCLE)
        DI_status = IMEStatus::RESET_THIS_CYCLE;
    else if (EI_status == IMEStatus::SET_NEXT_CYCLE)
        EI_status = IMEStatus::SET_THIS_CYCLE;

    const InstrInfo* op_info = (*curr_instr == 0xCB) ?
                                &CB_INSTR_TABLE[curr_instr[1]] :
                                &INSTR_TABLE[*curr_instr];

    if (op_info->handler) (this->*(op_info->handler))();
    else invalid_opcode();

    if (branch_taken == BranchTaken::YES)
         clock_cycles += op_info->cycles_on_action;
    else clock_cycles += op_info->cycles_on_no_action;

    if (DI_status == IMEStatus::RESET_THIS_CYCLE) disable_interrupts_now();
    else if (EI_status == IMEStatus::SET_THIS_CYCLE) enable_interrupts_now();
}

void CPU::disable_interrupts_now()
{
    IME_flag_reset();
    DI_status = IMEStatus::DO_NOTHING;
}

void CPU::enable_interrupts_now()
{
    IME_flag_set();
    EI_status = IMEStatus::DO_NOTHING;
}

uint8_t CPU::extract_immediate8(const uint8_t* instruction)
{
    if (!instruction) instruction = curr_instr;
    return curr_instr[1];
}

uint16_t CPU::extract_immediate16(const uint8_t* instruction)
{
    if (!instruction) instruction = curr_instr;
    return (static_cast<uint16_t>(instruction[1])) |
           (static_cast<uint16_t>(instruction[2]) << 8);
}

uint64_t CPU::get_cycles()
{
    return clock_cycles;
}

void CPU::reset_cycles()
{
    clock_cycles = 0;
}

void CPU::invalid_opcode()
{
    throw OpcodeError(PC, mem[PC]);
}
