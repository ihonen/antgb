#pragma once

#include "constants.hh"
#include "util/macros.hh"
#include "emulator/types.hh"

#include <cassert>
#include <set>

enum BpMode : uint64_t
{
    BpNone = 0x00,
    BpOnRead = 0x01,
    BpOnWrite = 0x02,
    BpOnExec = 0x04
};

class Breakpoint
{
public:
    Breakpoint(regid_t register_id, const std::set<BpMode>& modes = {});
    void set_mode(BpMode mode);
    void clear_mode(BpMode mode);
    void toggle_mode(BpMode mode);
    bool has_mode(BpMode mode);
    const std::set<BpMode>& modes();
    regid_t register_id() const;
protected:
    regid_t m_register_id = INVALID_ADDRESS;
    // Evaluated lazily when requested.
    std::set<BpMode> m_modes = {};
    bool m_on_read = false;
    bool m_on_write = false;
    bool m_on_exec = false;
};
