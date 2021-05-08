#pragma once

#include "disassembler.hh"
#include "idebugobserver.hh"
#include "iemulator.hh"
#include "emulator/emulator.hh"
#include "emulator/cpu.hh"
#include "emulator/types.hh"
#include "util/macros.hh"
#include <unordered_map>

#include <set>
#include <thread>

class DebugCore : public iEmulator
{
public:
    DebugCore(iEmulator* emu);
    virtual ~DebugCore() override;
    virtual void load_rom(const void* rom, size_t size) override;
    virtual int execute_next() override;
    void step_over();
    void step_into();
    void step_out();
    void run();
    void pause();
    void toggle_breakpoint(regid_t register_id, BpMode mode);
    Breakpoint* breakpoint_at(regid_t register_id);
    void add_observer(iDebugObserver* observer);
    virtual void button_pressed(JoypadButton button) override;
    virtual void button_released(JoypadButton button) override;
    iEmulator* emu() { return emu_; }
    Disassembler* disasm() { return &disassembler; }
    const std::unordered_map<regid_t, Breakpoint*>& breakpoints() { return m_breakpoints; }
    virtual inline uint16_t read(regid_t register_id) override;
    virtual inline void write(regid_t register_id, uint16_t value) override;
    virtual void reset_emulation() override;
    virtual void set_render_callback(void (*callback)(const framebuf_t *, int)) override;
    bool is_running();
protected:
    void keep_running();
    void notify_observers(const DebugEvent* event);

    iEmulator* emu_;
    Disassembler disassembler;
    std::unordered_map<regid_t, Breakpoint*> m_breakpoints;
    std::vector<iDebugObserver*> observers;
    std::thread* emulation_thread = nullptr;
    bool m_is_running = false;
};

FORCE_INLINE uint16_t DebugCore::read(regid_t register_id)
{
    return emu_->read(register_id);
}

FORCE_INLINE void DebugCore::write(regid_t register_id, uint16_t value)
{
    return emu_->write(register_id, value);
}
