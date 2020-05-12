#pragma once

#include <cstdint>
#include "../util/macros.hh"

class DebugObserver
{
public:
    virtual ~DebugObserver() = default;
    virtual void on_debugging_resumed() = 0;
    virtual void on_debugging_paused() = 0;
    virtual void on_breakpoint_added(uint16_t address) = 0;
    virtual void on_breakpoint_removed(uint16_t address) = 0;
    virtual void on_data_breakpoint_added(uint16_t address) = 0;
    virtual void on_data_breakpoint_removed(uint16_t address) = 0;
    virtual void on_memory_changed(uint16_t address) = 0;
    virtual void on_rom_loaded() = 0;
    virtual void on_special_register_changed() = 0;
};
