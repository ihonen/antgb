#include "cpu.hh"

GBMachine::CPU::CPU()
{
    init();
}

void GBMachine::CPU::restart()
{
    init();
}

void GBMachine::CPU::init()
{
    mem = {0};
    reg = {0};
    curr_instr = nullptr;
    curr_interrupt = nullptr;
    branch_taken = false;
    DI_status = IMEStatus::DO_NOTHING;
    EI_status = IMEStatus::DO_NOTHING;
    is_halted = false;
    is_stopped = false;
    clock_cycles = 0;
    is_interrupted = false;

    IME_flag = 0x00;
    mem[HWREG_IF_ADDR] = 0x00;
    mem[HWREG_IF_ADDR] = 0x00;
    AF = 0x01B0;
    BC = 0x0013;
    DE = 0x00D8;
    HL = 0x014D;
    SP = 0xFFFE;
    mem[0xFF05] = 0x00; // TIMA
    mem[0xFF06] = 0x00; // TMA
    mem[0xFF07] = 0x00; // TAC
    mem[0xFF10] = 0x80; // NR10
    mem[0xFF11] = 0xBF; // NR11
    mem[0xFF12] = 0xF3; // NR12
    mem[0xFF14] = 0xBF; // NR14
    mem[0xFF16] = 0x3F; // NR21
    mem[0xFF17] = 0x00; // NR22
    mem[0xFF19] = 0xBF; // NR24
    mem[0xFF1A] = 0x7F; // NR30
    mem[0xFF1B] = 0xFF; // NR31
    mem[0xFF1C] = 0x9F; // NR32
    mem[0xFF1E] = 0xBF; // NR33
    mem[0xFF20] = 0xFF; // NR41
    mem[0xFF21] = 0x00; // NR42
    mem[0xFF22] = 0x00; // NR43
    mem[0xFF23] = 0xBF; // NR44
    mem[0xFF24] = 0x77; // NR50
    mem[0xFF25] = 0xF3; // NR51
    mem[0xFF26] = 0xF1; // NR52
    mem[0xFF40] = 0x91; // LCDC
    mem[0xFF42] = 0x00; // SCY
    mem[0xFF43] = 0x00; // SCX
    mem[0xFF45] = 0x00; // LYC
    mem[0xFF47] = 0xFC; // BGP
    mem[0xFF48] = 0xFF; // OBP0
    mem[0xFF49] = 0xFF; // OBP1
    mem[0xFF4A] = 0x00; // WY
    mem[0xFF4B] = 0x00; // WX
    mem[0xFFFF] = 0x00; // IE

    PC = 0x0100;
}

void GBMachine::CPU::execute(const uint8_t* instruction)
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

uint8_t GBMachine::CPU::extract_immediate8(const uint8_t* instruction)
{
    if (!instruction) instruction = curr_instr;
    return curr_instr[1];
}

uint16_t GBMachine::CPU::extract_immediate16(const uint8_t* instruction)
{
    if (!instruction) instruction = curr_instr;
    return (static_cast<uint16_t>(instruction[1])) |
           (static_cast<uint16_t>(instruction[2]) << 8);
}

uint64_t GBMachine::CPU::get_cycles()
{
    return clock_cycles;
}

void GBMachine::CPU::reset_cycles()
{
    clock_cycles = 0;
}

void GBMachine::CPU::invalid_opcode()
{
    throw OpcodeError(PC, mem[PC]);
}
