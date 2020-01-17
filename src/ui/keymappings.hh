#ifndef KEYMAPPINGS_HH
#define KEYMAPPINGS_HH

#include "../mach/joypad.hh"
#include <map>
#include <QKeyEvent>

const std::map<int, JoypadButton> KEYMAP
{
    {Qt::Key_W,         JoypadButton::Up},
    {Qt::Key_S,         JoypadButton::Down},
    {Qt::Key_A,         JoypadButton::Left},
    {Qt::Key_D,         JoypadButton::Right},
    {Qt::Key_Return,    JoypadButton::A},
    {Qt::Key_Backspace, JoypadButton::B},
    {Qt::Key_Comma,     JoypadButton::Select},
    {Qt::Key_Period,    JoypadButton::Start}
};

#endif // KEYMAPPINGS_HH
