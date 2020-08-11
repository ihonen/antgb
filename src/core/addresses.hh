#pragma once

#pragma once

#include "types.hh"

//------------------------------------------------------------------------------
// MEMORY REGIONS
//------------------------------------------------------------------------------

namespace antgb
{
static const addr_t BOOTROM_LOW  = 0x0000;
static const addr_t BOOTROM_HIGH = 0x00FF;
static const addr_t BOOTROM_SIZE = 0x0100;

static const addr_t ROM0_LOW  = 0x0000;
static const addr_t ROM0_HIGH = 0x3FFF;
static const addr_t ROM0_SIZE = 0x4000;

static const addr_t ROM1_LOW  = 0x4000;
static const addr_t ROM1_HIGH = 0x7FFF;
static const addr_t ROM1_SIZE = 0x4000;

static const addr_t VRAM_LOW  = 0x8000;
static const addr_t VRAM_HIGH = 0x9FFF;
static const addr_t VRAM_SIZE = 0x2000;

static const addr_t ERAM_LOW  = 0xA000;
static const addr_t ERAM_HIGH = 0xBFFF;
static const addr_t ERAM_SIZE = 0x2000;

static const addr_t WRAM0_LOW  = 0xC000;
static const addr_t WRAM0_HIGH = 0xCFFF;
static const addr_t WRAM0_SIZE = 0x1000;

static const addr_t WRAM1_LOW  = 0xD000;
static const addr_t WRAM1_HIGH = 0xDFFF;
static const addr_t WRAM1_SIZE = 0x1000;

static const addr_t ECHO_LOW  = 0xE000;
static const addr_t ECHO_HIGH = 0xFDFF;
static const addr_t ECHO_SIZE = 0x1E00;

static const addr_t OAM_LOW  = 0xFE00;
static const addr_t OAM_HIGH = 0xFE9F;
static const addr_t OAM_SIZE = 0x00A0;

static const addr_t UNUSABLE_LOW  = 0xFEA0;
static const addr_t UNUSABLE_HIGH = 0xFEFF;
static const addr_t UNUSABLE_SIZE = 0x0060;

static const addr_t IO_LOW  = 0xFF00;
static const addr_t IO_HIGH = 0xFF7F;
static const addr_t IO_SIZE = 0x0080;

static const addr_t HRAM_LOW  = 0xFF80;
static const addr_t HRAM_HIGH = 0xFFFE;
static const addr_t HRAM_SIZE = 0x007F;

// PPU
static const addr_t TILERAM_LOW  = 0x8000;
static const addr_t TILERAM_HIGH = 0x97FF;
static const addr_t TILERAM_SIZE = 0x1800;

static const addr_t BGMAP1_LOW  = 0x9800;
static const addr_t BGMAP1_HIGH = 0x9BFF;
static const addr_t BGMAP_SIZE  = 0x4000;

static const addr_t BGMAP2_LOW  = 0x9C00;
static const addr_t BGMAP2_HIGH = 0x9FFF;

// APU
static const addr_t WAVERAM_LOW  = 0xFF30;
static const addr_t WAVERAM_HIGH = 0xFF3F;
static const addr_t WAVERAM_SIZE = 0x0010;

//------------------------------------------------------------------------------
// HARDWARE REGISTERS
//------------------------------------------------------------------------------

// APU
static const addr_t REG_NR10 = 0xFF10;
static const addr_t REG_NR11 = 0xFF11;
static const addr_t REG_NR12 = 0xFF12;
static const addr_t REG_NR13 = 0xFF13;
static const addr_t REG_NR14 = 0xFF14;
static const addr_t REG_NR21 = 0xFF16;
static const addr_t REG_NR22 = 0xFF17;
static const addr_t REG_NR23 = 0xFF18;
static const addr_t REG_NR24 = 0xFF19;
static const addr_t REG_NR30 = 0xFF1A;
static const addr_t REG_NR31 = 0xFF1B;
static const addr_t REG_NR32 = 0xFF1C;
static const addr_t REG_NR33 = 0xFF1D;
static const addr_t REG_NR34 = 0xFF1E;
static const addr_t REG_NR41 = 0xFF20;
static const addr_t REG_NR42 = 0xFF21;
static const addr_t REG_NR43 = 0xFF22;
static const addr_t REG_NR44 = 0xFF23;
static const addr_t REG_NR50 = 0xFF24;
static const addr_t REG_NR51 = 0xFF25;
static const addr_t REG_NR52 = 0xFF26;

// Interrupts
static const addr_t REG_IE = 0xFFFF;
static const addr_t REG_IF = 0xFF0F;

// Joypad
static const addr_t REG_JOYP = 0xFF00;

// PPU
static const addr_t REG_LCDC = 0xFF40;
static const addr_t REG_STAT = 0xFF41;
static const addr_t REG_SCY  = 0xFF42;
static const addr_t REG_SCX  = 0xFF43;
static const addr_t REG_LY   = 0xFF44;
static const addr_t REG_LYC  = 0xFF45;
static const addr_t REG_DMA  = 0xFF46;
static const addr_t REG_BGP  = 0xFF47;
static const addr_t REG_OBP0 = 0xFF48;
static const addr_t REG_OBP1 = 0xFF49;
static const addr_t REG_WY   = 0xFF4A;
static const addr_t REG_WX   = 0xFF4B;

// Serial
static const addr_t REG_SB = 0xFF01;
static const addr_t REG_SC = 0xFF02;

// Timer
static const addr_t REG_DIV  = 0xFF04;
static const addr_t REG_TIMA = 0xFF05;
static const addr_t REG_TMA  = 0xFF06;
static const addr_t REG_TAC  = 0xFF07;

//------------------------------------------------------------------------------
// CUSTOM ADDRESSES
//------------------------------------------------------------------------------

// CPU
static const addr_t REG_A  = 0x10001;
static const addr_t REG_F  = 0x10002;
static const addr_t REG_B  = 0x10003;
static const addr_t REG_C  = 0x10004;
static const addr_t REG_D  = 0x10005;
static const addr_t REG_E  = 0x10006;
static const addr_t REG_H  = 0x10007;
static const addr_t REG_L  = 0x10008;
static const addr_t REG_AF = 0x10009;
static const addr_t REG_BC = 0x1000A;
static const addr_t REG_DE = 0x1000B;
static const addr_t REG_HL = 0x1000C;
static const addr_t REG_PC = 0x1000D;
static const addr_t REG_SP = 0x1000E;

// Interrupts
static const addr_t REG_IME = 0x1000F;

//------------------------------------------------------------------------------
// BIT POSITIONS
//------------------------------------------------------------------------------

// CPU
static const addr_t FLAG_C = 4;
static const addr_t FLAG_H = 5;
static const addr_t FLAG_N = 6;
static const addr_t FLAG_Z = 7;

// Interrupts
static const addr_t IF_VBLANK_INT  = 0;
static const addr_t IF_LCDSTAT_INT = 1;
static const addr_t IF_TIMER_INT   = 2;
static const addr_t IF_SERIAL_INT  = 3;
static const addr_t IF_JOYPAD_INT  = 4;
static const addr_t IE_VBLANK_INT  = 0;
static const addr_t IE_LCDSTAT_INT = 1;
static const addr_t IE_TIMER_INT   = 2;
static const addr_t IE_SERIAL_INT  = 3;
static const addr_t IE_JOYPAD_INT  = 4;

// Joypad
static const addr_t JOYP_RIGHT_OR_A            = 0;
static const addr_t JOYP_LEFT_OR_B             = 1;
static const addr_t JOYP_UP_OR_SELECT          = 2;
static const addr_t JOYP_DOWN_OR_START         = 3;
static const addr_t JOYP_SELECT_DIRECTION_KEYS = 4;
static const addr_t JOYP_SELECT_BUTTON_KEYS    = 5;

// PPU
static const addr_t LCDC_BG_WINDOW_ENABLE      = 0;
static const addr_t LCDC_SPRITE_ENABLE         = 1;
static const addr_t LCDC_SPRITE_SIZE           = 2;
static const addr_t LCDC_BG_TILEMAP_SELECT     = 3;
static const addr_t LCDC_TILEDATA_SELECT       = 4;
static const addr_t LCDC_WINDOW_ENABLE         = 5;
static const addr_t LCDC_WINDOW_TILEMAP_SELECT = 6;
static const addr_t LCDC_LCD_ENABLE            = 7;
static const addr_t STAT_PPUMODE_FLAG_MASK     = 0x03;
static const addr_t STAT_LYC_LY_COINCIDENCE    = 2;
static const addr_t STAT_HBLANK_INT_ENABLE     = 3;
static const addr_t STAT_VBLANK_INT_ENABLE     = 4;
static const addr_t STAT_OAM_INT_ENABLE        = 5;
static const addr_t STAT_LYC_INT_ENABLE        = 6;

// Serial
static const addr_t SC_SHIFT_CLOCK    = 0;
static const addr_t SC_TRANSFER_START = 7;

} // namespace antgb
