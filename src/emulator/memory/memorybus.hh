#pragma once

#include "addresses.hh"
#include "cartridge.hh"
#include "bootrom.hh"
#include "emulator/common/macros.hh"
#include "emulator/interfaces/imemorybusnode.hh"
#include "emulator/common/types.hh"

#include <vector>

class MemoryBus
{
public:
    MemoryBus(
        BootRom& bootrom,
        iMemoryBusNode& echoram,
        iMemoryBusNode& hram,
        iMemoryBusNode& oam,
        iMemoryBusNode& vram,
        iMemoryBusNode& wram0,
        iMemoryBusNode& wram1,
        Cartridge& cartridge,
        iMemoryBusNode& apu_registers,
        iMemoryBusNode& cpu_registers,
        iMemoryBusNode& joypad_registers,
        iMemoryBusNode& ppu_registers,
        iMemoryBusNode& serial_registers,
        iMemoryBusNode& timer_registers
    );

    void hard_reset();
    iMemoryBusNode* find_owner(addr_t address);
    inline uint8_t* get(addr_t address);
    inline uint8_t read(addr_t address);
    inline void write(addr_t address, uint8_t value);
    inline bool force_write(addr_t address, uint8_t value);

    BootRom& bootrom_;

    iMemoryBusNode& echoram_;
    iMemoryBusNode& hram_;
    iMemoryBusNode& oam_;
    iMemoryBusNode& vram_;
    iMemoryBusNode& wram0_;
    iMemoryBusNode& wram1_;

    Cartridge& cartridge_;

    iMemoryBusNode& apu_registers_;
    iMemoryBusNode& cpu_registers_;
    iMemoryBusNode& joypad_registers_;
    iMemoryBusNode& ppu_registers_;
    iMemoryBusNode& serial_registers_;
    iMemoryBusNode& timer_registers_;
};

inline iMemoryBusNode* MemoryBus::find_owner(addr_t address)
{
    iMemoryBusNode* owner = nullptr;

    if (bootrom_.owns(address) && !bootrom_.is_locked())
        owner = static_cast<iMemoryBusNode*>(&bootrom_);
    else if (cartridge_.owns(address))
        owner = &cartridge_;
    else if (hram_.owns(address))
        owner = &hram_;
    else if (oam_.owns(address))
        owner = &oam_;
    else if (vram_.owns(address))
        owner = &vram_;
    else if (wram0_.owns(address))
        owner = &wram0_;
    else if (wram1_.owns(address))
        owner = &wram1_;
    else if (apu_registers_.owns(address))
        owner = &apu_registers_;
    else if (cpu_registers_.owns(address))
        owner = &cpu_registers_;
    else if (joypad_registers_.owns(address))
        owner = &joypad_registers_;
    else if (ppu_registers_.owns(address))
        owner = &ppu_registers_;
    else if (serial_registers_.owns(address))
        owner = &serial_registers_;
    else if (timer_registers_.owns(address))
        owner = &timer_registers_;
    return owner;
}

FORCE_INLINE uint8_t* MemoryBus::get(addr_t address)
{
    auto* owner = find_owner(address);
    if (owner)
        return owner->get(address);
    return nullptr;
}

FORCE_INLINE uint8_t MemoryBus::read(addr_t address)
{
    auto* owner = find_owner(address);
    if (owner)
        return owner->read(address);
    return 0xFF;
}

FORCE_INLINE void MemoryBus::write(addr_t address, uint8_t value)
{
    auto* owner = find_owner(address);
    if (owner)
        owner->write(address, value);
}

FORCE_INLINE bool MemoryBus::force_write(addr_t address, uint8_t value)
{
    auto* dest = get(address);
    if (dest)
        *dest = value;
    return 0;
}

// TODO: Don't allow writes to ROM.
