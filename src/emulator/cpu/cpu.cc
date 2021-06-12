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

    current_instruction = nullptr;
    branch_taken = false;
    DI_countdown = NO_COUNTDOWN;
    EI_countdown = NO_COUNTDOWN;
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

    if (EI_countdown > 0)
        --EI_countdown;

    if (interrupts.has_pending_requests())
    {
        auto interrupt = interrupts.next_request();

        if (interrupt.source != Interrupts::NoInterrupt)
        {
            bool was_halted = is_halted;
            is_halted = false;
            if (interrupt.source == Interrupts::JoypadInterrupt)
            {
                // TODO: Does this require an actual
                // interrupt or just a button press?
                is_stopped = false;
            }

            if (reg.read_IME())
            {
                if (was_halted)
                    clock_cycles += 4;
                reg.write_IME(0);
                interrupts.clear_interrupt(interrupt.source);
                jump_to_isr(interrupt.vector_address);
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

    if (DI_countdown == 0)
    {
        reg.write_IME(0);
        DI_countdown = NO_COUNTDOWN;
        EI_countdown = NO_COUNTDOWN;
    }
    else if (EI_countdown == 0)
    {
        reg.write_IME(1);
        DI_countdown = NO_COUNTDOWN;
        EI_countdown = NO_COUNTDOWN;
    }
}

void Cpu::invalid_opcode()
{
    throw OpcodeError(0, *current_instruction);
}

void Cpu::jump_to_isr(addr_t vector_address)
{
    if (vector_address == 0x00) return;

    // cerr << "Jumping to interrupt vector @ " << std::hex << vector_address << endl;
    // trace_log << "Jumping to interrupt vector @ " << std::hex << vector_address << endl;

    reg.write_IME(0);
    PUSH_r16(reg.get_PC());
    reg.write_PC(vector_address);
    current_instruction = mem->get(reg.read_PC());
    DI_countdown = NO_COUNTDOWN;
    EI_countdown = NO_COUNTDOWN;
    clock_cycles += 20;
}
