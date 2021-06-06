#pragma once

#include "memorybank.hh"

class BootRom
    : public MemoryBank<BOOTROM_LOW, BOOTROM_HIGH>
{
protected:
    bool is_locked_ = false;

public:
    BootRom() = default;
    ~BootRom() = default;

    bool is_locked()
    {
        return is_locked_;
    }

    void set_locked(bool locked)
    {
        is_locked_ = locked;
    }
};
