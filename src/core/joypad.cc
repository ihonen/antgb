#include "joypad.hh"

#include "bitmanip.hh"
#include "memory.hh"
#include <iostream>

namespace antgb
{

Joypad::Joypad(Memory* memory, Irc* irc_) :
    mem(memory),
    irc(irc_)
{
    io_register = mem->get(IO_REG_ADDRESS);

    button_status =
    {
        {JoypadUp,          {Up,        false}},
        {JoypadDown,        {Down,      false}},
        {JoypadLeft,        {Left,      false}},
        {JoypadRight,       {Right,     false}},
        {JoypadSelect,      {Select,    false}},
        {JoypadStart,       {Start,     false}},
        {JoypadA,           {ButtonA,   false}},
        {JoypadB,           {ButtonB,   false}},
    };
}

void Joypad::button_pressed(JoypadButton button)
{
    if (button == JoypadNone) return;

    if (get_bit(io_register, DirectionKeysSelect))
    {
        switch (button)
        {
            case JoypadRight:
            case JoypadLeft:
            case JoypadUp:
            case JoypadDown:
                clear_bit(io_register, button_status[button].bit_pos);
                break;
            default:
                break;
        }
    }
    else if (get_bit(io_register, ButtonKeysSelect))
    {
        switch (button)
        {
            case JoypadA:
            case JoypadB:
            case JoypadSelect:
            case JoypadStart:
                clear_bit(io_register, button_status[button].bit_pos);
                break;
            default:
                break;
        }
    }

    if (button_status[button].pressed == false)
    {
        button_status[button].pressed = true;
        irc->request_interrupt(Irc::JoypadInterrupt);
    }
}

void Joypad::button_released(JoypadButton button)
{
    if (button == JoypadNone) return;

    if (get_bit(io_register, DirectionKeysSelect))
    {
        switch (button)
        {
            case JoypadRight:
            case JoypadLeft:
            case JoypadUp:
            case JoypadDown:
                set_bit(io_register, button_status[button].bit_pos);
                break;
            default:
                break;
        }
    }
    else if (get_bit(io_register, ButtonKeysSelect))
    {
        switch (button)
        {
            case JoypadA:
            case JoypadB:
            case JoypadSelect:
            case JoypadStart:
                set_bit(io_register, button_status[button].bit_pos);
                break;
            default:
                break;
        }
    }

    button_status[button].pressed = false;
}

} // namespace antgb
