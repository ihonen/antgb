#pragma once

#include "types.hh"

#include <cstdint>

////////////////////////////////////////////////////////////////////////////////
// APU
////////////////////////////////////////////////////////////////////////////////

static constexpr memaddr_t NR10_ADDR = 0xFF10;
static constexpr memaddr_t NR11_ADDR = 0xFF11;
static constexpr memaddr_t NR12_ADDR = 0xFF12;
static constexpr memaddr_t NR13_ADDR = 0xFF13;
static constexpr memaddr_t NR14_ADDR = 0xFF14;
static constexpr memaddr_t NR21_ADDR = 0xFF16;
static constexpr memaddr_t NR22_ADDR = 0xFF17;
static constexpr memaddr_t NR23_ADDR = 0xFF18;
static constexpr memaddr_t NR24_ADDR = 0xFF19;
static constexpr memaddr_t NR30_ADDR = 0xFF1A;
static constexpr memaddr_t NR31_ADDR = 0xFF1B;
static constexpr memaddr_t NR32_ADDR = 0xFF1C;
static constexpr memaddr_t NR33_ADDR = 0xFF1D;
static constexpr memaddr_t NR34_ADDR = 0xFF1E;
static constexpr memaddr_t NR41_ADDR = 0xFF20;
static constexpr memaddr_t NR42_ADDR = 0xFF21;
static constexpr memaddr_t NR43_ADDR = 0xFF22;
static constexpr memaddr_t NR44_ADDR = 0xFF23;
static constexpr memaddr_t NR50_ADDR = 0xFF24;
static constexpr memaddr_t NR51_ADDR = 0xFF25;
static constexpr memaddr_t NR52_ADDR = 0xFF26;
static constexpr memaddr_t WAVERAM_LOW_ADDR = 0xFF30;
static constexpr memaddr_t WAVERAM_HIGH_ADDR = 0xFF3F;
static constexpr memaddr_t APU_LOW_ADDR = NR10_ADDR;
static constexpr memaddr_t APU_HIGH_ADDR = WAVERAM_HIGH_ADDR;

////////////////////////////////////////////////////////////////////////////////
// INTERRUPTS
////////////////////////////////////////////////////////////////////////////////

static constexpr memaddr_t IE_ADDR = 0xFFFF;
static constexpr memaddr_t IF_ADDR = 0xFF0F;

////////////////////////////////////////////////////////////////////////////////
// JOYPAD
////////////////////////////////////////////////////////////////////////////////

static constexpr memaddr_t JOYP_ADDR = 0xFF00;
static constexpr memaddr_t JOYPAD_LOW_ADDR = JOYP_ADDR;
static constexpr memaddr_t JOYPAD_HIGH_ADDR = JOYP_ADDR;

////////////////////////////////////////////////////////////////////////////////
// PPU
////////////////////////////////////////////////////////////////////////////////

static constexpr memaddr_t LCDC_ADDR = 0xFF40;
static constexpr memaddr_t STAT_ADDR = 0xFF41;
static constexpr memaddr_t SCY_ADDR = 0xFF42;
static constexpr memaddr_t SCX_ADDR = 0xFF43;
static constexpr memaddr_t LY_ADDR = 0xFF44;
static constexpr memaddr_t LYC_ADDR = 0xFF45;
static constexpr memaddr_t DMA_ADDR = 0xFF46;
static constexpr memaddr_t BGP_ADDR = 0xFF47;
static constexpr memaddr_t OBP0_ADDR = 0xFF48;
static constexpr memaddr_t OBP1_ADDR = 0xFF49;
static constexpr memaddr_t WY_ADDR = 0xFF4A;
static constexpr memaddr_t WX_ADDR = 0xFF4B;
static constexpr memaddr_t PPU_LOW_ADDR = LCDC_ADDR;
static constexpr memaddr_t PPU_HIGH_ADDR = WX_ADDR;

////////////////////////////////////////////////////////////////////////////////
// SERIAL
////////////////////////////////////////////////////////////////////////////////

static constexpr memaddr_t SB_ADDR = 0xFF01;
static constexpr memaddr_t SC_ADDR = 0xFF02;
static constexpr memaddr_t SERIAL_LOW_ADDR = SB_ADDR;
static constexpr memaddr_t SERIAL_HIGH_ADDR = SC_ADDR;

////////////////////////////////////////////////////////////////////////////////
// TIMER
////////////////////////////////////////////////////////////////////////////////

static constexpr memaddr_t DIV_ADDR = 0xFF04;
static constexpr memaddr_t TIMA_ADDR = 0xFF05;
static constexpr memaddr_t TMA_ADDR = 0xFF06;
static constexpr memaddr_t TAC_ADDR = 0xFF07;
static constexpr memaddr_t TIMER_LOW_ADDR = DIV_ADDR;
static constexpr memaddr_t TIMER_HIGH_ADDR = TAC_ADDR;
