#include "cpu.hh"

#include "emulator/memory/memorybus.hh"
#include "emulator/peripherals/timer.hh"
#include <array>
#include <iomanip>
#include <iostream>

Cpu::Cpu(
    CpuRegisters& registers,
    Interrupts& interrupts,
    MemoryBus& mem,
    Timer& timer
)
    : reg_(registers)
    , interrupts_(interrupts)
    , memory_(mem)
    , timer_(timer)
    , current_instruction_(nullptr)
    , branch_was_taken_(false)
    , is_halted_(false)
    , is_stopped_(false)
    , elapsed_tcycles_(0)
{
    trace_log_.open("trace.log", std::ofstream::out);
    post_bootram_reset();
}

CpuRegisters& Cpu::get_registers()
{
    return reg_;
}

Cpu::~Cpu() = default;

void Cpu::post_bootram_reset()
{
    reg_ = CpuRegisters();
    reg_.post_bootram_reset();
    interrupts_.post_bootram_reset();

    current_instruction_ = nullptr;
    branch_was_taken_ = false;

    is_halted_ = false;
    is_stopped_ = false;
    elapsed_tcycles_ = 0;
}

void Cpu::execute_next()
{
    // TODO: Is the interrupt handling correct?

    if (interrupts_.has_pending_requests())
    {
        auto& interrupt = interrupts_.get_top_priority_request();
        if (interrupt.source != Interrupts::None)
        {
            if (interrupt.source == Interrupts::Joypad)
            {
                is_stopped_ = false;
                timer_.set_stopped(false);
            }

            const bool was_halted = is_halted_;
            is_halted_ = false;

            if (interrupts_.interrupts_enabled())
            {
                if (was_halted)
                {
                    // TODO: Where does this come from?
                    elapsed_tcycles_ += 4;
                }

                jump_to_interrupt_handler(interrupt);
                return;
            }
        }
    }

    if (!is_halted_ && !is_stopped_)
    {
        current_instruction_ = memory_.get(reg_.read_PC());
        assert(current_instruction_ != nullptr);
        branch_was_taken_ = false;

        const OpcodeInfo& opcode_info =
            (current_instruction_[0] != 0xCB)
            ? OPCODE_TABLE.at(current_instruction_[0])
            : CB_OPCODE_TABLE.at(current_instruction_[1]);

        reg_.write_PC(reg_.read_PC() + opcode_info.len_bytes);

        dispatch(current_instruction_);

        elapsed_tcycles_ += branch_was_taken_
            ? opcode_info.tcycles_if_branch_taken
            : opcode_info.tcycles_if_branch_not_taken;
    }
    else
    {
        // Halted or stopped, so advance time by one machine cycle.
        elapsed_tcycles_ += 4;
    }
}

void Cpu::invalid_opcode()
{
    throw OpcodeError(0, *current_instruction_);
}

void Cpu::jump_to_interrupt_handler(const Interrupts::Irq& irq)
{
    interrupts_.clear_interrupt(irq.source);
    interrupts_.disable_interrupts_now();
    CALL_n16(irq.handler_address);
    current_instruction_ = memory_.get(reg_.read_PC());
    elapsed_tcycles_ += 20;
}
