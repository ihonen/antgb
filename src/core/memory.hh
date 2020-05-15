#pragma once

#include "apu.hh"
#include "cartridge.hh"
#include "interrupts.hh"
#include "joypad.hh"
#include "ppu.hh"
#include "serial.hh"
#include "timer.hh"
#include "types.hh"

using namespace std;

class Memory
{
public:

    struct MemoryRegion
    {
        memaddr_t low;
        memaddr_t high;
        memaddr_t size;
    };

    // Unused at the moment
    Irc::Registers ircreg;

    Memory();

    void hard_reset();
    uint8_t* get(memaddr_t address);
    uint8_t read(memaddr_t address);
    bool write(memaddr_t address, uint8_t value);
    bool force_write(memaddr_t address, uint8_t value);
    bool can_read(memaddr_t address);
    bool can_write(memaddr_t address);
    void launch_oam_dma(memaddr_t destination,
                        memaddr_t source,
                        memaddr_t size);
    void emulate(uint64_t cpu_cycles);
    void emulate_oam_dma(uint64_t cpu_cycles);
    void end_oam_dma();
    void clear_dma_status();
    void set_cartridge(Cartridge* cartridge);

    Cartridge* cartridge;

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

    union __attribute__((packed))
    {
        uint8_t h8000_vram[VRAM.size];
        struct __attribute__((packed))
        {
            uint8_t h8000_tilemap0[TILEDATA0.size];
            uint8_t h8800_tilemap1[TILEDATA1.size];
            uint8_t h8800_tilemap2[TILEDATA2.size];
        };
    };
    uint8_t hc000_wram0[WRAM0.size];
    uint8_t hd000_wram1[WRAM1.size];
    uint8_t hfe00_oam[OAM.size];
    union __attribute__((packed))
    {
        uint8_t hff00_io[IO.size];
        struct __attribute__((packed))
        {
            Joypad::Registers joypadreg;
            Serial::Registers serialreg;
            uint8_t hff03_io_unused_in_dmg_1__;
            Timer::Registers timerreg;
            uint8_t hff08_io_unused_in_dmg_2__[7];
            // Interrupt flags
            uint8_t hff0f_if;
            Apu::Registers apureg;
            Ppu::Registers ppureg;
        };
    };
    uint8_t hff80_hram[HRAM.size];
    uint8_t hffff_ie;
};
