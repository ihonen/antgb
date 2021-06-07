#pragma once

#include "apu.hh"
#include "apuregisters.hh"
#include "addresses.hh"
#include "cartridge.hh"
#include "cpuregisters.hh"
#include "joypad.hh"
#include "joypadregisters.hh"
#include "macros.hh"
#include "ppu.hh"
#include "ppuregisters.hh"
#include "serial.hh"
#include "timer.hh"
#include "types.hh"

#include "bootrom.hh"
#include "echoram.hh"
#include "hram.hh"
#include "oam.hh"
#include "vram.hh"
#include "wram0.hh"
#include "wram1.hh"

class MemoryBus
{
public:
    MemoryBus(
        BootRom& bootrom,
        EchoRam& echoram,
        Hram& hram,
        Oam& oam,
        Vram& vram,
        Wram0& wram0,
        Wram1& wram1,
        ApuRegisters& apu_registers,
        CpuRegisters& cpu_registers,
        JoypadRegisters& joypad_registers,
        PpuRegisters& ppu_registers,
        SerialRegisters& serial_registers,
        TimerRegisters& timer_registers
    );

    void hard_reset();
    inline uint8_t* get(memaddr_t address);
    inline uint8_t read(memaddr_t address);
    inline void write(memaddr_t address, uint8_t value);
    inline bool force_write(memaddr_t address, uint8_t value);
    inline bool can_read(memaddr_t address);
    inline bool can_write(memaddr_t address);
    void launch_oam_dma(memaddr_t destination,
                        memaddr_t source,
                        memaddr_t size);
    void emulate(uint64_t cpu_cycles);
    void emulate_oam_dma(uint64_t cpu_cycles);
    void end_oam_dma();
    void clear_dma_status();
    void set_cartridge(Cartridge* cartridge);

    Cartridge* cartridge;

    BootRom& bootrom_;
    EchoRam& echoram_;
    Hram& hram_;
    Oam& oam_;
    Vram& vram_;
    Wram0& wram0_;
    Wram1& wram1_;

    ApuRegisters& apu_registers_;
    CpuRegisters& cpu_registers_;
    JoypadRegisters& joypad_registers_;
    PpuRegisters& ppu_registers_;
    SerialRegisters& serial_registers_;
    TimerRegisters& timer_registers_;

    struct
    {
        uint64_t unemulated_cpu_cycles;
        uint64_t cpu_cycles_left;
        memaddr_t src_pointer;
        memaddr_t dest_pointer;
        memaddr_t src_low;
        memaddr_t src_high;
        memaddr_t size;
    } dma_status;
};

FORCE_INLINE uint8_t* MemoryBus::get(memaddr_t address)
{
    if (address <= BOOTROM_HIGH && !bootrom_.is_locked())
    {
        return bootrom_.get(address);
    }
    else if (address <= ROM1_HIGH)
    {
        if (cartridge) return cartridge->get(address);
        else return nullptr;
    }
    else if (address >= VRAM_LOW && address <= VRAM_HIGH)
    {
        return vram_.get(address);
    }
    else if (address >= ERAM_LOW && address <= ERAM_HIGH)
    {
        if (cartridge) return cartridge->get(address);
        else return nullptr;
    }
    else if (address >= WRAM0_LOW && address <= WRAM0_HIGH)
    {
        return wram0_.get(address);
    }
    else if (address >= WRAM1_LOW && address <= WRAM1_HIGH)
    {
        return wram1_.get(address);
    }
    else if (address >= ECHO_LOW && address <= ECHO_HIGH)
    {
        return echoram_.get(address);
    }
    else if (address >= OAM_LOW && address <= OAM_HIGH)
    {
        return oam_.get(address);
    }
    else if (address >= UNUSABLE_LOW && address <= UNUSABLE_HIGH)
    {
        return nullptr;
    }
    else if (address >= HRAM_LOW && address <= HRAM_HIGH)
    {
        return hram_.get(address);
    }
    else if (address >= APU_LOW && address <= APU_HIGH)
    {
        return apu_registers_.get(address);
    }
    else if (address == JOYP_ADDR)
    {
        return joypad_registers_.get(address);
    }
    else if (address >= PPU_LOW && address <= PPU_HIGH)
    {
        return ppu_registers_.get(address);
    }
    else if (address >= SERIAL_LOW && address <= SERIAL_HIGH)
    {
        return serial_registers_.get(address);
    }
    else if (address >= TIMER_LOW && address <= TIMER_HIGH)
    {
        return timer_registers_.get(address);
    }
    else if (address == IF_ADDR || address == IE_ADDR)
    {
        return cpu_registers_.get(address);
    }
    else return nullptr;
}

FORCE_INLINE uint8_t MemoryBus::read(memaddr_t address)
{
    if (address <= BOOTROM_HIGH && !bootrom_.is_locked())
    {
        return bootrom_.read(address);
    }
    else if (address <= ROM1_HIGH)
    {
        if (cartridge) return cartridge->read(address);
        else return 0xFF;
    }
    else if (address >= VRAM_LOW && address <= VRAM_HIGH)
    {
        return vram_.read(address);
    }
    else if (address >= ERAM_LOW && address <= ERAM_HIGH)
    {
        if (cartridge) return cartridge->read(address);
        else return 0xFF;
    }
    else if (address >= WRAM0_LOW && address <= WRAM0_HIGH)
    {
        return wram0_.read(address);
    }
    else if (address >= WRAM1_LOW && address <= WRAM1_HIGH)
    {
        return wram1_.read(address);
    }
    else if (address >= ECHO_LOW && address <= ECHO_HIGH)
    {
        return echoram_.read(address);
    }
    else if (address >= OAM_LOW && address <= OAM_HIGH)
    {
        return oam_.read(address);
    }
    else if (address >= UNUSABLE_LOW && address <= UNUSABLE_HIGH)
    {
        return 0xFF;
    }
    else if (address >= HRAM_LOW && address <= HRAM_HIGH)
    {
        return hram_.read(address);
    }
    else if (address >= APU_LOW && address <= APU_HIGH)
    {
        return apu_registers_.read(address);
    }
    else if (address == JOYP_ADDR)
    {
        return joypad_registers_.read(address);
    }
    else if (address >= PPU_LOW && address <= PPU_HIGH)
    {
        return ppu_registers_.read(address);
    }
    else if (address >= SERIAL_LOW && address <= SERIAL_HIGH)
    {
        return serial_registers_.read(address);
    }
    else if (address >= TIMER_LOW && address <= TIMER_HIGH)
    {
        return timer_registers_.read(address);
    }
    else if (address == IF_ADDR || address == IE_ADDR)
    {
        return cpu_registers_.read(address);
    }
    else return 0xFF;
}

FORCE_INLINE void MemoryBus::write(memaddr_t address, uint8_t value)
{
    if (address <= BOOTROM_HIGH && !bootrom_.is_locked())
    {
        bootrom_.write(address, value);
    }
    else if (address <= ROM1_HIGH)
    {
        if (cartridge) return cartridge->write(address, value);
        else;
    }
    else if (address >= VRAM_LOW && address <= VRAM_HIGH)
    {
        return vram_.write(address, value);
    }
    else if (address >= ERAM_LOW && address <= ERAM_HIGH)
    {
        if (cartridge) return cartridge->write(address, value);
        else;
    }
    else if (address >= WRAM0_LOW && address <= WRAM0_HIGH)
    {
        wram0_.write(address, value);
    }
    else if (address >= WRAM1_LOW && address <= WRAM1_HIGH)
    {
        wram1_.write(address, value);
    }
    else if (address >= ECHO_LOW && address <= ECHO_HIGH)
    {
        echoram_.write(address, value);
    }
    else if (address >= OAM_LOW && address <= OAM_HIGH)
    {
        oam_.write(address, value);
    }
    else if (address >= UNUSABLE_LOW && address <= UNUSABLE_HIGH)
    {

    }
    else if (address >= HRAM_LOW && address <= HRAM_HIGH)
    {
        hram_.write(address, value);
    }
    else if (address >= APU_LOW && address <= APU_HIGH)
    {
        apu_registers_.write(address, value);
    }
    else if (address == JOYP_ADDR)
    {
        joypad_registers_.write(address, value);
    }
    else if (address >= PPU_LOW && address <= PPU_HIGH)
    {
        ppu_registers_.write(address, value);
    }
    else if (address >= SERIAL_LOW && address <= SERIAL_HIGH)
    {
        serial_registers_.write(address, value);
    }
    else if (address >= TIMER_LOW && address <= TIMER_HIGH)
    {
        timer_registers_.write(address, value);
    }
    else if (address == IF_ADDR || address == IE_ADDR)
    {
        cpu_registers_.write(address, value);
    }
}

FORCE_INLINE bool MemoryBus::force_write(memaddr_t address, uint8_t value)
{
    auto dest = get(address);
    if (dest) *dest = value;
    return 0;
}
