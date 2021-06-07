#pragma once

#include "addresses.hh"
#include "imemorybusnode.hh"

class SerialRegisters : public iMemoryBusNode
{
public:
    SerialRegisters();
    virtual ~SerialRegisters() override;

    virtual bool owns(addr_t address) override;
    virtual uint8_t* get(addr_t address) override;
    virtual uint8_t read(addr_t address) override;
    virtual void write(addr_t address, uint8_t value) override;

protected:
    uint8_t SB;
    uint8_t SC;
};
