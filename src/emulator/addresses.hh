#pragma once

#include "types.hh"

#include <cstdint>

////////////////////////////////////////////////////////////////////////////////
// MEMORY REGIONS
////////////////////////////////////////////////////////////////////////////////

static constexpr memaddr_t BOOTROM_LOW  = 0x0000;
static constexpr memaddr_t BOOTROM_HIGH = 0x00FF;
static constexpr memaddr_t BOOTROM_SIZE = 0x0100;

static constexpr memaddr_t ROM0_LOW  = 0x0000;
static constexpr memaddr_t ROM0_HIGH = 0x3FFF;
static constexpr memaddr_t ROM0_SIZE = 0x4000;

static constexpr memaddr_t ROM1_LOW  = 0x4000;
static constexpr memaddr_t ROM1_HIGH = 0x7FFF;
static constexpr memaddr_t ROM1_SIZE = 0x4000;

static constexpr memaddr_t VRAM_LOW  = 0x8000;
static constexpr memaddr_t VRAM_HIGH = 0x9FFF;
static constexpr memaddr_t VRAM_SIZE = 0x2000;

static constexpr memaddr_t ERAM_LOW  = 0xA000;
static constexpr memaddr_t ERAM_HIGH = 0xBFFF;
static constexpr memaddr_t ERAM_SIZE = 0x2000;

static constexpr memaddr_t WRAM0_LOW  = 0xC000;
static constexpr memaddr_t WRAM0_HIGH = 0xCFFF;
static constexpr memaddr_t WRAM0_SIZE = 0x1000;

static constexpr memaddr_t WRAM1_LOW  = 0xD000;
static constexpr memaddr_t WRAM1_HIGH = 0xDFFF;
static constexpr memaddr_t WRAM1_SIZE = 0x1000;

static constexpr memaddr_t ECHO_LOW  = 0xE000;
static constexpr memaddr_t ECHO_HIGH = 0xFDFF;
static constexpr memaddr_t ECHO_SIZE = 0x1E00;

static constexpr memaddr_t OAM_LOW  = 0xFE00;
static constexpr memaddr_t OAM_HIGH = 0xFE9F;
static constexpr memaddr_t OAM_SIZE = 0x00A0;

static constexpr memaddr_t UNUSABLE_LOW  = 0xFEA0;
static constexpr memaddr_t UNUSABLE_HIGH = 0xFEFF;
static constexpr memaddr_t UNUSABLE_SIZE = 0x0060;

static constexpr memaddr_t IO_LOW  = 0xFF00;
static constexpr memaddr_t IO_HIGH = 0xFF7F;
static constexpr memaddr_t IO_SIZE = 0x0080;

static constexpr memaddr_t HRAM_LOW  = 0xFF80;
static constexpr memaddr_t HRAM_HIGH = 0xFFFE;
static constexpr memaddr_t HRAM_SIZE = 0x007F;

// PPU

static constexpr memaddr_t TILERAM_LOW  = 0x8000;
static constexpr memaddr_t TILERAM_HIGH = 0x97FF;
static constexpr memaddr_t TILERAM_SIZE = 0x1800;

static constexpr memaddr_t BGMAP1_LOW  = 0x9800;
static constexpr memaddr_t BGMAP1_HIGH = 0x9BFF;
static constexpr memaddr_t BGMAP_SIZE  = 0x4000;

static constexpr memaddr_t BGMAP2_LOW  = 0x9C00;
static constexpr memaddr_t BGMAP2_HIGH = 0x9FFF;

////////////////////////////////////////////////////////////////////////////////
// HARDWARE REGISTERS
////////////////////////////////////////////////////////////////////////////////

// APU
static constexpr memaddr_t NR10_ADDR    = 0xFF10;
static constexpr memaddr_t NR11_ADDR    = 0xFF11;
static constexpr memaddr_t NR12_ADDR    = 0xFF12;
static constexpr memaddr_t NR13_ADDR    = 0xFF13;
static constexpr memaddr_t NR14_ADDR    = 0xFF14;
static constexpr memaddr_t NR21_ADDR    = 0xFF16;
static constexpr memaddr_t NR22_ADDR    = 0xFF17;
static constexpr memaddr_t NR23_ADDR    = 0xFF18;
static constexpr memaddr_t NR24_ADDR    = 0xFF19;
static constexpr memaddr_t NR30_ADDR    = 0xFF1A;
static constexpr memaddr_t NR31_ADDR    = 0xFF1B;
static constexpr memaddr_t NR32_ADDR    = 0xFF1C;
static constexpr memaddr_t NR33_ADDR    = 0xFF1D;
static constexpr memaddr_t NR34_ADDR    = 0xFF1E;
static constexpr memaddr_t NR41_ADDR    = 0xFF20;
static constexpr memaddr_t NR42_ADDR    = 0xFF21;
static constexpr memaddr_t NR43_ADDR    = 0xFF22;
static constexpr memaddr_t NR44_ADDR    = 0xFF23;
static constexpr memaddr_t NR50_ADDR    = 0xFF24;
static constexpr memaddr_t NR51_ADDR    = 0xFF25;
static constexpr memaddr_t NR52_ADDR    = 0xFF26;
static constexpr memaddr_t WAVERAM_LOW  = 0xFF30;
static constexpr memaddr_t WAVERAM_HIGH = 0xFF3F;
static constexpr memaddr_t WAVERAM_SIZE = 0x0010;
static constexpr memaddr_t APU_LOW      = 0xFF10;
static constexpr memaddr_t APU_HIGH     = 0xFF3F;

// Interrupts
static constexpr memaddr_t IE_ADDR = 0xFFFF;
static constexpr memaddr_t IF_ADDR = 0xFF0F;

// Joypad
static constexpr memaddr_t JOYP_ADDR = 0xFF00;

// PPU
static constexpr memaddr_t LCDC_ADDR = 0xFF40;
static constexpr memaddr_t STAT_ADDR = 0xFF41;
static constexpr memaddr_t SCY_ADDR  = 0xFF42;
static constexpr memaddr_t SCX_ADDR  = 0xFF43;
static constexpr memaddr_t LY_ADDR   = 0xFF44;
static constexpr memaddr_t LYC_ADDR  = 0xFF45;
static constexpr memaddr_t DMA_ADDR  = 0xFF46;
static constexpr memaddr_t BGP_ADDR  = 0xFF47;
static constexpr memaddr_t OBP0_ADDR = 0xFF48;
static constexpr memaddr_t OBP1_ADDR = 0xFF49;
static constexpr memaddr_t WY_ADDR   = 0xFF4A;
static constexpr memaddr_t WX_ADDR   = 0xFF4B;
static constexpr memaddr_t PPU_LOW   = 0xFF40;
static constexpr memaddr_t PPU_HIGH  = 0xFF4B;

// Serial
static constexpr memaddr_t SB_ADDR     = 0xFF01;
static constexpr memaddr_t SC_ADDR     = 0xFF02;
static constexpr memaddr_t SERIAL_LOW  = 0xFF01;
static constexpr memaddr_t SERIAL_HIGH = 0xFF02;

// Timer
static constexpr memaddr_t DIV_ADDR   = 0xFF04;
static constexpr memaddr_t TIMA_ADDR  = 0xFF05;
static constexpr memaddr_t TMA_ADDR   = 0xFF06;
static constexpr memaddr_t TAC_ADDR   = 0xFF07;
static constexpr memaddr_t TIMER_LOW  = 0xFF04;
static constexpr memaddr_t TIMER_HIGH = 0xFF07;
