#pragma once

#include <array>
#include <cstdint>
#include <cstddef>

class iFrontend
{
public:

    static constexpr size_t SCREEN_WIDTH = 160;
    static constexpr size_t SCREEN_HEIGHT = 144;

    using Pixel = uint32_t;
    using Pixels = std::array<std::array<Pixel, SCREEN_HEIGHT>, SCREEN_WIDTH>;

    virtual ~iFrontend() = default;
    virtual void render(const Pixels& pixels) = 0;
};
