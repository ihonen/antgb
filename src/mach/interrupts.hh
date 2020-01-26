#ifndef INTERRUPTS_HH
#define INTERRUPTS_HH

#include "memory.hh"

class InterruptController
{
public:
    enum InterruptId
    {
        VBlankInterrupt = 0x00,
        LcdStatInterrupt = 0x01,
        TimerInterrupt = 0x02,
        SerialInterrupt = 0x03,
        JoypadInterrupt = 0x04,
        NoInterrupt = 0xFF
    };

    const memaddr_t VECTOR_ADDRESS[5] =
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

    InterruptController(Memory* mem);

    void hard_reset();
    bool has_active_requests();
    InterruptInfo accept_next_request();
    uint8_t ime_flag_get();
    void ime_flag_set();
    void ime_flag_clear();
    void request_interrupt(int source);
    bool interrupt_requested(int source);
    bool interrupt_enabled(int source);
    void clear_interrupt(int source);
    void disable_interrupt(int source);
    void enable_interrupt(int source);

    uint8_t interrupt_master_enable;
    Memory* mem;
};

#endif // INTERRUPTS_HH
