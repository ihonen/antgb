#pragma once

#include "emulator/interfaces/iemulatorcomponent.hh"
#include "emulator/cpu/interrupts.hh"
#include "joypadregisters.hh"
#include <map>

class Joypad : public iEmulatorComponent
{
public:
    Joypad(JoypadRegisters& registers, Interrupts& interrupts);

    virtual void pre_cpu_exec_tick() override {}
    virtual void post_cpu_exec_tick(emutime_t /*tcycles*/) override {}

    void button_pressed(JoypadButton button);
    void button_released(JoypadButton button);
protected:
    JoypadRegisters& registers_;
    Interrupts& interrupts_;
};
