#include "debugcore.hh"

#include "instructions.hh"

using std::cerr;
using std::cout;
using std::endl;

DebugCore::DebugCore(iEmulator* emulator)
{
    emu_ = emulator;
    emu_->reset_emulation();
}

DebugCore::~DebugCore()
{
    pause();
}

void DebugCore::load_rom(const void* rom, size_t size)
{
    emu_->load_rom(rom, size);
    DebugEvent event(DbRomLoaded);
    for (auto& observer : observers)
        observer->debug_event(&event);
}

int DebugCore::execute_next()
{
    emu_->execute_next();
    return 0;
}

void DebugCore::step_over()
{

}

void DebugCore::step_into()
{
    m_is_running = true;

    DebugEvent continue_event(DbContinued);
    for (auto& observer : observers)
        observer->debug_event(&continue_event);


    execute_next();

    DebugEvent pause_event(DbPaused);
    for (auto& observer : observers)
        observer->debug_event(&pause_event);

    m_is_running = false;
}

void DebugCore::step_out()
{

}

void DebugCore::keep_running()
{
    // To prevent being stuck in the current breakpoint.
    bool is_first_instruction = true;
    while (m_is_running)
    {
        uint8_t instruction_bytes[3];
        regid_t current_pc = read(RegPC);
        for (size_t i = 0; i < 3 && current_pc + i <= MAX_ADDRESS; ++i)
            instruction_bytes[i] = (uint8_t)read(current_pc + i);
        size_t max_len = std::min((size_t)(MAX_ADDRESS - current_pc), (size_t)3);
        Instruction current_instr(instruction_bytes, max_len);

        current_instr.disassembly();
        auto& mem_writes(current_instr.write_addresses(this));
        auto& mem_reads(current_instr.read_addresses(this));
        // TODO: Register breakpoints.
        auto& reg_writes(current_instr.opcode()->write_regs);
        auto& reg_reads(current_instr.opcode()->read_regs);
        auto writes_flags(current_instr.opcode()->write_flags);
        auto reads_flags(current_instr.opcode()->read_flags);

        if (current_pc == 0x0214)
        {
            for (auto& address : mem_reads)
            {
                cerr << std::hex << address << endl;
            }
        }

        bool do_execute = true;

        // Don't stop at the first breakpoint if it's where the run started.
        if (!is_first_instruction)
        {
            auto bp = breakpoint_at(read(RegPC));
            if (bp && bp->has_mode(BpOnExec))
                do_execute = false;

            if (do_execute)
            {
                for (auto& address : mem_writes)
                {
                    bp = breakpoint_at(address);
                    if (bp && bp->has_mode(BpOnWrite))
                    {
                        do_execute = false;
                        break;
                    }
                }
            }

            if (do_execute)
            {
                for (auto& address : mem_reads)
                {
                    bp = breakpoint_at(address);
                    if (bp && bp->has_mode(BpOnRead))
                    {
                        do_execute = false;
                        break;
                    }
                }
            }
        }

        if (!do_execute) break;


        execute_next();
        is_first_instruction = false;
    }

    DebugEvent pause_event(DbPaused);
    for (auto& observer : observers)
        observer->debug_event(&pause_event);
}

void DebugCore::notify_observers(const DebugEvent* event)
{
    for (auto& observer : observers)
        observer->debug_event(event);
}

void DebugCore::run()
{
    DebugEvent continue_event(DbContinued);
    for (auto& observer : observers)
        observer->debug_event(&continue_event);

    if (emulation_thread && emulation_thread->joinable())
        emulation_thread->join();
    delete emulation_thread;
    emulation_thread = nullptr;
    m_is_running = true;
    emulation_thread = new std::thread([&](){ keep_running(); });
}

void DebugCore::pause()
{
    m_is_running = false;
    if (emulation_thread)
    {
        if (emulation_thread->joinable()) emulation_thread->join();
        delete emulation_thread;
        emulation_thread = nullptr;
    }

    DebugEvent pause_event(DbPaused);
    for (auto& observer : observers)
        observer->debug_event(&pause_event);
}

void DebugCore::toggle_breakpoint(regid_t register_id, BpMode mode)
{
    if (m_breakpoints.count(register_id) != 0)
    {
        auto bp = m_breakpoints[register_id];
        bp->toggle_mode(mode);

        if (bp->has_mode(BpNone))
        {
            m_breakpoints.erase(register_id);
            DebugEvent bp_remove_event(DbBreakpointRemoved,
                                       register_id);
            notify_observers(&bp_remove_event);
            delete bp;
        }
        else
        {
            DebugEvent bp_modify_event(DbBreakpointModified,
                                       register_id);
            notify_observers(&bp_modify_event);
        }
    }
    else
    {
        auto new_bp = new Breakpoint(register_id, {mode});
        m_breakpoints[register_id] = new_bp;
        DebugEvent bp_add_event(DbBreakpointAdded, register_id);
        notify_observers(&bp_add_event);
    }
}

Breakpoint* DebugCore::breakpoint_at(regid_t register_id)
{
    auto bp_it = m_breakpoints.find(register_id);
    if (bp_it != m_breakpoints.end())
        return bp_it->second;
    return nullptr;
}

void DebugCore::add_observer(iDebugObserver* observer)
{
    observers.push_back(observer);
}

void DebugCore::button_pressed(JoypadButton button)
{
    emu_->button_pressed(button);
}

void DebugCore::button_released(JoypadButton button)
{
    emu_->button_released(button);
}

void DebugCore::reset_emulation()
{

}

void DebugCore::set_render_callback(void (*callback)(const framebuf_t*, int))
{

}

bool DebugCore::is_running()
{
    return m_is_running;
}
