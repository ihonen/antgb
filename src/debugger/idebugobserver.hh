#pragma once

#include "debugevent.hh"

class iDebugObserver
{
public:
    virtual ~iDebugObserver() = default;
    virtual void debug_event(const DebugEvent* event) = 0;
};
