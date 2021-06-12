#pragma once

#include "cpuregisters.hh"
#include "emulator/common/macros.hh"
#include "emulator/common/types.hh"

class Interrupts
{
public:
    enum IrqSource
    {
        VBlank  = 0x00,
        LcdStat = 0x01,
        Timer   = 0x02,
        Serial  = 0x03,
        Joypad  = 0x04,
        None    = 0xFF
    };

    struct Irq
    {
        IrqSource source;
        addr_t handler_address;
    };

    Interrupts(CpuRegisters& registers);

    inline void post_bootram_reset();

    inline void pre_instruction_execute();
    inline void post_instruction_execute();

    inline bool has_pending_requests();
    inline const Interrupts::Irq& next_request();

    inline void request_interrupt(int source);
    inline bool interrupt_requested(int source);
    inline bool interrupt_enabled(int source);
    inline void clear_interrupt(int source);
    inline void disable_interrupt(int source);
    inline void enable_interrupt(int source);

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

    const Irq VBLANK_IRQ = {VBlank, 0x0040};
    const Irq LCDSTAT_IRQ = {LcdStat, 0x0048};
    const Irq TIMER_IRQ = {Timer, 0x0050};
    const Irq SERIAL_IRQ = {Serial, 0x0058};
    const Irq JOYPAD_IRQ = {Joypad, 0x0060};
    const Irq NONE_IRQ = {None, 0x0000};

    static constexpr int NO_COUNTDOWN = -1;
    int DI_countdown_ = NO_COUNTDOWN;
    int EI_countdown_ = NO_COUNTDOWN;
};

FORCE_INLINE void Interrupts::post_bootram_reset()
{
    DI_countdown_ = NO_COUNTDOWN;
    EI_countdown_ = NO_COUNTDOWN;
}

FORCE_INLINE void Interrupts::pre_instruction_execute()
{
    if (DI_countdown_ != NO_COUNTDOWN)
    {
        --DI_countdown_;
    }
    if (EI_countdown_ != NO_COUNTDOWN)
    {
        --EI_countdown_;
    }
}

FORCE_INLINE void Interrupts::post_instruction_execute()
{
    if (EI_countdown_ == 0)
    {
        DI_countdown_ = NO_COUNTDOWN;
        EI_countdown_ = NO_COUNTDOWN;
        reg_.write_IME(1);
    }
    else if (DI_countdown_ == 0)
    {
        DI_countdown_ = NO_COUNTDOWN;
        EI_countdown_ = NO_COUNTDOWN;
        reg_.write_IME(0);
    }
}

FORCE_INLINE bool Interrupts::has_pending_requests()
{
    return (reg_.read_IF() & 0x1F) != 0;
}

FORCE_INLINE const Interrupts::Irq& Interrupts::next_request()
{
    for (IrqSource i = VBlank;
         i <= Joypad;
         i = static_cast<IrqSource>(static_cast<int>(i) + 1))
    {
        if (interrupt_enabled(i) && interrupt_requested(i))
        {
            switch (i)
            {
                case VBlank:
                    return VBLANK_IRQ;
                case LcdStat:
                    return LCDSTAT_IRQ;
                case Timer:
                    return TIMER_IRQ;
                case Serial:
                    return SERIAL_IRQ;
                case Joypad:
                    return JOYPAD_IRQ;
                case None:
                    break;
            }
        }
    }

    return NONE_IRQ;
}

FORCE_INLINE void Interrupts::request_interrupt(int source)
{
    switch (source)
    {
    case VBlank: ++vblank_irq_count_; break;
    case LcdStat: ++lcdstat_irq_count_; break;
    case Joypad: ++joypad_irq_count_; break;
    case Timer: ++timer_irq_count_; break;
    case Serial: ++serial_irq_count_; break;
    }

    reg_.write_IF(reg_.read_IF() | (0x01 << source));
}

FORCE_INLINE bool Interrupts::interrupt_requested(int source)
{
    return (reg_.read_IF() & (0x01 << source)) != 0;
}

FORCE_INLINE bool Interrupts::interrupt_enabled(int source)
{
    return (reg_.read_IE() & (0x01 << source)) != 0;
}

FORCE_INLINE void Interrupts::clear_interrupt(int source)
{
    reg_.write_IF(reg_.read_IF() & ~(0x01 << source));
}

FORCE_INLINE void Interrupts::disable_interrupt(int source)
{
    reg_.write_IE(reg_.read_IE() & ~(0x01 << source));
}

FORCE_INLINE void Interrupts::enable_interrupt(int source)
{
    reg_.write_IE(reg_.read_IE() | (0x01 << source));
}

FORCE_INLINE void Interrupts::enable_interrupts_after(int delay)
{
    DI_countdown_ = NO_COUNTDOWN;
    EI_countdown_ = delay;
}

FORCE_INLINE void Interrupts::enable_interrupts_now()
{
    DI_countdown_ = NO_COUNTDOWN;
    EI_countdown_ = NO_COUNTDOWN;
    reg_.write_IME(1);
}

FORCE_INLINE void Interrupts::disable_interrupts_now()
{
    DI_countdown_ = NO_COUNTDOWN;
    EI_countdown_ = NO_COUNTDOWN;
    reg_.write_IME(0);
}
