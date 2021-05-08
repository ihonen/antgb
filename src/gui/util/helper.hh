#pragma once

#include <QString>
#include "util/macros.hh"

FORCE_INLINE QString hexstr8(uint8_t hex_number)
{
    return QString::number(hex_number, 16).rightJustified(2, '0');
}

FORCE_INLINE QString hexstr16(uint16_t hex_number)
{
    return QString::number(hex_number, 16).rightJustified(4, '0');
}

FORCE_INLINE QString hexstr(uint8_t* array, size_t size, QString separator = " ")
{
    QString ret;
    for (size_t i = 0; i < size; ++i)
    {
        ret += QString::number(array[i], 16).rightJustified(2, '0');
        if (i != size - 1)
            ret += separator;
    }

    return ret;
}

FORCE_INLINE const QString& register_name(uint16_t address)
{
    static const QString NONE = "";
    static const QString JOYP = "JOYP";
    static const QString SB = "SB";
    static const QString SC = "SC";
    static const QString DIV = "DIV";
    static const QString TIMA = "TIMA";
    static const QString TMA = "TMA";
    static const QString TAC = "TAC";
    static const QString IF = "IF";
    static const QString NR10 = "NR10";
    static const QString NR11 = "NR11";
    static const QString NR12 = "NR12";
    static const QString NR13 = "NR13";
    static const QString NR14 = "NR14";
    static const QString NR21 = "NR21";
    static const QString NR22 = "NR22";
    static const QString NR23 = "NR23";
    static const QString NR24 = "NR24";
    static const QString NR30 = "NR30";
    static const QString NR31 = "NR31";
    static const QString NR32 = "NR32";
    static const QString NR33 = "NR33";
    static const QString NR34 = "NR34";
    static const QString NR41 = "NR41";
    static const QString NR42 = "NR42";
    static const QString NR43 = "NR43";
    static const QString NR44 = "NR44";
    static const QString NR50 = "NR50";
    static const QString NR51 = "NR51";
    static const QString NR52 = "NR51";

    static const QString WAVE_PATTERN_RAM = "Wave RAM";

    static const QString LCDC = "LCDC";
    static const QString STAT = "STAT";
    static const QString SCY = "SCY";
    static const QString SCX = "SCX";
    static const QString LY = "LY";
    static const QString LYC = "LYC";
    static const QString DMA = "DMA";
    static const QString BGP = "BGP";
    static const QString OBP0 = "OBP0";
    static const QString OBP1 = "OBP1";
    static const QString WY = "WY";
    static const QString WX = "WX";
    static const QString IE = "IE";

    switch (address)
    {
        case 0xFF00: return JOYP;
        case 0xFF01: return SB;
        case 0xFF02: return SC;
        case 0xFF04: return DIV;
        case 0xFF05: return TIMA;
        case 0xFF06: return TMA;
        case 0xFF07: return TAC;
        case 0xFF0F: return IF;
        case 0xFF10: return NR10;
        case 0xFF11: return NR11;
        case 0xFF12: return NR12;
        case 0xFF13: return NR13;
        case 0xFF14: return NR14;
        case 0xFF16: return NR21;
        case 0xFF17: return NR22;
        case 0xFF18: return NR23;
        case 0xFF19: return NR24;
        case 0xFF1A: return NR30;
        case 0xFF1B: return NR31;
        case 0xFF1C: return NR32;
        case 0xFF1D: return NR33;
        case 0xFF1E: return NR34;
        case 0xFF20: return NR41;
        case 0xFF21: return NR42;
        case 0xFF22: return NR43;
        case 0xFF23: return NR44;
        case 0xFF24: return NR50;
        case 0xFF25: return NR51;
        case 0xFF26: return NR52;
        case 0xFF30: return WAVE_PATTERN_RAM;
        case 0xFF31: return WAVE_PATTERN_RAM;
        case 0xFF32: return WAVE_PATTERN_RAM;
        case 0xFF33: return WAVE_PATTERN_RAM;
        case 0xFF34: return WAVE_PATTERN_RAM;
        case 0xFF35: return WAVE_PATTERN_RAM;
        case 0xFF36: return WAVE_PATTERN_RAM;
        case 0xFF37: return WAVE_PATTERN_RAM;
        case 0xFF38: return WAVE_PATTERN_RAM;
        case 0xFF39: return WAVE_PATTERN_RAM;
        case 0xFF3A: return WAVE_PATTERN_RAM;
        case 0xFF3B: return WAVE_PATTERN_RAM;
        case 0xFF3C: return WAVE_PATTERN_RAM;
        case 0xFF3D: return WAVE_PATTERN_RAM;
        case 0xFF3E: return WAVE_PATTERN_RAM;
        case 0xFF3F: return WAVE_PATTERN_RAM;
        case 0xFF40: return LCDC;
        case 0xFF41: return STAT;
        case 0xFF42: return SCY;
        case 0xFF43: return SCX;
        case 0xFF44: return LY;
        case 0xFF45: return LYC;
        case 0xFF46: return DMA;
        case 0xFF47: return BGP;
        case 0xFF48: return OBP0;
        case 0xFF49: return OBP1;
        case 0xFF4A: return WY;
        case 0xFF4B: return WX;
        case 0xFFFF: return IE;
        default:
            break;
    }

    return NONE;
}

FORCE_INLINE const QString& memory_abbreviation(uint16_t address)
{
    static const QString ROM0 =   "ROM0";
    static const QString ROM1 =   "ROM1";
    static const QString VRA0 =   "VRA0";
    static const QString SRA0 =   "SRA0";
    static const QString WRA0 =   "WRA0";
    static const QString WRA1 =   "WRA1";
    static const QString ECH0 =   "ECH0";
    static const QString ECH1 =   "ECH1";
    static const QString OAM =    "OAM ";
    static const QString UNUSED = "--- ";
    static const QString IO =     "I/O ";
    static const QString HRAM =   "HRAM";

    if (address <= 0x3FFF) return ROM0;
    else if (address <= 0x7FFF) return ROM1;
    else if (address <= 0x9FFF) return VRA0;
    else if (address <= 0xBFFF) return SRA0;
    else if (address <= 0xCFFF) return WRA0;
    else if (address <= 0xDFFF) return WRA1;
    else if (address <= 0xEFFF) return ECH0;
    else if (address <= 0xFDFF) return ECH1;
    else if (address <= 0xFE9B) return OAM;
    else if (address <= 0xFEFF) return UNUSED;
    else if (address <= 0xFF7F) return IO ;
    else if (address <= 0xFFFE) return HRAM;
    else if (address == 0xFFFF) return IO ;

    return UNUSED;
}

QString escape_regex_special_chars(const QString& string);
