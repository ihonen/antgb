#ifndef KEYBINDINGS_HH
#define KEYBINDINGS_HH

#include "../mach/joypad.hh"
#include <map>
#include <QKeyEvent>

const std::map<int, Joypad::Button> KEYMAP
{
    {Qt::Key_W,         Joypad::Button::Up},
    {Qt::Key_S,         Joypad::Button::Down},
    {Qt::Key_A,         Joypad::Button::Left},
    {Qt::Key_D,         Joypad::Button::Right},
    {Qt::Key_Return,    Joypad::Button::A},
    {Qt::Key_Backspace, Joypad::Button::B},
    {Qt::Key_Comma,     Joypad::Button::Select},
    {Qt::Key_Period,    Joypad::Button::Start}
};

#endif // KEYBINDINGS_HH
