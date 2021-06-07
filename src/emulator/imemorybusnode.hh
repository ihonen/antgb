#pragma once

#include "types.hh"

class iMemoryBusNode
{
public:
    virtual ~iMemoryBusNode() = default;
    virtual bool owns(memaddr_t address) = 0;
    virtual uint8_t* get(memaddr_t address) = 0;
    virtual uint8_t read(memaddr_t address) = 0;
    virtual void write(memaddr_t address, uint8_t value) = 0;
};
