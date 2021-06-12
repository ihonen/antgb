#pragma once

#include "cpuregisters.hh"
#include "emulator/common/macros.hh"
#include "emulator/common/types.hh"

class Interrupts
{
public:
    enum Source
    {
        VBlank  = 0,
        LcdStat = 1,
        Timer   = 2,
        Serial  = 3,
        Joypad  = 4,
        None,
    };

    struct Irq
    {
        Source source;
        addr_t handler_address;
    };

    Interrupts(CpuRegisters& registers);

    void post_bootram_reset();

    inline void pre_instruction_exec_tick();
    inline void post_instruction_exec_tick();

    inline bool has_pending_requests();
    const Interrupts::Irq& next_request();

    void request_interrupt(Source source);
    inline bool interrupt_requested(Source source);
    inline bool interrupt_enabled(Source source);
    inline void clear_interrupt(Source source);
    inline void disable_interrupt(Source source);
    inline void enable_interrupt(Source source);

    inline void enable_interrupts_after(int delay);
    inline void enable_interrupts_now();
    inline void disable_interrupts_now();

protected:
    CpuRegisters& reg_;

    uint64_t vblank_irq_count_  = 0;
    uint64_t lcdstat_irq_count_ = 0;
    uint64_t timer_irq_count_   = 0;
    uint64_t serial_irq_count_  = 0;
    uint64_t joypad_irq_count_  = 0;

    const Irq VBLANK_IRQ  = {VBlank,  0x0040};
    const Irq LCDSTAT_IRQ = {LcdStat, 0x0048};
    const Irq TIMER_IRQ   = {Timer,   0x0050};
    const Irq SERIAL_IRQ  = {Serial,  0x0058};
    const Irq JOYPAD_IRQ  = {Joypad,  0x0060};
    const Irq NONE_IRQ    = {None,    0x0000};

    static constexpr int EXPIRED = -1;
    int enable_delay_left_;
    int disable_delay_left_;
};

FORCE_INLINE void Interrupts::pre_instruction_exec_tick()
{
    if (disable_delay_left_ != EXPIRED)
    {
        --disable_delay_left_;
    }
    if (enable_delay_left_ != EXPIRED)
    {
        --enable_delay_left_;
    }
}

FORCE_INLINE void Interrupts::post_instruction_exec_tick()
{
    if (enable_delay_left_ == 0)
    {
        disable_delay_left_ = EXPIRED;
        enable_delay_left_ = EXPIRED;
        reg_.write_IME(1);
    }
    else if (disable_delay_left_ == 0)
    {
        disable_delay_left_ = EXPIRED;
        enable_delay_left_ = EXPIRED;
        reg_.write_IME(0);
    }
}

FORCE_INLINE bool Interrupts::has_pending_requests()
{
    return bool(reg_.read_IF());
}

FORCE_INLINE bool Interrupts::interrupt_requested(Source source)
{
    return bool((reg_.read_IF() & (1 << source)));
}

FORCE_INLINE bool Interrupts::interrupt_enabled(Source source)
{
    return bool((reg_.read_IE() & (1 << source)));
}

FORCE_INLINE void Interrupts::clear_interrupt(Source source)
{
    reg_.write_IF(reg_.read_IF() & ~(1 << source));
}

FORCE_INLINE void Interrupts::disable_interrupt(Source source)
{
    reg_.write_IE(reg_.read_IE() & ~(1 << source));
}

FORCE_INLINE void Interrupts::enable_interrupt(Source source)
{
    reg_.write_IE(reg_.read_IE() | static_cast<uint8_t>((1 << source)));
}

FORCE_INLINE void Interrupts::enable_interrupts_after(int delay)
{
    disable_delay_left_ = EXPIRED;
    enable_delay_left_ = delay;
}

FORCE_INLINE void Interrupts::enable_interrupts_now()
{
    disable_delay_left_ = EXPIRED;
    enable_delay_left_ = EXPIRED;
    reg_.write_IME(1);
}

FORCE_INLINE void Interrupts::disable_interrupts_now()
{
    disable_delay_left_ = EXPIRED;
    enable_delay_left_ = EXPIRED;
    reg_.write_IME(0);
}
