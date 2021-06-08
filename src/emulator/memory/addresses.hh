#pragma once

#include "emulator/common/types.hh"

#include <cstdint>

////////////////////////////////////////////////////////////////////////////////
// MEMORY REGIONS
////////////////////////////////////////////////////////////////////////////////

static constexpr addr_t BOOTROM_LOW  = 0x0000;
static constexpr addr_t BOOTROM_HIGH = 0x00FF;
static constexpr addr_t BOOTROM_SIZE = 0x0100;

static constexpr addr_t ROM0_LOW  = 0x0000;
static constexpr addr_t ROM0_HIGH = 0x3FFF;
static constexpr addr_t ROM0_SIZE = 0x4000;

static constexpr addr_t ROM1_LOW  = 0x4000;
static constexpr addr_t ROM1_HIGH = 0x7FFF;
static constexpr addr_t ROM1_SIZE = 0x4000;

static constexpr addr_t VRAM_LOW  = 0x8000;
static constexpr addr_t VRAM_HIGH = 0x9FFF;
static constexpr addr_t VRAM_SIZE = 0x2000;

static constexpr addr_t ERAM_LOW  = 0xA000;
static constexpr addr_t ERAM_HIGH = 0xBFFF;
static constexpr addr_t ERAM_SIZE = 0x2000;

static constexpr addr_t WRAM0_LOW  = 0xC000;
static constexpr addr_t WRAM0_HIGH = 0xCFFF;
static constexpr addr_t WRAM0_SIZE = 0x1000;

static constexpr addr_t WRAM1_LOW  = 0xD000;
static constexpr addr_t WRAM1_HIGH = 0xDFFF;
static constexpr addr_t WRAM1_SIZE = 0x1000;

static constexpr addr_t ECHO_LOW  = 0xE000;
static constexpr addr_t ECHO_HIGH = 0xFDFF;
static constexpr addr_t ECHO_SIZE = 0x1E00;

static constexpr addr_t OAM_LOW  = 0xFE00;
static constexpr addr_t OAM_HIGH = 0xFE9F;
static constexpr addr_t OAM_SIZE = 0x00A0;

static constexpr addr_t UNUSABLE_LOW  = 0xFEA0;
static constexpr addr_t UNUSABLE_HIGH = 0xFEFF;
static constexpr addr_t UNUSABLE_SIZE = 0x0060;

static constexpr addr_t IO_LOW  = 0xFF00;
static constexpr addr_t IO_HIGH = 0xFF7F;
static constexpr addr_t IO_SIZE = 0x0080;

static constexpr addr_t HRAM_LOW  = 0xFF80;
static constexpr addr_t HRAM_HIGH = 0xFFFE;
static constexpr addr_t HRAM_SIZE = 0x007F;

// PPU

static constexpr addr_t TILERAM_LOW  = 0x8000;
static constexpr addr_t TILERAM_HIGH = 0x97FF;
static constexpr addr_t TILERAM_SIZE = 0x1800;

static constexpr addr_t BGMAP1_LOW  = 0x9800;
static constexpr addr_t BGMAP1_HIGH = 0x9BFF;
static constexpr addr_t BGMAP_SIZE  = 0x4000;

static constexpr addr_t BGMAP2_LOW  = 0x9C00;
static constexpr addr_t BGMAP2_HIGH = 0x9FFF;

////////////////////////////////////////////////////////////////////////////////
// HARDWARE REGISTERS
////////////////////////////////////////////////////////////////////////////////

// APU
static constexpr addr_t NR10_ADDR    = 0xFF10;
static constexpr addr_t NR11_ADDR    = 0xFF11;
static constexpr addr_t NR12_ADDR    = 0xFF12;
static constexpr addr_t NR13_ADDR    = 0xFF13;
static constexpr addr_t NR14_ADDR    = 0xFF14;
static constexpr addr_t NR21_ADDR    = 0xFF16;
static constexpr addr_t NR22_ADDR    = 0xFF17;
static constexpr addr_t NR23_ADDR    = 0xFF18;
static constexpr addr_t NR24_ADDR    = 0xFF19;
static constexpr addr_t NR30_ADDR    = 0xFF1A;
static constexpr addr_t NR31_ADDR    = 0xFF1B;
static constexpr addr_t NR32_ADDR    = 0xFF1C;
static constexpr addr_t NR33_ADDR    = 0xFF1D;
static constexpr addr_t NR34_ADDR    = 0xFF1E;
static constexpr addr_t NR41_ADDR    = 0xFF20;
static constexpr addr_t NR42_ADDR    = 0xFF21;
static constexpr addr_t NR43_ADDR    = 0xFF22;
static constexpr addr_t NR44_ADDR    = 0xFF23;
static constexpr addr_t NR50_ADDR    = 0xFF24;
static constexpr addr_t NR51_ADDR    = 0xFF25;
static constexpr addr_t NR52_ADDR    = 0xFF26;
static constexpr addr_t WAVERAM_LOW  = 0xFF30;
static constexpr addr_t WAVERAM_HIGH = 0xFF3F;
static constexpr addr_t WAVERAM_SIZE = 0x0010;
static constexpr addr_t APU_LOW      = 0xFF10;
static constexpr addr_t APU_HIGH     = 0xFF3F;

// Interrupts
static constexpr addr_t IE_ADDR = 0xFFFF;
static constexpr addr_t IF_ADDR = 0xFF0F;

// Joypad
static constexpr addr_t JOYP_ADDR = 0xFF00;

// PPU
static constexpr addr_t LCDC_ADDR = 0xFF40;
static constexpr addr_t STAT_ADDR = 0xFF41;
static constexpr addr_t SCY_ADDR  = 0xFF42;
static constexpr addr_t SCX_ADDR  = 0xFF43;
static constexpr addr_t LY_ADDR   = 0xFF44;
static constexpr addr_t LYC_ADDR  = 0xFF45;
static constexpr addr_t DMA_ADDR  = 0xFF46;
static constexpr addr_t BGP_ADDR  = 0xFF47;
static constexpr addr_t OBP0_ADDR = 0xFF48;
static constexpr addr_t OBP1_ADDR = 0xFF49;
static constexpr addr_t WY_ADDR   = 0xFF4A;
static constexpr addr_t WX_ADDR   = 0xFF4B;
static constexpr addr_t PPU_LOW   = 0xFF40;
static constexpr addr_t PPU_HIGH  = 0xFF4B;

// Serial
static constexpr addr_t SB_ADDR     = 0xFF01;
static constexpr addr_t SC_ADDR     = 0xFF02;
static constexpr addr_t SERIAL_LOW  = 0xFF01;
static constexpr addr_t SERIAL_HIGH = 0xFF02;

// Timer
static constexpr addr_t DIV_ADDR   = 0xFF04;
static constexpr addr_t TIMA_ADDR  = 0xFF05;
static constexpr addr_t TMA_ADDR   = 0xFF06;
static constexpr addr_t TAC_ADDR   = 0xFF07;
static constexpr addr_t TIMER_LOW  = 0xFF04;
static constexpr addr_t TIMER_HIGH = 0xFF07;
