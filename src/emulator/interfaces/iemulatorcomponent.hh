#pragma once

#include "emulator/common/types.hh"

class iEmulatorComponent
{
public:
    virtual ~iEmulatorComponent() = default;
    virtual void pre_cpu_exec_tick() = 0;
    virtual void post_cpu_exec_tick(emutime_t /*tcycles*/) = 0;
};
