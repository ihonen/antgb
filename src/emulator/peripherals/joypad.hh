#pragma once

#include "debugger/iemulator.hh"

#include "emulator/interfaces/iemulatorcomponent.hh"
#include "emulator/cpu/interrupts.hh"
#include "joypadregisters.hh"
#include <map>

class Joypad : public iEmulatorComponent
{
public:
    Joypad(JoypadRegisters& registers, Interrupts& interrupts);

    void set_joypad_press_callback(iFrontend::JoypadCallback callback);
    void set_joypad_release_callback(iFrontend::JoypadCallback callback);

    virtual void pre_cpu_exec_tick() override {}
    virtual void post_cpu_exec_tick(emutime_t /*tcycles*/) override {}

    void button_pressed(JoypadButton button);
    void button_released(JoypadButton button);
protected:
    JoypadRegisters& registers_;
    Interrupts& interrupts_;
    iFrontend::JoypadCallback press_callback_;
    iFrontend::JoypadCallback release_callback_;
};
