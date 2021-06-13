#pragma once

#include "emulator/common/types.hh"

#include <array>
#include <cstdint>
#include <cstddef>
#include <functional>
#include <iostream>

class iFrontend
{
public:
    static constexpr size_t SCREEN_WIDTH = 160;
    static constexpr size_t SCREEN_HEIGHT = 144;

    using Pixel = uint32_t;
    using Pixels = std::array<std::array<Pixel, SCREEN_HEIGHT>, SCREEN_WIDTH>;

    using RenderCallback = std::function<void(const Pixels&)>;
    using SerialCallback = std::function<void(uint8_t)>;
    using JoypadCallback = std::function<void(JoypadButton)>;

    virtual ~iFrontend() = default;
};
