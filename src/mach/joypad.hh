#ifndef JOYPAD_HH
#define JOYPAD_HH

#include <cstdint>

enum class JoypadButton {NONE, Up, Down, Left, Right, Select, Start, A, B};

typedef struct __attribute__((packed))
{
    uint8_t right_or_button_a : 1;
    uint8_t left_or_button_b : 1;
    uint8_t up_or_select : 1;
    uint8_t down_or_start : 1;
    uint8_t direction_keys_select : 1;
    uint8_t button_keys_select : 1;
    uint8_t __unused : 2;
} JoypadRegister;

#endif // JOYPAD_HH
