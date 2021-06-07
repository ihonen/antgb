#pragma once

#include "imemorybusnode.hh"
#include "memorybase.hh"
#include "types.hh"

#include <array>

class Cartridge : public iMemoryBusNode
{
public:
    Cartridge();
    Cartridge(const std::string& filepath);

    virtual ~Cartridge() override = default;

    virtual bool owns(memaddr_t address) override;
    virtual uint8_t* get(memaddr_t address) override;
    virtual uint8_t read(memaddr_t address) override;
    virtual void write(memaddr_t address, uint8_t value) override;
private:
    MemoryBase<ROM0_LOW, ROM0_HIGH> rom0_;
    MemoryBase<ROM1_LOW, ROM1_HIGH> rom1_;
    MemoryBase<ERAM_LOW, ERAM_HIGH> eram_;
};
