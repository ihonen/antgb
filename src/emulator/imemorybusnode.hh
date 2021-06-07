#pragma once

#include "types.hh"

class iMemoryBusNode
{
public:
    virtual ~iMemoryBusNode() = default;
    virtual bool owns(addr_t address) = 0;
    virtual uint8_t* get(addr_t address) = 0;
    virtual uint8_t read(addr_t address) = 0;
    virtual void write(addr_t address, uint8_t value) = 0;
};
