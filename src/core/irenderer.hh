#pragma once

#include <array>
#include <cstdint>

namespace antgb
{
typedef std::array<std::array<uint8_t, 160>, 144> framebuf_t;

class iRenderer
{
public:
    virtual ~iRenderer()                        = default;
    virtual void do_render(framebuf_t framebuf) = 0;
};

} // namespace antgb
