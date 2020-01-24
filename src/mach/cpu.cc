#include "cpu.hh"

#include "../util/disassembler.hh"
#include <iomanip>
#include <iostream>

Disassembler disassembler;

CPU::CPU(MMU& mmu_, IRC& irc_) :
    mmu(mmu_),
    irc(irc_)
{
    init();
}

void CPU::set_PC(uint16_t value)
{
    PC = value;
}

void CPU::restart()
{
    init();
}

void CPU::init()
{
    reg = {0};
    curr_instr = nullptr;
    branch_taken = false;
    DI_status = IMEStatus::DO_NOTHING;
    EI_status = IMEStatus::DO_NOTHING;
    is_halted = false;
    is_stopped = false;
    clock_cycles = 0;

    // PC = 0x0100;
    set_PC(0x0000);
}

void CPU::execute(const uint8_t* instruction)
{
    if (!instruction) instruction = &mmu.mem[PC];
    curr_instr = instruction;
    branch_taken = false;

    // Nested interrupts are possible if the user has set IME
    // in the handler, so no if statement here.
    if (irc.has_active_requests())
    {
        auto interrupt = irc.accept_next_request();
        if (interrupt.source != IRC::NoInterrupt)
        {
            if (is_halted) is_halted = false;
            if (is_stopped && interrupt.source == IRC::JoypadInterrupt)
                is_stopped = false;
            jump_to_isr(interrupt.vector_address);
            return;
        }
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


    std::cout << "@" << std::setw(5) << std::left << PC << ":   "
              << std::setw(16) << std::left << disassembler.disassemble(const_cast<uint8_t*>(instruction))
              << static_cast<int>(op_info->len_bytes) << " bytes"
              << std::endl;

    // PC has to be incremented before instruction execution.
    PC += op_info->len_bytes;

    if (op_info->handler) (this->*(op_info->handler))();
    else invalid_opcode();

    if (branch_taken)
        clock_cycles += op_info->cycles_on_action;
    else
    {
        clock_cycles += op_info->cycles_on_no_action;
        //PC += op_info->len_bytes;
    }

    if (DI_status == IMEStatus::RESET_THIS_CYCLE) irc.ime_flag_clear();
    else if (EI_status == IMEStatus::SET_THIS_CYCLE) irc.ime_flag_set();
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
    throw OpcodeError(0, *curr_instr);
}
