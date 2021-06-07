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


class Memory
{
public:

    struct MemoryRegion
    {
        memaddr_t low;
        memaddr_t high;
        memaddr_t size;
    };

    Memory(
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
    inline bool write(memaddr_t address, uint8_t value);
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

    // Main regions
    static constexpr MemoryRegion BOOT = {0x0000, 0x00FF, 0x0100};
    static constexpr MemoryRegion ROM0 = {0x0000, 0x3FFF, 0x4000};
    static constexpr MemoryRegion ROM1 = {0x4000, 0x7FFF, 0x4000};
    static constexpr MemoryRegion VRAM = {0x8000, 0x9FFF, 0x2000};
    static constexpr MemoryRegion ERAM = {0xA000, 0xBFFF, 0x2000};
    static constexpr MemoryRegion WRAM0 = {0xC000, 0xCFFF, 0x1000};
    static constexpr MemoryRegion WRAM1 = {0xD000, 0xDFFF, 0x1000};
    static constexpr MemoryRegion ECHO = {0xE000, 0xFDFF, 0x1E00};
    static constexpr MemoryRegion OAM = {0xFE00, 0xFE9F, 0x00A0};
    static constexpr MemoryRegion UNUSABLE = {0xFEA0, 0xFEFF, 0x0060};
    static constexpr MemoryRegion IO = {0xFF00, 0xFF7F, 0x0080};
    static constexpr MemoryRegion HRAM = {0xFF80, 0xFFFE, 0x007F};
    static constexpr MemoryRegion IE = {0xFFFF, 0xFFFF, 0x0001};

    // Subregions
    static constexpr MemoryRegion TILEDATA0 = {0x8000, 0x87FF, 0x0800};
    static constexpr MemoryRegion TILEDATA1 = {0x8800, 0x8FFF, 0x0800};
    static constexpr MemoryRegion TILEDATA2 = {0x9000, 0x97FF, 0x0800};

    uint8_t bytes[0x10000];
};

// TODO: Write-only (such as NR11)
struct Mask
{
    uint8_t valid = 0xFF;
    uint8_t invalid = 0x00;
    uint8_t readonly = 0xFF;
};

static std::map<memaddr_t, Mask> MASK
{
    // JOYP
    {0xFF00, {0b00111111, 0b11000000, 0b00001111}},
    // TAC
    {0xFF07, {0b00000111, 0b11111000, 0b11111000}},
    // NR52
    {0xFF26, {0b11111111, 0b00000000, 0b00001111}},
    //STAT
    {0xFF41, {0b01111111, 0b10000000, 0b10000111}},
    // IF
    {0xFF00, {0b00011111, 0b11100000, 0b11100000}},
    // IE
    {0xFFFF, {0b00011111, 0b11100000, 0b11100000}}
};

FORCE_INLINE uint8_t* Memory::get(memaddr_t address)
{
    if (address <= ROM1.high)
    {
        if (cartridge) return cartridge->get(address);
        else return nullptr;
    }
    else if (address >= ERAM.low && address <= ERAM.high)
    {
        return nullptr;
    }
    else if (address >= UNUSABLE.low && address <= UNUSABLE.high)
    {
        return nullptr;
    }
    else if (address >= ECHO.low && address <= ECHO.high)
    {
        return &bytes[WRAM0.low] + (address - ECHO.low);
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
    else return &bytes[address];
}

inline void foo() {}

FORCE_INLINE uint8_t Memory::read(memaddr_t address)
{

    uint8_t* source = get(address);
    if (!source) return 0xFF;

    // Some addresses have bits that can't be read, so make sure to return 1
    // in the places of those bits.
    uint8_t invalid_mask = 0x00;
    if (MASK.count(address))
        invalid_mask = MASK[address].invalid;

    return *source | (invalid_mask & 0xFF);
}

FORCE_INLINE bool Memory::write(memaddr_t address, uint8_t value)
{
    if (address == SB_ADDR || address == SC_ADDR)
    {
        foo();
    }

    uint8_t* dest = get(address);
    if (!dest) return false;

    // Some addresses have read-only bits, so make sure not to change them.
    uint8_t readonly_mask = 0x00;
    if (MASK.count(address))
        readonly_mask = MASK[address].readonly;

    // DIV
    if (address == 0xFF04) value = 0x00;

    *dest = (*dest & readonly_mask) | (value & ~readonly_mask);
    return true;
}

FORCE_INLINE bool Memory::force_write(memaddr_t address, uint8_t value)
{
    auto dest = get(address);
    if (dest) *dest = value;
    return 0;
}
