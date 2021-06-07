#pragma once

#include "wram0.hh"
#include "wram1.hh"

class EchoRam : public MemoryBase<ECHO_LOW, ECHO_HIGH>
{
public:
    EchoRam(Wram0& wram0, Wram1& wram1)
        : wram0_(wram0)
        , wram1_(wram1)
    {
    }

    virtual uint8_t* get(memaddr_t address) override
    {
        memaddr_t redirect_address = address - ECHO_LOW + WRAM0_LOW;
        if (redirect_address <= wram0_.high())
        {
            return wram0_.get(redirect_address);
        }
        else
        {
            return wram1_.get(redirect_address);
        }
    }

protected:
    Wram0& wram0_;
    Wram1& wram1_;
};
