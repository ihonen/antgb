#pragma once

#include "breakpoint.hh"
#include "types.hh"
#include "util/macros.hh"
#include "emulator/common/types.hh"

enum DebugEventType
{
    DbNone,
    DbContinued,
    DbPaused,
    DbStateReset,
    DbDataChanged,
    DbBreakpointAdded,
    DbBreakpointRemoved,
    DbBreakpointModified,
    DbRomLoaded
};

class DebugEvent
{
public:
    DebugEvent(DebugEventType type,
               regid_t register_id = RegNone) :
        m_type(type),
        m_register_id(register_id)
    {

    }

    FORCE_INLINE
    DebugEventType type() const
    { return m_type; }

    FORCE_INLINE
    regid_t register_id() const
    { return m_register_id; }
protected:
    DebugEventType m_type = DbNone;
    regid_t m_register_id = INVALID_ADDRESS;
};
