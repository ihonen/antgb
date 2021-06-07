#pragma once

#include "memorybase.hh"

class BootRom
    : public MemoryBase<BOOTROM_LOW, BOOTROM_HIGH>
{
protected:
    bool is_locked_ = false;

public:
    BootRom() = default;

    bool is_locked()
    {
        return is_locked_;
    }

    void set_locked(bool locked)
    {
        is_locked_ = locked;
    }
};
