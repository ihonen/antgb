#pragma once

#include "cpuregisters.hh"
#include "emulator/common/macros.hh"
#include "emulator/common/types.hh"

class Interrupts
{
public:
    enum InterruptId
    {
        VBlankInterrupt = 0x00,
        LcdStatInt      = 0x01,
        TimerInterrupt  = 0x02,
        SerialInterrupt = 0x03,
        JoypadInterrupt = 0x04,
        NoInterrupt     = 0xFF
    };

    typedef struct
    {
        InterruptId source;
        addr_t handler_address;
    } InterruptInfo;

    const addr_t INTERRUPT_VECTOR[5] = {0x0040, 0x0048, 0x0050, 0x0058, 0x0060};

    Interrupts(CpuRegisters& registers);
    inline void hard_reset();
    inline void pre_instruction_execute();
    inline void post_instruction_execute();
    inline bool has_pending_requests();
    inline InterruptInfo next_request();
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
    uint64_t vblank_irqs  = 0;
    uint64_t lcdstat_irqs = 0;
    uint64_t timer_irqs   = 0;
    uint64_t serial_irqs  = 0;
    uint64_t joypad_irqs  = 0;

    static constexpr int NO_COUNTDOWN = -1;
    int DI_countdown = NO_COUNTDOWN;
    int EI_countdown = NO_COUNTDOWN;
};

FORCE_INLINE void Interrupts::hard_reset()
{
    DI_countdown = NO_COUNTDOWN;
    EI_countdown = NO_COUNTDOWN;
}

FORCE_INLINE void Interrupts::pre_instruction_execute()
{
    if (DI_countdown != NO_COUNTDOWN)
    {
        --DI_countdown;
    }
    if (EI_countdown != NO_COUNTDOWN)
    {
        --EI_countdown;
    }
}

FORCE_INLINE void Interrupts::post_instruction_execute()
{
    if (EI_countdown == 0)
    {
        DI_countdown = NO_COUNTDOWN;
        EI_countdown = NO_COUNTDOWN;
        reg_.write_IME(1);
    }
    else if (DI_countdown == 0)
    {
        DI_countdown = NO_COUNTDOWN;
        EI_countdown = NO_COUNTDOWN;
        reg_.write_IME(0);
    }
}

FORCE_INLINE bool Interrupts::has_pending_requests()
{
    return (reg_.read_IF() & 0x1F) != 0;
}

FORCE_INLINE Interrupts::InterruptInfo Interrupts::next_request()
{
    for (InterruptId i = VBlankInterrupt; i < JoypadInterrupt;
         i             = (InterruptId)((int)i + 1))
    {
        if (interrupt_enabled(i) && interrupt_requested(i))
        {
            return {(InterruptId)i, INTERRUPT_VECTOR[i]};
        }
    }

    return {NoInterrupt, 0x0000};
}

FORCE_INLINE void Interrupts::request_interrupt(int source)
{
    switch (source)
    {
    case VBlankInterrupt: ++vblank_irqs; break;
    case LcdStatInt: ++lcdstat_irqs; break;
    case JoypadInterrupt: ++joypad_irqs; break;
    case TimerInterrupt: ++timer_irqs; break;
    case SerialInterrupt: ++serial_irqs; break;
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
    DI_countdown = NO_COUNTDOWN;
    EI_countdown = delay;
}

FORCE_INLINE void Interrupts::enable_interrupts_now()
{
    DI_countdown = NO_COUNTDOWN;
    EI_countdown = NO_COUNTDOWN;
    reg_.write_IME(1);
}

FORCE_INLINE void Interrupts::disable_interrupts_now()
{
    DI_countdown = NO_COUNTDOWN;
    EI_countdown = NO_COUNTDOWN;
    reg_.write_IME(0);
}
