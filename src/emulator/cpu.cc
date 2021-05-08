#include "cpu.hh"

#include "../debugger/disassembler.hh"
#include "memory.hh"
#include <iomanip>
#include <iostream>

static Disassembler disassembler;

Cpu::Cpu(Memory* memory, Irc* irc_) :
    mem(memory),
    irc(irc_)
{
    trace_log.open("trace.log", std::ofstream::out);
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
    AF = 0x01B0;
    BC = 0x0013;
    DE = 0x00D8;
    HL = 0x014D;
    SP = 0xFFFE;
    PC = 0x0100;

    curr_instr = nullptr;
    branch_taken = false;
    DI_action = IMEStatus::DO_NOTHING;
    EI_action = IMEStatus::DO_NOTHING;
    is_halted = false;
    is_stopped = false;
    clock_cycles = 0;
}

void Cpu::execute(const uint8_t* instruction)
{
    if (!instruction) instruction = mem->get(PC);
    assert(instruction);
    curr_instr = instruction;
    branch_taken = false;

    if (EI_action == IMEStatus::SET_NEXT_CYCLE)
        EI_action = IMEStatus::SET_THIS_CYCLE;

    if (irc->has_pending_requests())
    {
        auto interrupt = irc->next_request();

        if (interrupt.source != Irc::NoInterrupt)
        {
            bool was_halted = is_halted;
            is_halted = false;
            if (interrupt.source == Irc::JoypadInterrupt)
            {
                // TODO: Does this require an actual
                // interrupt or just a button press?
                is_stopped = false;
            }

            if (irc->ime_flag_get())
            {
                if (was_halted) clock_cycles += 4;
                irc->ime_flag_clear();
                irc->clear_interrupt(interrupt.source);
                jump_to_isr(interrupt.vector_address);
                return;
            }
        }
    }

    if (!is_halted && !is_stopped)
    {
        const InstrInfo* op_info = (*curr_instr == 0xCB) ?
                                    &CB_INSTR_TABLE[curr_instr[1]] :
                                    &INSTR_TABLE[*curr_instr];

        PC += op_info->len_bytes;

        if (op_info->handler) (this->*(op_info->handler))();
        else invalid_opcode();

        if (branch_taken)
            clock_cycles += op_info->cycles_on_action;
        else
        {
            clock_cycles += op_info->cycles_on_no_action;
        }

    }
    else
    {
        // Halted or stopped, so advance time by one machine cycle.
        clock_cycles += 4;
    }

    if (DI_action == IMEStatus::RESET_THIS_CYCLE)
    {
        irc->ime_flag_clear();
        DI_action = IMEStatus::DO_NOTHING;
    }
    else if (EI_action == IMEStatus::SET_THIS_CYCLE)
    {
        irc->ime_flag_set();
        DI_action = IMEStatus::DO_NOTHING;
    }
}

void Cpu::invalid_opcode()
{
    throw OpcodeError(0, *curr_instr);
}

void Cpu::jump_to_isr(memaddr_t vector_address)
{
    if (vector_address == 0x00) return;

    // cerr << "Jumping to interrupt vector @ " << std::hex << vector_address << endl;
    // trace_log << "Jumping to interrupt vector @ " << std::hex << vector_address << endl;

    irc->ime_flag_clear();
    PUSH_r16(PC);
    PC = vector_address;
    curr_instr = mem->get(PC);
    DI_action = IMEStatus::DO_NOTHING;
    EI_action = IMEStatus::DO_NOTHING;
    clock_cycles += 20;
}
