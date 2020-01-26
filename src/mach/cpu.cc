#include "cpu.hh"

#include "../util/disassembler.hh"
#include <iomanip>
#include <iostream>

Disassembler disassembler;

Cpu::Cpu(Memory* memory, InterruptController* irc_) :
    mem(memory),
    irc(irc_)
{
    hard_reset();
}

void Cpu::set_PC(uint16_t value)
{
    PC = value;
}

void Cpu::restart()
{
    hard_reset();
}

void Cpu::hard_reset()
{
    reg = {0};
    curr_instr = nullptr;
    branch_taken = false;
    DI_status = IMEStatus::DO_NOTHING;
    EI_status = IMEStatus::DO_NOTHING;
    is_halted = false;
    is_stopped = false;
    clock_cycles = 0;

    AF = 0x01B0;
    BC = 0x0013;
    DE = 0x00D8;
    HL = 0x014D;
    SP = 0xFFFE;
    PC = 0x0100;
}

void Cpu::execute(const uint8_t* instruction)
{
    if (!instruction) instruction = mem->get(PC);
    curr_instr = instruction;
    branch_taken = false;

    // Nested interrupts are possible if the user has set IME
    // in the handler, so no if statement here.
    if (irc->has_active_requests())
    {
        auto interrupt = irc->accept_next_request();
        if (interrupt.source != InterruptController::NoInterrupt)
        {
            if (is_halted) is_halted = false;
            if (is_stopped && interrupt.source == InterruptController::JoypadInterrupt)
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

    std::cout << "@"
              << std::setw(5) << std::left << std::hex
              << PC
              << disassembler.disassemble(const_cast<uint8_t*>(instruction))
              << std::endl;

    PC += op_info->len_bytes;

    if (op_info->handler) (this->*(op_info->handler))();
    else invalid_opcode();

    if (branch_taken)
        clock_cycles += op_info->cycles_on_action;
    else
    {
        clock_cycles += op_info->cycles_on_no_action;
    }

    if (DI_status == IMEStatus::RESET_THIS_CYCLE) irc->ime_flag_clear();
    else if (EI_status == IMEStatus::SET_THIS_CYCLE) irc->ime_flag_set();
}

uint8_t Cpu::extract_immediate8(const uint8_t* instruction)
{
    if (!instruction) instruction = curr_instr;
    return curr_instr[1];
}

uint16_t Cpu::extract_immediate16(const uint8_t* instruction)
{
    if (!instruction) instruction = curr_instr;
    return (static_cast<uint16_t>(instruction[1])) |
           (static_cast<uint16_t>(instruction[2]) << 8);
}

uint64_t Cpu::get_cycles()
{
    return clock_cycles;
}

void Cpu::reset_cycles()
{
    clock_cycles = 0;
}

void Cpu::invalid_opcode()
{
    throw OpcodeError(0, *curr_instr);
}

void Cpu::jump_to_isr(memaddr_t vector_address)
{
    if (vector_address == 0x00) return;

    cout << "Jumping to interrupt vector @ " << vector_address << endl;

    is_interrupted = true;
    irc->ime_flag_clear();
    PUSH_r16(PC);
    PC = vector_address;
    curr_instr = mem->get(PC);
    DI_status = IMEStatus::DO_NOTHING;
    EI_status = IMEStatus::DO_NOTHING;
    clock_cycles += 20;
}
