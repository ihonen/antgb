#pragma once

#include "types.hh"

class Irc
{
public:
    struct Registers
    {
        uint8_t IE;
        uint8_t IF;
    };

    static constexpr memaddr_t IE_ADDRESS = 0xFFFF;
    static constexpr memaddr_t IF_ADDRESS = 0xFF0F;

    enum InterruptId
    {
        VBlankInterrupt = 0x00,
        LcdStatInt = 0x01,
        TimerInterrupt = 0x02,
        SerialInterrupt = 0x03,
        JoypadInterrupt = 0x04,
        NoInterrupt = 0xFF
    };

    const memaddr_t INTERRUPT_VECTOR[5] =
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
        memaddr_t vector_address;
    } InterruptInfo;

    Irc(Memory* mem);

    void hard_reset();
    bool has_pending_requests();
    InterruptInfo next_request();
    uint8_t ime_flag_get();
    void ime_flag_set();
    void ime_flag_clear();
    void request_interrupt(int source);
    bool interrupt_requested(int source);
    bool interrupt_enabled(int source);
    void clear_interrupt(int source);
    void disable_interrupt(int source);
    void enable_interrupt(int source);

    // TODO: Move into CPU.
    uint8_t IME;
    Memory* mem;
};
