#include "disassembler.hh"

#include "../core/cpu.hh"

#include <map>
using namespace std;
/*
#define imm8 to_string(extract_immediate8(instruction))
#define imm16 to_string(extract_immediate16(instruction))
*/

static uint8_t extract_immediate8(const uint8_t* instruction)
{
    return instruction[1];
}

static uint16_t extract_immediate16(const uint8_t* instruction)
{
    return (static_cast<uint16_t>(instruction[1])) |
           (static_cast<uint16_t>(instruction[2]) << 8);
}

Disassembler::Disassembler()
{

}

std::string Disassembler::disassemble(void* instruction_)
{
    if (!instruction_) return "-";

    uint8_t* instruction = (uint8_t*)instruction_;
    char imm8_[32];
    char imm16_[32];

    sprintf(imm8_, "%X", extract_immediate8(instruction));
    sprintf(imm16_, "%X", extract_immediate16(instruction));
    std::string imm8(imm8_);
    std::string imm16(imm16_);

    std::string mnemonic = "";

    // Irregular opcodes:
    switch (*instruction)
    {
        case 0x00: return "NOP";
        case 0x01: return "LD BC, $" + imm16;
        case 0x02: return "LD (BC), A";
        case 0x03: return "INC BC";
        case 0x04: return "INC B";
        case 0x05: return "DEC B";
        case 0x06: return "LD B, $" + imm8;
        case 0x07: return "RLCA";
        case 0x08: return "LD ($" + imm16 + "), SP";
        case 0x09: return "ADD HL, BC";
        case 0x0A: return "LD A, (BC)";
        case 0x0B: return "DEC BC";
        case 0x0C: return "INC C";
        case 0x0D: return "DEC C";
        case 0x0E: return "LD C, $" + imm8;
        case 0x0F: return "RRCA";
        case 0x10: return "STOP";
        case 0x11: return "LD DE, $" + imm16;
        case 0x12: return "LD (DE), A";
        case 0x13: return "INC DE";
        case 0x14: return "INC D";
        case 0x15: return "DEC D";
        case 0x16: return "LD D, $" + imm8;
        case 0x17: return "RLA";
        case 0x18: return "JR $" + imm8;
        case 0x19: return "ADD HL, DE";
        case 0x1A: return "LD A, (DE)";
        case 0x1B: return "DEC DE";
        case 0x1C: return "INC E";
        case 0x1D: return "DEC E";
        case 0x1E: return "LD E, $" + imm8;
        case 0x1F: return "RRA";
        case 0x20: return "JR NZ, $" + imm8;
        case 0x21: return "LD HL, $" + imm16;
        case 0x22: return "LD (HLI), A";
        case 0x23: return "INC HL";
        case 0x24: return "INC H";
        case 0x25: return "DEC H";
        case 0x26: return "LD H, $" + imm8;
        case 0x27: return "DAA";
        case 0x28: return "JR Z, $" + imm8;
        case 0x29: return "ADD HL, HL";
        case 0x2A: return "LD A, (HLI)";
        case 0x2B: return "DEC HL";
        case 0x2C: return "INC L";
        case 0x2D: return "DEC L";
        case 0x2E: return "LD L, $" + imm8;
        case 0x2F: return "CPL";
        case 0x30: return "JR NC, $" + imm8;
        case 0x31: return "LD SP, $" + imm16;
        case 0x32: return "LD (HLD), A";
        case 0x33: return "INC SP";
        case 0x34: return "INC (HL)";
        case 0x35: return "DEC (HL)";
        case 0x36: return "LD (HL), " + imm8;
        case 0x37: return "SCF";
        case 0x38: return "JR C, $" + imm8;
        case 0x39: return "ADD HL, SP";
        case 0x3A: return "LD A, (HLD)";
        case 0x3B: return "DEC SP";
        case 0x3C: return "INC A";
        case 0x3D: return "DEC A";
        case 0x3E: return "LD A, $" + imm8;
        case 0x3F: return "CCF";
        case 0x77: return "LD (HL), A";
        case 0x76: return "HALT";
        case 0xC0: return "RET NZ";
        case 0xC1: return "POP BC";
        case 0xC2: return "JP NZ, $" + imm16;
        case 0xC3: return "JP $" + imm16;
        case 0xC4: return "CALL NZ, $" + imm16;
        case 0xC5: return "PUSH BC";
        case 0xC6: return "ADD A, $" + imm8;
        case 0xC7: return "RST $00";
        case 0xC8: return "RET Z";
        case 0xC9: return "RET";
        case 0xCA: return "JP Z, $" + imm16;
        case 0xCC: return "CALL Z, $" + imm16;
        case 0xCD: return "CALL, $" + imm16;
        case 0xCE: return "ADC A, $" + imm8;
        case 0xCF: return "RST $08";
        case 0xD0: return "RET NC";
        case 0xD1: return "POP DE";
        case 0xD2: return "JP NC, $" + imm16;
        case 0xD4: return "CALL NC, $" + imm16;
        case 0xD5: return "PUSH DE";
        case 0xD6: return "SUB $" + imm8;
        case 0xD7: return "RST $10";
        case 0xD8: return "RET C";
        case 0xD9: return "RETI";
        case 0xDA: return "JP C, $" + imm8;
        case 0xDC: return "CALL C, $" + imm16;
        case 0xDE: return "BX A, $" + imm8;
        case 0xDF: return "RST $18";
        case 0xE0: return "LD ($" + imm8 + "), A";
        case 0xE1: return "POP HL";
        case 0xE2: return "LD (C), A";
        case 0xE5: return "PUSH HL";
        case 0xE6: return "AND $" + imm8;
        case 0xE7: return "RST $20";
        case 0xE8: return "ADD SP, $" + imm8;
        case 0xE9: return "JP (HL)";
        case 0xEA: return "LD ($" + imm16 + "), A";
        case 0xEE: return "XOR $" + imm8;
        case 0xEF: return "RST $28";
        case 0xF0: return "LD A, ($" + imm8 + ")";
        case 0xF1: return "POP AF";
        case 0xF2: return "LD A, (C)";
        case 0xF3: return "DI";
        case 0xF4: return "PUSH AF";
        case 0xF5: return "OR $" + imm8;
        case 0xF6: return "OR $" + imm8;
        case 0xF7: return "RST $30";
        case 0xF8: return "LD HL, SP";
        case 0xF9: return "LD SP, HL";
        case 0xFA: return "LD A, ($" + imm16 + ")";
        case 0xFB: return "EI";
        case 0xFE: return "CP $" + imm8;
        case 0xFF: return "RST $38";
    }

    // Regular opcodes:
    if (*instruction >= 0x40 && *instruction <= 0xBF)
    {
        if (*instruction >= 0x40 && *instruction <= 0x47)
        {
            mnemonic += "LD B, ";
        }
        else if (*instruction >= 0x48 && *instruction <= 0x4F)
        {
            mnemonic += "LD C, ";
        }
        else if (*instruction >= 0x50 && *instruction <= 0x57)
        {
            mnemonic += "LD D, ";
        }
        else if (*instruction >= 0x58 && *instruction <= 0x5F)
        {
            mnemonic += "LD E, ";
        }
        else if (*instruction >= 0x60 && *instruction <= 0x67)
        {
            mnemonic += "LD H, ";
        }
        else if (*instruction >= 0x68 && *instruction <= 0x6F)
        {
            mnemonic += "";
        }
        else if (*instruction >= 0x70 && *instruction <= 0x76)
        {
            mnemonic += "LD (HL), ";
        }
        else if (*instruction >= 0x78 && *instruction <= 0x7F)
        {
            mnemonic += "LD A, ";
        }
        else if (*instruction >= 0x80 && *instruction <= 0x87)
        {
            mnemonic += "ADD A, ";
        }
        else if (*instruction >= 0x88 && *instruction <= 0x8F)
        {
            mnemonic += "ADC A, ";
        }
        else if (*instruction >= 0x90 && *instruction <= 0x97)
        {
            mnemonic += "SUB ";
        }
        else if (*instruction >= 0x98 && *instruction <= 0x9F)
        {
            mnemonic += "SBC, ";
        }
        else if (*instruction >= 0xA0 && *instruction <= 0xA7)
        {
            mnemonic += "AND ";
        }
        else if (*instruction >= 0xA8 && *instruction <= 0xAF)
        {
            mnemonic += "XOR ";
        }
        else if (*instruction >= 0xB0 && *instruction <= 0xB7)
        {
            mnemonic += "OR ";
        }
        else if (*instruction >= 0xB8 && *instruction <= 0xBF)
        {
            mnemonic += "CP ";
        }
    }
    else if (*instruction == 0xCB)
    {
        // Irregular instructions:
        switch (instruction[1])
        {
            case 0x37: return "SWAP A";
        }

        // Regular instructions:
        if (instruction[1] >= 0x00 && instruction[1] <= 0x07)
        {
            mnemonic += "RLC ";
        }
        else if (instruction[1] >= 0x08 && instruction[1] <= 0x0F)
        {
            mnemonic += "RRC ";
        }
        else if (instruction[1] >= 0x10 && instruction[1] <= 0x17)
        {
            mnemonic += "RL";
        }
        else if (instruction[1] >= 0x18 && instruction[1] <= 0x1F)
        {
            mnemonic += "RR ";
        }
        else if (instruction[1] >= 0x20 && instruction[1] <= 0x27)
        {
            mnemonic += "SLA ";
        }
        else if (instruction[1] >= 0x28 && instruction[1] <= 0x2F)
        {
            mnemonic += "SRA ";
        }
        else if (instruction[1] >= 0x38 && instruction[1] <= 0x3F)
        {
            mnemonic += "SRL ";
        }
        else if (instruction[1] >= 0x40 && instruction[1] <= 0x47)
        {
            mnemonic += "BIT 0, ";
        }
        else if (instruction[1] >= 0x48 && instruction[1] <= 0x4F)
        {
            mnemonic += "BIT 1, ";
        }
        else if (instruction[1] >= 0x50 && instruction[1] <= 0x57)
        {
            mnemonic += "BIT 2, ";
        }
        else if (instruction[1] >= 0x58 && instruction[1] <= 0x5F)
        {
            mnemonic += "BIT 3, ";
        }
        else if (instruction[1] >= 0x60 && instruction[1] <= 0x67)
        {
            mnemonic += "BIT 4, ";
        }
        else if (instruction[1] >= 0x68 && instruction[1] <= 0x6F)
        {
            mnemonic += "BIT 5, ";
        }
        else if (instruction[1] >= 0x70 && instruction[1] <= 0x77)
        {
            mnemonic += "BIT 6, ";
        }
        else if (instruction[1] >= 0x78 && instruction[1] <= 0x7F)
        {
            mnemonic += "BIT 7, ";
        }
        else if (instruction[1] >= 0x80 && instruction[1] <= 0x87)
        {
            mnemonic += "RES 0, ";
        }
        else if (instruction[1] >= 0x88 && instruction[1] <= 0x8F)
        {
            mnemonic += "RES 1, ";
        }
        else if (instruction[1] >= 0x90 && instruction[1] <= 0x97)
        {
            mnemonic += "RES 2, ";
        }
        else if (instruction[1] >= 0x98 && instruction[1] <= 0x9F)
        {
            mnemonic += "RES 3, ";
        }
        else if (instruction[1] >= 0xA0 && instruction[1] <= 0xA7)
        {
            mnemonic += "RES 4, ";
        }
        else if (instruction[1] >= 0xA8 && instruction[1] <= 0xAF)
        {
            mnemonic += "RES 5, ";
        }
        else if (instruction[1] >= 0xB0 && instruction[1] <= 0xB7)
        {
            mnemonic += "RES 6, ";
        }
        else if (instruction[1] >= 0xB8 && instruction[1] <= 0xBF)
        {
            mnemonic += "RES 7, ";
        }
        else if (instruction[1] >= 0xC0 && instruction[1] <= 0xC7)
        {
            mnemonic += "SET 0, ";
        }
        else if (instruction[1] >= 0xC8 && instruction[1] <= 0xCF)
        {
            mnemonic += "SET 1, ";
        }
        else if (instruction[1] >= 0xD0 && instruction[1] <= 0xD7)
        {
            mnemonic += "SET 2, ";
        }
        else if (instruction[1] >= 0xD8 && instruction[1] <= 0xDF)
        {
            mnemonic += "SET 3, ";
        }
        else if (instruction[1] >= 0xE0 && instruction[1] <= 0xE7)
        {
            mnemonic += "SET 4, ";
        }
        else if (instruction[1] >= 0xE8 && instruction[1] <= 0xEF)
        {
            mnemonic += "SET 5, ";
        }
        else if (instruction[1] >= 0xF0 && instruction[1] <= 0xF7)
        {
            mnemonic += "SET 6, ";
        }
        else if (instruction[1] >= 0xF8 && instruction[1] <= 0xFF)
        {
            mnemonic += "SET 7, ";
        }
        else
        {
            volatile uint8_t second_byte = instruction[1];
            return "-";
        }
    }
    else
    {
        return "-";
    }

    if (instruction[0] == 0xCB)
    {
        switch (instruction[1] & 0x0F)
        {
            case 0x00: case 0x08: return mnemonic + "B";
            case 0x01: case 0x09: return mnemonic + "C";
            case 0x02: case 0x0A: return mnemonic + "D";
            case 0x03: case 0x0B: return mnemonic + "E";
            case 0x04: case 0x0C: return mnemonic + "H";
            case 0x05: case 0x0D: return mnemonic + "L";
            case 0x06: case 0x0E: return mnemonic + "(HL)";
            case 0x07: case 0x0F: return mnemonic + "A";
        }
    }
    else
    {
        switch (instruction[0] & 0x0F)
        {
            case 0x00: case 0x08: return mnemonic + "B";
            case 0x01: case 0x09: return mnemonic + "C";
            case 0x02: case 0x0A: return mnemonic + "D";
            case 0x03: case 0x0B: return mnemonic + "E";
            case 0x04: case 0x0C: return mnemonic + "H";
            case 0x05: case 0x0D: return mnemonic + "L";
            case 0x06: case 0x0E: return mnemonic + "(HL)";
            case 0x07: case 0x0F: return mnemonic + "A";
        }
    }

    return mnemonic;
}

size_t Disassembler::instr_len(void* instruction)
{
    if (!instruction) return 0;

    uint8_t first_byte = ((uint8_t*)instruction)[0];
    if (first_byte != 0xCB)
    {
        return Cpu::INSTR_TABLE[first_byte].len_bytes;
    }
    else
    {
        uint8_t second_byte = ((uint8_t*)instruction)[1];
        return Cpu::CB_INSTR_TABLE[second_byte].len_bytes;
    }
}
