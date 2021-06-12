#include "interrupts.hh"

Interrupts::Interrupts(CpuRegisters& registers)
    : reg_(registers)
    , vblank_irq_count_(0)
    , lcdstat_irq_count_(0)
    , timer_irq_count_(0)
    , serial_irq_count_(0)
    , joypad_irq_count_(0)
    , enable_delay_left_(EXPIRED)
    , disable_delay_left_(EXPIRED)
{
}


void Interrupts::post_bootram_reset()
{
    vblank_irq_count_   = 0;
    lcdstat_irq_count_  = 0;
    timer_irq_count_    = 0;
    serial_irq_count_   = 0;
    joypad_irq_count_   = 0;
    disable_delay_left_ = EXPIRED;
    enable_delay_left_  = EXPIRED;
}
const Interrupts::Irq& Interrupts::next_request()
{
    for (Source i = VBlank;
         i <= Joypad;
         i = static_cast<Source>(static_cast<int>(i) + 1))
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
                    return NONE_IRQ;
            }
        }
    }
    return NONE_IRQ;
}

void Interrupts::request_interrupt(Source source)
{
    switch (source)
    {
    case VBlank:
        ++vblank_irq_count_;
        break;
    case LcdStat:
        ++lcdstat_irq_count_;
        break;
    case Joypad:
        ++joypad_irq_count_;
        break;
    case Timer:
        ++timer_irq_count_;
        break;
    case Serial:
        ++serial_irq_count_;
        break;
    case None:
        break;
    }

    reg_.write_IF(reg_.read_IF() | static_cast<uint8_t>((1 << source)));
}
