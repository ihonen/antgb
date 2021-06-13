#pragma once

#include "memorybase.hh"

class BootRom : public MemoryBase<BOOTROM_LOW, BOOTROM_HIGH>
{
protected:
    bool is_locked_ = false;

public:
    BootRom(const std::string& filepath);

    virtual bool owns(addr_t address) override
    {
        if (address == BOOTROM_LOCK_ADDR)
        {
            return true;
        }

        return MemoryBase<BOOTROM_LOW, BOOTROM_HIGH>::owns(address);
    }

    virtual uint8_t* get(addr_t address) override
    {
        if (address == BOOTROM_LOCK_ADDR)
        {
            return nullptr;
        }

        return MemoryBase<BOOTROM_LOW, BOOTROM_HIGH>::get(address);
    }

    virtual uint8_t read(addr_t address) override
    {
        if (address == BOOTROM_LOCK_ADDR)
        {
            return 0xFF;
        }

        return MemoryBase<BOOTROM_LOW, BOOTROM_HIGH>::read(address);
    }

    virtual void write(addr_t address, uint8_t value) override
    {
        if (address == BOOTROM_LOCK_ADDR)
        {
            set_locked(true);
        }
        else
        {
            MemoryBase<BOOTROM_LOW, BOOTROM_HIGH>::write(address, value);
        }
    }

    bool is_locked()
    {
        return is_locked_;
    }

    void set_locked(bool locked)
    {
        is_locked_ = locked;
    }
};
