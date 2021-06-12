#include "cpu.hh"

#include "emulator/memory/memorybus.hh"
#include <array>
#include <iomanip>
#include <iostream>

Cpu::Cpu(CpuRegisters& registers, Interrupts& interrupts, MemoryBus* mem)
    : reg(registers)
    , interrupts(interrupts)
    , mem(mem)
{
    trace_log.open("trace.log", std::ofstream::out);
    hard_reset();
}

Cpu::~Cpu() = default;

void Cpu::set_PC(uint16_t value)
{
    reg.write_PC(value);
}

void Cpu::restart()
{
    hard_reset();
}

void Cpu::hard_reset()
{
    reg = CpuRegisters();
    reg.post_bootram_reset();
    interrupts.hard_reset();

    current_instruction = nullptr;
    branch_taken = false;

    is_halted = false;
    is_stopped = false;
    clock_cycles = 0;

}

void Cpu::execute(const uint8_t* instruction)
{
    if (!instruction) instruction = mem->get(reg.read_PC());
    assert(instruction);
    current_instruction = instruction;
    branch_taken = false;

    interrupts.pre_instruction_execute();

    if (interrupts.has_pending_requests())
    {
        auto interrupt = interrupts.next_request();
        if (interrupt.source != Interrupts::NoInterrupt)
        {
            bool was_halted = is_halted;
            is_halted = false;

            switch (interrupt.source)
            {
                // TODO: Is this correct?
                case Interrupts::JoypadInterrupt:
                    is_stopped = false;
                    break;
                default:
                    break;
            }

            if (reg.read_IME())
            {
                if (was_halted)
                {
                    clock_cycles += 4;
                }

                jump_to_interrupt_handler(interrupt);
                return;
            }
        }
    }

    if (!is_halted && !is_stopped)
    {
        const InstructionInfo* op_info = (*current_instruction == 0xCB) ?
                                    &CB_OPCODE_TABLE[current_instruction[1]] :
                                    &OPCODE_TABLE[*current_instruction];

        reg.write_PC(reg.read_PC() + op_info->len_bytes);

        decode_and_dispatch(current_instruction);

        if (branch_taken)
            clock_cycles += op_info->cycles_on_action;
        else
            clock_cycles += op_info->cycles_on_no_action;
    }
    else
    {
        // Halted or stopped, so advance time by one machine cycle.
        clock_cycles += 4;
    }

    interrupts.post_instruction_execute();
}

void Cpu::invalid_opcode()
{
    throw OpcodeError(0, *current_instruction);
}

void Cpu::jump_to_interrupt_handler(const Interrupts::InterruptInfo& interrupt)
{
    interrupts.clear_interrupt(interrupt.source);
    interrupts.disable_interrupts_now();
    PUSH_r16(reg.get_PC());
    reg.write_PC(interrupt.handler_address);
    current_instruction = mem->get(reg.read_PC());
    clock_cycles += 20;
}
