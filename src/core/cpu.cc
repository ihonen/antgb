#include "cpu.hh"

#include "../debugger/disassembler.hh"
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
    regs.reg = {0};
    curr_instr = nullptr;
    branch_taken = false;
    DI_action = IMEStatus::DO_NOTHING;
    EI_action = IMEStatus::DO_NOTHING;
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

        static bool do_print = false;

        do_print = true;

        if (do_print)
        {
            trace_log << "\n";
            trace_log << "AF: " << std::hex << AF << "\n";
            trace_log << "BC: " << std::hex << BC << "\n";
            trace_log << "DE: " << std::hex << DE << "\n";
            trace_log << "HL: " << std::hex << HL << "\n";
            trace_log << "SP: " << std::hex << SP << "\n";
            trace_log << "PC: " << std::hex << PC << "\n";

        trace_log << "@"
                  << std::setw(5) << std::left << std::hex
                  << PC
                  << disassembler.disassemble(const_cast<uint8_t*>(instruction))
                  << "\n";
        trace_log << std::flush;
        /*
            */

            /*
            cout      << "@"
                      << std::setw(5) << std::left << std::hex
                      << PC
                      << disassembler.disassemble(const_cast<uint8_t*>(instruction))
                      << std::endl;
            */


            /*
            */
        }

        do_print = false;

        if (PC == 0x06f1)
        {
            volatile int a = 0;
        }

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

    trace_log << "Jumping to interrupt vector @ " << std::hex << vector_address << endl;

    irc->ime_flag_clear();
    PUSH_r16(PC);
    PC = vector_address;
    curr_instr = mem->get(PC);
    DI_action = IMEStatus::DO_NOTHING;
    EI_action = IMEStatus::DO_NOTHING;
    clock_cycles += 20;
}
