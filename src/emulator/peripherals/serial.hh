#pragma once

#include "emulator/interfaces/iemulatorcomponent.hh"
#include "debugger/ifrontend.hh"
#include "emulator/cpu/interrupts.hh"
#include "serialregisters.hh"

class Serial : public iEmulatorComponent
{
public:
    static constexpr emutime_t TCYCLES_PER_BYTE = 4096;

    Serial(SerialRegisters& reg, Interrupts& interrupts, iFrontend* frontend = nullptr);

    void set_serial_callback(iFrontend::SerialCallback callback);

    virtual void pre_cpu_exec_tick() override {}
    inline virtual void post_cpu_exec_tick(emutime_t tcycles) override;

    emutime_t tcycles_left_in_transfer;
    Interrupts& interrupts;
    SerialRegisters& reg;
    iFrontend* frontend_;

    iFrontend::SerialCallback callback_;
};

FORCE_INLINE void Serial::post_cpu_exec_tick(emutime_t tcycles)
{
    if (reg.read(SB_ADDR) != 0x00 && reg.read(SC_ADDR) == 0x81)
    {
        if (callback_ != nullptr)
        {
            callback_(reg.read(SB_ADDR));
        }
        reg.write(SB_ADDR, 0x00);
        tcycles_left_in_transfer = TCYCLES_PER_BYTE;
    }

    if (tcycles_left_in_transfer <= tcycles && tcycles_left_in_transfer != 0)
    {
        interrupts.request_interrupt(Interrupts::Serial);
        tcycles_left_in_transfer = 0;
        reg.write(SB_ADDR, 0x00);
        reg.write(SC_ADDR, 0x01);
    }
    else
    {
        tcycles_left_in_transfer -= tcycles;
    }
}
