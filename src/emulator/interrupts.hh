#pragma once

#include "types.hh"
#include "util/macros.hh"

class Irc
{
public:
    struct Registers
    {
        uint8_t IE;
        uint8_t IF;
    };

    enum InterruptId
    {
        VBlankInterrupt = 0x00,
        LcdStatInt = 0x01,
        TimerInterrupt = 0x02,
        SerialInterrupt = 0x03,
        JoypadInterrupt = 0x04,
        NoInterrupt = 0xFF
    };

    const addr_t INTERRUPT_VECTOR[5] =
    {
        0x0040,
        0x0048,
        0x0050,
        0x0058,
        0x0060
    };

    typedef struct
    {
        InterruptId source;
        addr_t vector_address;
    } InterruptInfo;

    Irc(uint8_t* IF, uint8_t* IE);

    void hard_reset();
    inline bool has_pending_requests();
    inline InterruptInfo next_request();
    inline uint8_t ime_flag_get();
    inline void ime_flag_set();
    inline void ime_flag_clear();
    inline void request_interrupt(int source);
    inline bool interrupt_requested(int source);
    inline bool interrupt_enabled(int source);
    inline void clear_interrupt(int source);
    inline void disable_interrupt(int source);
    inline void enable_interrupt(int source);

    // TODO: Move into CPU.
    uint8_t IME;
    uint8_t* IF = nullptr;
    uint8_t* IE = nullptr;
};

FORCE_INLINE bool Irc::has_pending_requests()
{
    return (*IF & 0x1F) != 0;
}

FORCE_INLINE Irc::InterruptInfo Irc::next_request()
{
    for (InterruptId i = VBlankInterrupt; i < JoypadInterrupt; i = (InterruptId)((int)i + 1))
    {
        if (interrupt_enabled(i) && interrupt_requested(i))
        {
            return {(InterruptId)i, INTERRUPT_VECTOR[i]};
        }
    }

    return {NoInterrupt, 0x0000};
}

FORCE_INLINE uint8_t Irc::ime_flag_get()
{
    return IME;
}

FORCE_INLINE void Irc::ime_flag_set()
{
    IME = 0x01;
}

FORCE_INLINE void Irc::ime_flag_clear()
{
    IME = 0x00;
}

FORCE_INLINE void Irc::request_interrupt(int source)
{
    /*
    switch (source)
    {
        case VBlankInterrupt:
            cerr << "Vblank IRQ" << endl;
            break;
        case LcdStatInt:
            cerr << "LCD STAT IRQ" << endl;
            break;
        case JoypadInterrupt:
            cerr << "Joypad IRQ" << endl;
            break;
        case TimerInterrupt:
            cerr << "Timer IRQ" << endl;
            break;
        case SerialInterrupt:
            cerr << "Serial IRQ" << endl;
            break;
    }
    */

    *IF |= 0x01 << source;
}

FORCE_INLINE bool Irc::interrupt_requested(int source)
{
    return (*IF & (0x01 << source)) != 0;
}

FORCE_INLINE bool Irc::interrupt_enabled(int source)
{
    return (*IE & (0x01 << source)) != 0;
}

FORCE_INLINE void Irc::clear_interrupt(int source)
{
    *IF &= ~(0x01 << source);
}

FORCE_INLINE void Irc::disable_interrupt(int source)
{
    *IE &= ~(0x01 << source);
}

FORCE_INLINE void Irc::enable_interrupt(int source)
{
    *IE |= 0x01 << source;
}
