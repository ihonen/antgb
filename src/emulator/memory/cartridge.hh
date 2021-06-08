#pragma once

#include "emulator/interfaces/imemorybusnode.hh"
#include "memorybase.hh"
#include "emulator/common/types.hh"

#include <array>

class Cartridge : public iMemoryBusNode
{
public:
    Cartridge();
    Cartridge(const std::string& filepath);

    virtual ~Cartridge() override = default;

    bool is_inserted();
    void set_inserted(bool inserted);

    virtual bool owns(addr_t address) override;
    virtual uint8_t* get(addr_t address) override;
    virtual uint8_t read(addr_t address) override;
    virtual void write(addr_t address, uint8_t value) override;
private:
    MemoryBase<ROM0_LOW, ROM0_HIGH> rom0_;
    MemoryBase<ROM1_LOW, ROM1_HIGH> rom1_;
    MemoryBase<ERAM_LOW, ERAM_HIGH> eram_;

    bool is_inserted_;
};
