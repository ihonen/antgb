#include "joypad.hh"

#include "../util/bitmanip.hh"
#include <iostream>

Joypad::Joypad(Memory* memory, InterruptController* irc_) :
    mem(memory),
    irc(irc_)
{
    io_register = mem->get(IO_REG_ADDRESS);

    button_status =
    {
        {Button::Up,        {Up,        false}},
        {Button::Down,      {Down,      false}},
        {Button::Left,      {Left,      false}},
        {Button::Right,     {Right,     false}},
        {Button::Select,    {Select,    false}},
        {Button::Start,     {Start,     false}},
        {Button::A,         {ButtonA,   false}},
        {Button::B,         {ButtonB,   false}},
    };
}

void Joypad::button_pressed(Button button)
{
    if (button == Button::None) return;

    if (get_bit(io_register, DirectionKeysSelect))
    {
        switch (button)
        {
            case Button::Right:
            case Button::Left:
            case Button::Up:
            case Button::Down:
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
            case Button::A:
            case Button::B:
            case Button::Select:
            case Button::Start:
                clear_bit(io_register, button_status[button].bit_pos);
                break;
            default:
                break;
        }
    }

    if (button_status[button].pressed == false)
    {
        button_status[button].pressed = true;
        irc->request_interrupt(InterruptController::JoypadInterrupt);
    }
}

void Joypad::button_released(Joypad::Button button)
{
    if (button == Button::None) return;

    if (get_bit(io_register, DirectionKeysSelect))
    {
        switch (button)
        {
            case Button::Right:
            case Button::Left:
            case Button::Up:
            case Button::Down:
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
            case Button::A:
            case Button::B:
            case Button::Select:
            case Button::Start:
                set_bit(io_register, button_status[button].bit_pos);
                break;
            default:
                break;
        }
    }

    button_status[button].pressed = false;
}
