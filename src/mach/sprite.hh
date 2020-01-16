#ifndef SPRITE_HH
#define SPRITE_HH

#include <cstdint>

typedef struct __attribute__((packed))
{
    uint8_t y_pos;
    uint8_t x_pos;
    uint8_t tile_number;
    struct __attribute__((packed))
    {
        uint8_t obj_to_bg_priority : 1;
        uint8_t y_flip : 1;
        uint8_t x_flip : 1;
        uint8_t palette_number : 1;
        uint8_t __unused_in_dmg_1 : 1;
        uint8_t __unused_in_dmg_2: 3;
    } flags;
} SpriteAttribute;

#endif // SPRITE_HH
