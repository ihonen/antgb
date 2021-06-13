#pragma once

#include "emulator/interfaces/iemulatorcomponent.hh"
#include "emulator/cpu/cpu.hh"
#include "timerregisters.hh"

class Timer : public iEmulatorComponent
{
public:
    Timer(TimerRegisters& reg, Interrupts& interrupts);

    inline virtual void pre_cpu_exec_tick() override;
    inline virtual void post_cpu_exec_tick(emutime_t tcycles) override;

    void set_stopped(bool stopped);
    inline bool is_stopped();

protected:

    inline bool is_timer_enabled();
    inline emutime_t timer_tcycles_per_tick();

    inline void divider_post_cpu_exec_tick(emutime_t tcycles);
    inline void timer_post_cpu_exec_tick(emutime_t tcycles);
    inline void emulate_timerold(emutime_t tcycles);

    static constexpr emutime_t DIVIDER_FREQ_Hz = 16384;
    static constexpr emutime_t DIVIDER_TCYCLES_PER_TICK =
        Cpu::CLK_FREQ_Hz / DIVIDER_FREQ_Hz;

    const emutime_t TIMER_FREQ_DIVIDER[4] =
    {
        1024,
        16,
        64,
        256,
    };

    TimerRegisters& reg_;
    Interrupts& interrupts_;

    emutime_t divider_unemulated_tcycles_;
    emutime_t timer_unemulated_tcycles_;

    bool is_stopped_;
};

FORCE_INLINE void Timer::pre_cpu_exec_tick()
{
}

FORCE_INLINE bool Timer::is_stopped()
{
    return is_stopped_;
}

FORCE_INLINE void Timer::post_cpu_exec_tick(emutime_t tcycles)
{
    divider_post_cpu_exec_tick(tcycles);
    timer_post_cpu_exec_tick(tcycles);
}

FORCE_INLINE bool Timer::is_timer_enabled()
{
    return bool(reg_.read(TAC_ADDR) & (1 << TimerRegisters::TimerEnable));
}

FORCE_INLINE emutime_t Timer::timer_tcycles_per_tick()
{
    return TIMER_FREQ_DIVIDER[reg_.read(TAC_ADDR)
           & (TimerRegisters::InputClockSelect0
              | TimerRegisters::InputclockSelect1)];
}

FORCE_INLINE void Timer::divider_post_cpu_exec_tick(emutime_t tcycles)
{
    if (!is_stopped())
    {
        // TODO: Register interface violation.
        // This is required because DIV is marked read only in the interface.
        auto& div_value = *reg_.get(DIV_ADDR);

        for (divider_unemulated_tcycles_ += tcycles;
             divider_unemulated_tcycles_ >= DIVIDER_TCYCLES_PER_TICK;
             divider_unemulated_tcycles_ -= DIVIDER_TCYCLES_PER_TICK)
        {
            ++div_value;
        }
    }
}

FORCE_INLINE void Timer::timer_post_cpu_exec_tick(emutime_t tcycles)
{
    // TODO: Pandocs obscure behavior.

    if (is_timer_enabled() && !is_stopped())
    {
        auto tcycles_per_tick = timer_tcycles_per_tick();

        for (timer_unemulated_tcycles_ += tcycles;
             timer_unemulated_tcycles_ >= tcycles_per_tick;
             timer_unemulated_tcycles_ -= tcycles_per_tick)
        {
            if (reg_.read(TIMA_ADDR) == 0xFF)
            {
                reg_.write(TIMA_ADDR, reg_.read(TMA_ADDR));
                interrupts_.request_interrupt(Interrupts::Timer);
                return;
            }
            else
            {
                reg_.write(TIMA_ADDR, reg_.read(TIMA_ADDR) + 1);
            }
        }
    }
}
