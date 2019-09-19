#include "cpu.hh"

CPU::CPU()
{
    CPU_init();
}

void CPU::restart()
{
    CPU_init();
}

void CPU::CPU_init()
{
    mem = {0};
    reg = {0};
    IME_flag = 0x00;
    mem[HWREG_IF_ADDR] = 0x00;
    mem[HWREG_IF_ADDR] = 0x00;
    PC = 0x0100;
    SP = 0xFFFE;
    curr_instr = nullptr;
    curr_interrupt = nullptr;
    branch_taken = false;
    DI_status = IMEStatus::DO_NOTHING;
    EI_status = IMEStatus::DO_NOTHING;
    is_halted = false;
    is_stopped = false;
    clock_cycles = 0;
    is_interrupted = false;
}

void CPU::execute(const uint8_t* instruction)
{
    if (!instruction) instruction = &mem[PC];
    curr_instr = instruction;
    branch_taken = false;

    // Nested interrupts are possible if the user has set IME
    // in the handler, so no if statement here.
    const IntInfo* int_info = check_interrupts();
    if (int_info)
    {
        if (is_halted) is_halted = false;
        if (is_stopped && int_info->id == IntID::KEYPAD)
            is_stopped = false;
        curr_interrupt = int_info;
        handle_interrupt(curr_interrupt);
    }

    if (is_halted) return;
    if (is_stopped) return;

    if (DI_status == IMEStatus::RESET_NEXT_CYCLE)
        DI_status = IMEStatus::RESET_THIS_CYCLE;
    else if (EI_status == IMEStatus::SET_NEXT_CYCLE)
        EI_status = IMEStatus::SET_THIS_CYCLE;


    const InstrInfo* op_info = (*curr_instr == 0xCB) ?
                                &CB_INSTR_TABLE[curr_instr[1]] :
                                &INSTR_TABLE[*curr_instr];

    if (op_info->handler) (this->*(op_info->handler))();
    else invalid_opcode();

    if (branch_taken)
        clock_cycles += op_info->cycles_on_action;
    else
    {
        clock_cycles += op_info->cycles_on_no_action;
        // Only increment PC if did not branch.
        PC += op_info->len_bytes;
    }

    if (DI_status == IMEStatus::RESET_THIS_CYCLE) disable_interrupts_now();
    else if (EI_status == IMEStatus::SET_THIS_CYCLE) enable_interrupts_now();
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
