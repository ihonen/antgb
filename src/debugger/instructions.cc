#include "instructions.hh"

#include "debugcore.hh"
#include "../util/macros.hh"

const std::array<Opcode, 256> OPCODES =
{{
    {1, 4,  4,  0x00, {}, {}, false, false},
    {3, 12, 12, 0x01, {RegB, RegC}, {}, false, false},
    {1, 8,  8,  0x02, {}, {RegA, RegB, RegC}, false, false},
    {1, 8,  8,  0x03, {RegB, RegC}, {}, false, false},
    {1, 4,  4,  0x04, {RegB}, {}, true, false},
    {1, 4,  4,  0x05, {RegB}, {}, true, false},
    {2, 8,  8,  0x06, {RegB}, {}, false, false},
    {1, 4,  4,  0x07, {RegA}, {}, true, false},
    {3, 20, 20, 0x08, {}, {RegSP}, false, false},
    {1, 8,  8,  0x09, {RegH, RegL}, {RegB, RegC}, true, false},
    {1, 8,  8,  0x0A, {RegA}, {RegB, RegC}, false, false},
    {1, 8,  8,  0x0B, {RegB, RegC}, {}, false, false},
    {1, 4,  4,  0x0C, {RegC}, {}, true, false},
    {1, 4,  4,  0x0D, {RegC}, {}, true, false},
    {2, 8,  8,  0x0E, {RegC}, {}, false, false},
    {1, 4,  4,  0x0F, {RegA}, {}, true, false},
    {2, 4,  4,  0x10, {}, {}, false, false},
    {3, 12, 12, 0x11, {RegD, RegE}, {}, false, false},
    {1, 8,  8,  0x12, {}, {RegA, RegD, RegE}, false, false},
    {1, 8,  8,  0x13, {RegD, RegE}, {}, false, false},
    {1, 4,  4,  0x14, {RegD}, {}, true, false},
    {1, 4,  4,  0x15, {RegD}, {}, true, false},
    {2, 8,  8,  0x16, {RegD}, {}, false, false},
    {1, 4,  4,  0x17, {RegA}, {}, true, false},
    {2, 12, 12, 0x18, {}, {}, false, false},
    {1, 8,  8,  0x19, {RegH, RegL}, {RegD, RegE}, true, false},
    {1, 8,  8,  0x1A, {RegA}, {RegD, RegE}, false, false},
    {1, 8,  8,  0x1B, {RegD, RegE}, {}, false, false},
    {1, 4,  4,  0x1C, {RegE}, {}, true, false},
    {1, 4,  4,  0x1D, {RegE}, {}, true, false},
    {2, 8,  8,  0x1E, {RegE}, {}, false, false},
    {1, 4,  4,  0x1F, {RegA}, {}, true, false},
    {2, 12, 8,  0x20, {}, {}, false, true},
    {3, 12, 12, 0x21, {RegH, RegL}, {}, false, false},
    {1, 8,  8,  0x22, {RegH, RegL}, {RegA, RegH, RegL}, false, false},
    {1, 8,  8,  0x23, {RegH, RegL}, {}, false, false},
    {1, 4,  4,  0x24, {RegH}, {}, true, false},
    {1, 4,  4,  0x25, {RegH}, {}, true, false},
    {2, 8,  8,  0x26, {RegH}, {}, false, false},
    {1, 4,  4,  0x27, {RegA}, {}, true, false},
    {2, 12, 8,  0x28, {}, {}, false, true},
    {1, 8,  8,  0x29, {RegH, RegL}, {RegH, RegL}, true, false},
    {1, 8,  8,  0x2A, {RegA, RegH, RegL}, {RegH, RegL}, false, false},
    {1, 8,  8,  0x2B, {RegH, RegL}, {}, false, false},
    {1, 4,  4,  0x2C, {RegL}, {}, true, false},
    {1, 4,  4,  0x2D, {RegL}, {}, true, false},
    {2, 8,  8,  0x2E, {RegL}, {}, false, false},
    {1, 4,  4,  0x2F, {RegA}, {}, true, false},
    {2, 12, 8,  0x30, {}, {}, false, true},
    {3, 12, 12, 0x31, {RegSP}, {}, false, false},
    {1, 8,  8,  0x32, {RegH, RegL}, {RegA, RegH, RegL}, false, false},
    {1, 8,  8,  0x33, {RegSP}, {}, false, false},
    {1, 12, 12, 0x34, {}, {RegH, RegL}, true, false},
    {1, 12, 12, 0x35, {}, {RegH, RegL}, true, false},
    {2, 12, 12, 0x36, {}, {RegH, RegL}, false, false},
    {1, 4,  4,  0x37, {}, {}, true, false},
    {2, 12, 8,  0x38, {}, {}, false, true},
    {1, 8,  8,  0x39, {RegH, RegL}, {RegSP}, true, false},
    {1, 8,  8,  0x3A, {RegA, RegH, RegL}, {RegH, RegL}, false, false},
    {1, 8,  8,  0x3B, {RegSP}, {}, false, false},
    {1, 4,  4,  0x3C, {RegA}, {}, true, false},
    {1, 4,  4,  0x3D, {RegA}, {}, true, false},
    {2, 8,  8,  0x3E, {RegA}, {}, false, false},
    {1, 4,  4,  0x3F, {}, {}, true, false},
    {1, 4,  4,  0x40, {RegB}, {RegB}, false, false},
    {1, 4,  4,  0x41, {RegB}, {RegC}, false, false},
    {1, 4,  4,  0x42, {RegB}, {RegD}, false, false},
    {1, 4,  4,  0x43, {RegB}, {RegE}, false, false},
    {1, 4,  4,  0x44, {RegB}, {RegH}, false, false},
    {1, 4,  4,  0x45, {RegB}, {RegL}, false, false},
    {1, 8,  8,  0x46, {RegB}, {RegH, RegL}, false, false},
    {1, 4,  4,  0x47, {RegB}, {RegA}, false, false},
    {1, 4,  4,  0x48, {RegC}, {RegB}, false, false},
    {1, 4,  4,  0x49, {RegC}, {}, false, false},
    {1, 4,  4,  0x4A, {RegC}, {RegD}, false, false},
    {1, 4,  4,  0x4B, {RegC}, {RegE}, false, false},
    {1, 4,  4,  0x4C, {RegC}, {RegH}, false, false},
    {1, 4,  4,  0x4D, {RegC}, {RegL}, false, false},
    {1, 8,  8,  0x4E, {RegC}, {RegH, RegL}, false, false},
    {1, 4,  4,  0x4F, {RegC}, {RegA}, false, false},
    {1, 4,  4,  0x50, {RegD}, {RegB}, false, false},
    {1, 4,  4,  0x51, {RegD}, {RegC}, false, false},
    {1, 4,  4,  0x52, {RegD}, {RegD}, false, false},
    {1, 4,  4,  0x53, {RegD}, {RegE}, false, false},
    {1, 4,  4,  0x54, {RegD}, {RegH}, false, false},
    {1, 4,  4,  0x55, {RegD}, {RegL}, false, false},
    {1, 8,  8,  0x56, {RegD}, {RegH, RegL}, false, false},
    {1, 4,  4,  0x57, {RegD}, {RegA}, false, false},
    {1, 4,  4,  0x58, {RegE}, {RegB}, false, false},
    {1, 4,  4,  0x59, {RegE}, {RegC}, false, false},
    {1, 4,  4,  0x5A, {RegE}, {RegD}, false, false},
    {1, 4,  4,  0x5B, {RegE}, {RegE}, false, false},
    {1, 4,  4,  0x5C, {RegE}, {RegH}, false, false},
    {1, 4,  4,  0x5D, {RegE}, {RegL}, false, false},
    {1, 8,  8,  0x5E, {RegE}, {RegH, RegL}, false, false},
    {1, 4,  4,  0x5F, {RegE}, {RegA}, false, false},
    {1, 4,  4,  0x60, {RegH}, {RegB}, false, false},
    {1, 4,  4,  0x61, {RegH}, {RegC}, false, false},
    {1, 4,  4,  0x62, {RegH}, {RegD}, false, false},
    {1, 4,  4,  0x63, {RegH}, {RegE}, false, false},
    {1, 4,  4,  0x64, {RegH}, {RegH}, false, false},
    {1, 4,  4,  0x65, {RegH}, {RegL}, false, false},
    {1, 8,  8,  0x66, {RegH}, {RegH, RegL}, false, false},
    {1, 4,  4,  0x67, {RegH}, {RegA}, false, false},
    {1, 4,  4,  0x68, {RegL}, {RegB}, false, false},
    {1, 4,  4,  0x69, {RegL}, {RegC}, false, false},
    {1, 4,  4,  0x6A, {RegL}, {RegD}, false, false},
    {1, 4,  4,  0x6B, {RegL}, {RegE}, false, false},
    {1, 4,  4,  0x6C, {RegL}, {RegH}, false, false},
    {1, 4,  4,  0x6D, {RegL}, {RegL}, false, false},
    {1, 8,  8,  0x6E, {RegL}, {RegH, RegL}, false, false},
    {1, 4,  4,  0x6F, {RegL}, {RegA}, false, false},
    {1, 8,  8,  0x70, {}, {RegB, RegH, RegL}, false, false},
    {1, 8,  8,  0x71, {}, {RegC, RegH, RegL}, false, false},
    {1, 8,  8,  0x72, {}, {RegD, RegH, RegL}, false, false},
    {1, 8,  8,  0x73, {}, {RegE, RegH, RegL}, false, false},
    {1, 8,  8,  0x74, {}, {RegH, RegL}, false, false},
    {1, 8,  8,  0x75, {}, {RegH, RegL}, false, false},
    {1, 4,  4,  0x76, {}, {}, false, false},
    {1, 8,  8,  0x77, {}, {RegA, RegH, RegL}, false, false},
    {1, 4,  4,  0x78, {RegA}, {RegB}, false, false},
    {1, 4,  4,  0x79, {RegA}, {RegC}, false, false},
    {1, 4,  4,  0x7A, {RegA}, {RegD}, false, false},
    {1, 4,  4,  0x7B, {RegA}, {RegE}, false, false},
    {1, 4,  4,  0x7C, {RegA}, {RegH}, false, false},
    {1, 4,  4,  0x7D, {RegA}, {RegL}, false, false},
    {1, 8,  8,  0x7E, {RegA}, {RegH, RegL}, false, false},
    {1, 4,  4,  0x7F, {RegA}, {RegA}, false, false},
    {1, 4,  4,  0x80, {RegA}, {RegB}, true, false},
    {1, 4,  4,  0x81, {RegA}, {RegC}, true, false},
    {1, 4,  4,  0x82, {RegA}, {RegD}, true, false},
    {1, 4,  4,  0x83, {RegA}, {RegE}, true, false},
    {1, 4,  4,  0x84, {RegA}, {RegH}, true, false},
    {1, 4,  4,  0x85, {RegA}, {RegL}, true, false},
    {1, 8,  8,  0x86, {RegA}, {RegH, RegL}, true, false},
    {1, 4,  4,  0x87, {RegA}, {RegA}, true, false},
    {1, 4,  4,  0x88, {RegA}, {RegB}, true, false},
    {1, 4,  4,  0x89, {RegA}, {RegC}, true, false},
    {1, 4,  4,  0x8A, {RegA}, {RegD}, true, false},
    {1, 4,  4,  0x8B, {RegA}, {RegE}, true, false},
    {1, 4,  4,  0x8C, {RegA}, {RegH}, true, false},
    {1, 4,  4,  0x8D, {RegA}, {RegL}, true, false},
    {1, 8,  8,  0x8E, {RegA}, {RegH, RegL}, true, false},
    {1, 4,  4,  0x8F, {RegA}, {RegA}, true, false},
    {1, 4,  4,  0x90, {RegA}, {RegB}, true, false},
    {1, 4,  4,  0x91, {RegA}, {RegC}, true, false},
    {1, 4,  4,  0x92, {RegA}, {RegD}, true, false},
    {1, 4,  4,  0x93, {RegA}, {RegE}, true, false},
    {1, 4,  4,  0x94, {RegA}, {RegH}, true, false},
    {1, 4,  4,  0x95, {RegA}, {RegL}, true, false},
    {1, 8,  8,  0x96, {RegA}, {RegH, RegL}, true, false},
    {1, 4,  4,  0x97, {RegA}, {RegA}, true, false},
    {1, 4,  4,  0x98, {RegA}, {RegB}, true, false},
    {1, 4,  4,  0x99, {RegA}, {RegC}, true, false},
    {1, 4,  4,  0x9A, {RegA}, {RegD}, true, false},
    {1, 4,  4,  0x9B, {RegA}, {RegE}, true, false},
    {1, 4,  4,  0x9C, {RegA}, {RegH}, true, false},
    {1, 4,  4,  0x9D, {RegA}, {RegL}, true, false},
    {1, 8,  8,  0x9E, {RegA}, {RegH, RegL}, true, false},
    {1, 4,  4,  0x9F, {RegA}, {RegA}, true, false},
    {1, 4,  4,  0xA0, {RegA}, {RegB}, true, false},
    {1, 4,  4,  0xA1, {RegA}, {RegC}, true, false},
    {1, 4,  4,  0xA2, {RegA}, {RegD}, true, false},
    {1, 4,  4,  0xA3, {RegA}, {RegE}, true, false},
    {1, 4,  4,  0xA4, {RegA}, {RegH}, true, false},
    {1, 4,  4,  0xA5, {RegA}, {RegL}, true, false},
    {1, 8,  8,  0xA6, {RegA}, {RegH, RegL}, true, false},
    {1, 4,  4,  0xA7, {RegA}, {RegA}, true, false},
    {1, 4,  4,  0xA8, {RegA}, {RegB}, true, false},
    {1, 4,  4,  0xA9, {RegA}, {RegC}, true, false},
    {1, 4,  4,  0xAA, {RegA}, {RegD}, true, false},
    {1, 4,  4,  0xAB, {RegA}, {RegE}, true, false},
    {1, 4,  4,  0xAC, {RegA}, {RegH}, true, false},
    {1, 4,  4,  0xAD, {RegA}, {RegL}, true, false},
    {1, 8,  8,  0xAE, {RegA}, {RegH, RegL}, true, false},
    {1, 4,  4,  0xAF, {RegA}, {RegA}, true, false},
    {1, 4,  4,  0xB0, {RegA}, {RegB}, true, false},
    {1, 4,  4,  0xB1, {RegA}, {RegC}, true, false},
    {1, 4,  4,  0xB2, {RegA}, {RegD}, true, false},
    {1, 4,  4,  0xB3, {RegA}, {RegE}, true, false},
    {1, 4,  4,  0xB4, {RegA}, {RegH}, true, false},
    {1, 4,  4,  0xB5, {RegA}, {RegL}, true, false},
    {1, 8,  8,  0xB6, {RegA}, {RegH, RegL}, true, false},
    {1, 4,  4,  0xB7, {RegA}, {RegA}, true, false},
    {1, 4,  4,  0xB8, {}, {RegA, RegB}, true, false},
    {1, 4,  4,  0xB9, {}, {RegA, RegC}, true, false},
    {1, 4,  4,  0xBA, {}, {RegA, RegD}, true, false},
    {1, 4,  4,  0xBB, {}, {RegA, RegE}, true, false},
    {1, 4,  4,  0xBC, {}, {RegA, RegH}, true, false},
    {1, 4,  4,  0xBD, {}, {RegA, RegL}, true, false},
    {1, 8,  8,  0xBE, {}, {RegA, RegH, RegL}, true, false},
    {1, 4,  4,  0xBF, {}, {RegA}, true, false},
    {1, 20, 8,  0xC0, {}, {}, false, true},
    {1, 12, 12, 0xC1, {RegB, RegC}, {}, false, false},
    {3, 16, 12, 0xC2, {}, {}, false, true},
    {3, 16, 16, 0xC3, {}, {}, false, false},
    {3, 24, 12, 0xC4, {}, {}, false, true},
    {1, 16, 16, 0xC5, {}, {RegB, RegC}, false, false},
    {2, 8,  8,  0xC6, {RegA}, {}, true, false},
    {1, 16, 16, 0xC7, {}, {}, false, false},
    {1, 20, 8,  0xC8, {}, {}, false, true},
    {1, 16, 16, 0xC9, {}, {}, false, false},
    {3, 16, 12, 0xCA, {}, {}, false, true},
    {1, 4,  4,  0xCB, {}, {}, false, false},
    {3, 24, 12, 0xCC, {}, {}, false, true},
    {3, 24, 24, 0xCD, {}, {}, false, false},
    {2, 8,  8,  0xCE, {RegA}, {}, true, false},
    {1, 16, 16, 0xCF, {}, {}, false, false},
    {1, 20, 8,  0xD0, {}, {}, false, true},
    {1, 12, 12, 0xD1, {RegD, RegE}, {}, false, false},
    {3, 16, 12, 0xD2, {}, {}, false, true},
    {0, 0,  0,  0xD3, {}, {}, false, false},
    {3, 24, 12, 0xD4, {}, {}, false, true},
    {1, 16, 16, 0xD5, {}, {RegD, RegE}, false, false},
    {2, 8,  8,  0xD6, {RegA}, {}, true, false},
    {1, 16, 16, 0xD7, {}, {}, false, false},
    {1, 20, 8,  0xD8, {}, {}, false, true},
    {1, 16, 16, 0xD9, {RegIME}, {}, false, false},
    {3, 16, 12, 0xDA, {}, {}, false, true},
    {0, 0,  0,  0xDB, {}, {}, false, false},
    {3, 24, 12, 0xDC, {}, {}, false, true},
    {0, 0,  0,  0xDD, {}, {}, false, false},
    {2, 8,  8,  0xDE, {RegA}, {}, true, false},
    {1, 16, 16, 0xDF, {}, {}, false, false},
    {2, 12, 12, 0xE0, {}, {RegA}, false, false},
    {1, 12, 12, 0xE1, {RegH, RegL}, {}, false, false},
    {1, 8,  8,  0xE2, {}, {RegA, RegC}, false, false},
    {0, 0,  0,  0xE3, {}, {}, false, false},
    {0, 0,  0,  0xE4, {}, {}, false, false},
    {1, 16, 16, 0xE5, {}, {RegH, RegL}, false, false},
    {2, 8,  8,  0xE6, {RegA}, {}, true, false},
    {1, 16, 16, 0xE7, {}, {}, false, false},
    {2, 16, 16, 0xE8, {RegSP}, {}, true, false},
    {1, 4,  4,  0xE9, {}, {RegH, RegL}, false, false},
    {3, 16, 16, 0xEA, {}, {RegA}, false, false},
    {0, 0,  0,  0xEB, {}, {}, false, false},
    {0, 0,  0,  0xEC, {}, {}, false, false},
    {0, 0,  0,  0xED, {}, {}, false, false},
    {2, 8,  8,  0xEE, {RegA}, {}, true, false},
    {1, 16, 16, 0xEF, {}, {}, false, false},
    {2, 12, 12, 0xF0, {RegA}, {}, false, false},
    {1, 12, 12, 0xF1, {RegA}, {}, true, false},
    {1, 8,  8,  0xF2, {RegA}, {RegC}, false, false},
    {1, 4,  4,  0xF3, {RegIME}, {}, false, false},
    {0, 0,  0,  0xF4, {}, {}, false, false},
    {1, 16, 16, 0xF5, {}, {RegA}, false, true},
    {2, 8,  8,  0xF6, {RegA}, {}, true, false},
    {1, 16, 16, 0xF7, {}, {}, false, false},
    {2, 12, 12, 0xF8, {RegH, RegL}, {RegSP}, true, false},
    {1, 8,  8,  0xF9, {RegSP}, {RegH, RegL}, false, false},
    {3, 16, 16, 0xFA, {RegA}, {}, false, false},
    {1, 1,  4,  0xFB, {RegIME}, {}, false, false},
    {0, 0,  0,  0xFC, {}, {}, false, false},
    {0, 0,  0,  0xFD, {}, {}, false, false},
    {2, 8,  8,  0xFE, {}, {RegA}, true, false},
    {1, 16, 16, 0xFF, {}, {}, false, false}
}};

const std::array<Opcode, 256> CB_OPCODES =
{{
    {2, 8,  8,  0x00, {RegB}, {}, true, false},
    {2, 8,  8,  0x01, {RegC}, {}, true, false},
    {2, 8,  8,  0x02, {RegD}, {}, true, false},
    {2, 8,  8,  0x03, {RegE}, {}, true, false},
    {2, 8,  8,  0x04, {RegH}, {}, true, false},
    {2, 8,  8,  0x05, {RegL}, {}, true, false},
    {2, 16, 16, 0x06, {}, {RegH, RegL}, true, false},
    {2, 8,  8,  0x07, {RegA}, {}, true, false},
    {2, 8,  8,  0x08, {RegB}, {}, true, false},
    {2, 8,  8,  0x09, {RegC}, {}, true, false},
    {2, 8,  8,  0x0A, {RegD}, {}, true, false},
    {2, 8,  8,  0x0B, {RegE}, {}, true, false},
    {2, 8,  8,  0x0C, {RegH}, {}, true, false},
    {2, 8,  8,  0x0D, {RegL}, {}, true, false},
    {2, 16, 16, 0x0E, {}, {RegH, RegL}, true, false},
    {2, 8,  8,  0x0F, {RegA}, {}, true, false},
    {2, 8,  8,  0x10, {RegB}, {}, true, false},
    {2, 8,  8,  0x11, {RegC}, {}, true, false},
    {2, 8,  8,  0x12, {RegD}, {}, true, false},
    {2, 8,  8,  0x13, {RegE}, {}, true, false},
    {2, 8,  8,  0x14, {RegH}, {}, true, false},
    {2, 8,  8,  0x15, {RegL}, {}, true, false},
    {2, 16, 16, 0x16, {}, {RegH, RegL}, true, false},
    {2, 8,  8,  0x17, {RegA}, {}, true, false},
    {2, 8,  8,  0x18, {RegB}, {}, true, false},
    {2, 8,  8,  0x19, {RegC}, {}, true, false},
    {2, 8,  8,  0x1A, {RegD}, {}, true, false},
    {2, 8,  8,  0x1B, {RegE}, {}, true, false},
    {2, 8,  8,  0x1C, {RegH}, {}, true, false},
    {2, 8,  8,  0x1D, {RegL}, {}, true, false},
    {2, 16, 16, 0x1E, {}, {RegH, RegL}, true, false},
    {2, 8,  8,  0x1F, {RegA}, {}, true, false},
    {2, 8,  8,  0x20, {RegB}, {}, true, false},
    {2, 8,  8,  0x21, {RegC}, {}, true, false},
    {2, 8,  8,  0x22, {RegD}, {}, true, false},
    {2, 8,  8,  0x23, {RegE}, {}, true, false},
    {2, 8,  8,  0x24, {RegH}, {}, true, false},
    {2, 8,  8,  0x25, {RegL}, {}, true, false},
    {2, 16, 16, 0x26, {}, {RegH, RegL}, true, false},
    {2, 8,  8,  0x27, {RegA}, {}, true, false},
    {2, 8,  8,  0x28, {RegB}, {}, true, false},
    {2, 8,  8,  0x29, {RegC}, {}, true, false},
    {2, 8,  8,  0x2A, {RegD}, {}, true, false},
    {2, 8,  8,  0x2B, {RegE}, {}, true, false},
    {2, 8,  8,  0x2C, {RegH}, {}, true, false},
    {2, 8,  8,  0x2D, {RegL}, {}, true, false},
    {2, 16, 16, 0x2E, {}, {RegH, RegL}, true, false},
    {2, 8,  8,  0x2F, {RegA}, {}, true, false},
    {2, 8,  8,  0x30, {RegB}, {}, true, false},
    {2, 8,  8,  0x31, {RegC}, {}, true, false},
    {2, 8,  8,  0x32, {RegD}, {}, true, false},
    {2, 8,  8,  0x33, {RegE}, {}, true, false},
    {2, 8,  8,  0x34, {RegH}, {}, true, false},
    {2, 8,  8,  0x35, {RegL}, {}, true, false},
    {2, 16, 16, 0x36, {}, {RegH, RegL}, true, false},
    {2, 8,  8,  0x37, {RegA}, {}, true, false},
    {2, 8,  8,  0x38, {RegB}, {}, true, false},
    {2, 8,  8,  0x39, {RegC}, {}, true, false},
    {2, 8,  8,  0x3A, {RegD}, {}, true, false},
    {2, 8,  8,  0x3B, {RegE}, {}, true, false},
    {2, 8,  8,  0x3C, {RegH}, {}, true, false},
    {2, 8,  8,  0x3D, {RegL}, {}, true, false},
    {2, 16, 16, 0x3E, {}, {RegH, RegL}, true, false},
    {2, 8,  8,  0x3F, {RegA}, {}, true, false},
    {2, 8,  8,  0x40, {}, {RegB}, true, false},
    {2, 8,  8,  0x41, {}, {RegC}, true, false},
    {2, 8,  8,  0x42, {}, {RegD}, true, false},
    {2, 8,  8,  0x43, {}, {RegE}, true, false},
    {2, 8,  8,  0x44, {}, {RegH}, true, false},
    {2, 8,  8,  0x45, {}, {RegL}, true, false},
    {2, 12, 12, 0x46, {}, {RegH, RegL}, true, false},
    {2, 8,  8,  0x47, {}, {RegA}, true, false},
    {2, 8,  8,  0x48, {}, {RegB}, true, false},
    {2, 8,  8,  0x49, {}, {RegC}, true, false},
    {2, 8,  8,  0x4A, {}, {RegD}, true, false},
    {2, 8,  8,  0x4B, {}, {RegE}, true, false},
    {2, 8,  8,  0x4C, {}, {RegH}, true, false},
    {2, 8,  8,  0x4D, {}, {RegL}, true, false},
    {2, 12, 12, 0x4E, {}, {RegH, RegL}, true, false},
    {2, 8,  8,  0x4F, {}, {RegA}, true, false},
    {2, 8,  8,  0x50, {}, {RegB}, true, false},
    {2, 8,  8,  0x51, {}, {RegC}, true, false},
    {2, 8,  8,  0x52, {}, {RegD}, true, false},
    {2, 8,  8,  0x53, {}, {RegE}, true, false},
    {2, 8,  8,  0x54, {}, {RegH}, true, false},
    {2, 8,  8,  0x55, {}, {RegL}, true, false},
    {2, 12, 12, 0x56, {}, {RegH, RegL}, true, false},
    {2, 8,  8,  0x57, {}, {RegA}, true, false},
    {2, 8,  8,  0x58, {}, {RegB}, true, false},
    {2, 8,  8,  0x59, {}, {RegC}, true, false},
    {2, 8,  8,  0x5A, {}, {RegD}, true, false},
    {2, 8,  8,  0x5B, {}, {RegE}, true, false},
    {2, 8,  8,  0x5C, {}, {RegH}, true, false},
    {2, 8,  8,  0x5D, {}, {RegL}, true, false},
    {2, 12, 12, 0x5E, {}, {RegH, RegL}, true, false},
    {2, 8,  8,  0x5F, {}, {RegA}, true, false},
    {2, 8,  8,  0x60, {}, {RegB}, true, false},
    {2, 8,  8,  0x61, {}, {RegC}, true, false},
    {2, 8,  8,  0x62, {}, {RegD}, true, false},
    {2, 8,  8,  0x63, {}, {RegE}, true, false},
    {2, 8,  8,  0x64, {}, {RegH}, true, false},
    {2, 8,  8,  0x65, {}, {RegL}, true, false},
    {2, 12, 12, 0x66, {}, {RegH, RegL}, true, false},
    {2, 8,  8,  0x67, {}, {RegA}, true, false},
    {2, 8,  8,  0x68, {}, {RegB}, true, false},
    {2, 8,  8,  0x69, {}, {RegC}, true, false},
    {2, 8,  8,  0x6A, {}, {RegD}, true, false},
    {2, 8,  8,  0x6B, {}, {RegE}, true, false},
    {2, 8,  8,  0x6C, {}, {RegH}, true, false},
    {2, 8,  8,  0x6D, {}, {RegL}, true, false},
    {2, 12, 12, 0x6E, {}, {RegH, RegL}, true, false},
    {2, 8,  8,  0x6F, {}, {RegA}, true, false},
    {2, 8,  8,  0x70, {}, {RegB}, true, false},
    {2, 8,  8,  0x71, {}, {RegC}, true, false},
    {2, 8,  8,  0x72, {}, {RegD}, true, false},
    {2, 8,  8,  0x73, {}, {RegE}, true, false},
    {2, 8,  8,  0x74, {}, {RegH}, true, false},
    {2, 8,  8,  0x75, {}, {RegL}, true, false},
    {2, 12, 12, 0x76, {}, {RegH, RegL}, true, false},
    {2, 8,  8,  0x77, {}, {RegA}, true, false},
    {2, 8,  8,  0x78, {}, {RegB}, true, false},
    {2, 8,  8,  0x79, {}, {RegC}, true, false},
    {2, 8,  8,  0x7A, {}, {RegD}, true, false},
    {2, 8,  8,  0x7B, {}, {RegE}, true, false},
    {2, 8,  8,  0x7C, {}, {RegH}, true, false},
    {2, 8,  8,  0x7D, {}, {RegL}, true, false},
    {2, 12, 12, 0x7E, {}, {RegH, RegL}, true, false},
    {2, 8,  8,  0x7F, {}, {RegA}, true, false},
    {2, 8,  8,  0x80, {RegB}, {}, false, false},
    {2, 8,  8,  0x81, {RegC}, {}, false, false},
    {2, 8,  8,  0x82, {RegD}, {}, false, false},
    {2, 8,  8,  0x83, {RegE}, {}, false, false},
    {2, 8,  8,  0x84, {RegH}, {}, false, false},
    {2, 8,  8,  0x85, {RegL}, {}, false, false},
    {2, 16, 16, 0x86, {}, {RegH, RegL}, false, false},
    {2, 8,  8,  0x87, {RegA}, {}, false, false},
    {2, 8,  8,  0x88, {RegB}, {}, false, false},
    {2, 8,  8,  0x89, {RegC}, {}, false, false},
    {2, 8,  8,  0x8A, {RegD}, {}, false, false},
    {2, 8,  8,  0x8B, {RegE}, {}, false, false},
    {2, 8,  8,  0x8C, {RegH}, {}, false, false},
    {2, 8,  8,  0x8D, {RegL}, {}, false, false},
    {2, 16, 16, 0x8E, {}, {RegH, RegL}, false, false},
    {2, 8,  8,  0x8F, {RegA}, {}, false, false},
    {2, 8,  8,  0x90, {RegB}, {}, false, false},
    {2, 8,  8,  0x91, {RegC}, {}, false, false},
    {2, 8,  8,  0x92, {RegD}, {}, false, false},
    {2, 8,  8,  0x93, {RegE}, {}, false, false},
    {2, 8,  8,  0x94, {RegH}, {}, false, false},
    {2, 8,  8,  0x95, {RegL}, {}, false, false},
    {2, 16, 16, 0x96, {}, {RegH, RegL}, false, false},
    {2, 8,  8,  0x97, {RegA}, {}, false, false},
    {2, 8,  8,  0x98, {RegB}, {}, false, false},
    {2, 8,  8,  0x99, {RegC}, {}, false, false},
    {2, 8,  8,  0x9A, {RegD}, {}, false, false},
    {2, 8,  8,  0x9B, {RegE}, {}, false, false},
    {2, 8,  8,  0x9C, {RegH}, {}, false, false},
    {2, 8,  8,  0x9D, {RegL}, {}, false, false},
    {2, 16, 16, 0x9E, {}, {RegH, RegL}, false, false},
    {2, 8,  8,  0x9F, {RegA}, {}, false, false},
    {2, 8,  8,  0xA0, {RegB}, {}, false, false},
    {2, 8,  8,  0xA1, {RegC}, {}, false, false},
    {2, 8,  8,  0xA2, {RegD}, {}, false, false},
    {2, 8,  8,  0xA3, {RegE}, {}, false, false},
    {2, 8,  8,  0xA4, {RegH}, {}, false, false},
    {2, 8,  8,  0xA5, {RegL}, {}, false, false},
    {2, 16, 16, 0xA6, {}, {RegH, RegL}, false, false},
    {2, 8,  8,  0xA7, {RegA}, {}, false, false},
    {2, 8,  8,  0xA8, {RegB}, {}, false, false},
    {2, 8,  8,  0xA9, {RegC}, {}, false, false},
    {2, 8,  8,  0xAA, {RegD}, {}, false, false},
    {2, 8,  8,  0xAB, {RegE}, {}, false, false},
    {2, 8,  8,  0xAC, {RegH}, {}, false, false},
    {2, 8,  8,  0xAD, {RegL}, {}, false, false},
    {2, 16, 16, 0xAE, {}, {RegH, RegL}, false, false},
    {2, 8,  8,  0xAF, {RegA}, {}, false, false},
    {2, 8,  8,  0xB0, {RegB}, {}, false, false},
    {2, 8,  8,  0xB1, {RegC}, {}, false, false},
    {2, 8,  8,  0xB2, {RegD}, {}, false, false},
    {2, 8,  8,  0xB3, {RegE}, {}, false, false},
    {2, 8,  8,  0xB4, {RegH}, {}, false, false},
    {2, 8,  8,  0xB5, {RegL}, {}, false, false},
    {2, 16, 16, 0xB6, {}, {RegH, RegL}, false, false},
    {2, 8,  8,  0xB7, {RegA}, {}, false, false},
    {2, 8,  8,  0xB8, {RegB}, {}, false, false},
    {2, 8,  8,  0xB9, {RegC}, {}, false, false},
    {2, 8,  8,  0xBA, {RegD}, {}, false, false},
    {2, 8,  8,  0xBB, {RegE}, {}, false, false},
    {2, 8,  8,  0xBC, {RegH}, {}, false, false},
    {2, 8,  8,  0xBD, {RegL}, {}, false, false},
    {2, 16, 16, 0xBE, {}, {RegH, RegL}, false, false},
    {2, 8,  8,  0xBF, {RegA}, {}, false, false},
    {2, 8,  8,  0xC0, {RegB}, {}, false, false},
    {2, 8,  8,  0xC1, {RegC}, {}, false, false},
    {2, 8,  8,  0xC2, {RegD}, {}, false, false},
    {2, 8,  8,  0xC3, {RegE}, {}, false, false},
    {2, 8,  8,  0xC4, {RegH}, {}, false, false},
    {2, 8,  8,  0xC5, {RegL}, {}, false, false},
    {2, 16, 16, 0xC6, {}, {RegH, RegL}, false, false},
    {2, 8,  8,  0xC7, {RegA}, {}, false, false},
    {2, 8,  8,  0xC8, {RegB}, {}, false, false},
    {2, 8,  8,  0xC9, {RegC}, {}, false, false},
    {2, 8,  8,  0xCA, {RegD}, {}, false, false},
    {2, 8,  8,  0xCB, {RegE}, {}, false, false},
    {2, 8,  8,  0xCC, {RegH}, {}, false, false},
    {2, 8,  8,  0xCD, {RegL}, {}, false, false},
    {2, 16, 16, 0xCE, {}, {RegH, RegL}, false, false},
    {2, 8,  8,  0xCF, {RegA}, {}, false, false},
    {2, 8,  8,  0xD0, {RegB}, {}, false, false},
    {2, 8,  8,  0xD1, {RegC}, {}, false, false},
    {2, 8,  8,  0xD2, {RegD}, {}, false, false},
    {2, 8,  8,  0xD3, {RegE}, {}, false, false},
    {2, 8,  8,  0xD4, {RegH}, {}, false, false},
    {2, 8,  8,  0xD5, {RegL}, {}, false, false},
    {2, 16, 16, 0xD6, {}, {RegH, RegL}, false, false},
    {2, 8,  8,  0xD7, {RegA}, {}, false, false},
    {2, 8,  8,  0xD8, {RegB}, {}, false, false},
    {2, 8,  8,  0xD9, {RegC}, {}, false, false},
    {2, 8,  8,  0xDA, {RegD}, {}, false, false},
    {2, 8,  8,  0xDB, {RegE}, {}, false, false},
    {2, 8,  8,  0xDC, {RegH}, {}, false, false},
    {2, 8,  8,  0xDD, {RegL}, {}, false, false},
    {2, 16, 16, 0xDE, {}, {RegH, RegL}, false, false},
    {2, 8,  8,  0xDF, {RegA}, {}, false, false},
    {2, 8,  8,  0xE0, {RegB}, {}, false, false},
    {2, 8,  8,  0xE1, {RegC}, {}, false, false},
    {2, 8,  8,  0xE2, {RegD}, {}, false, false},
    {2, 8,  8,  0xE3, {RegE}, {}, false, false},
    {2, 8,  8,  0xE4, {RegH}, {}, false, false},
    {2, 8,  8,  0xE5, {RegL}, {}, false, false},
    {2, 16, 16, 0xE6, {}, {RegH, RegL}, false, false},
    {2, 8,  8,  0xE7, {RegA}, {}, false, false},
    {2, 8,  8,  0xE8, {RegB}, {}, false, false},
    {2, 8,  8,  0xE9, {RegC}, {}, false, false},
    {2, 8,  8,  0xEA, {RegD}, {}, false, false},
    {2, 8,  8,  0xEB, {RegE}, {}, false, false},
    {2, 8,  8,  0xEC, {RegH}, {}, false, false},
    {2, 8,  8,  0xED, {RegL}, {}, false, false},
    {2, 16, 16, 0xEE, {}, {RegH, RegL}, false, false},
    {2, 8,  8,  0xEF, {RegA}, {}, false, false},
    {2, 8,  8,  0xF0, {RegB}, {}, false, false},
    {2, 8,  8,  0xF1, {RegC}, {}, false, false},
    {2, 8,  8,  0xF2, {RegD}, {}, false, false},
    {2, 8,  8,  0xF3, {RegE}, {}, false, false},
    {2, 8,  8,  0xF4, {RegH}, {}, false, false},
    {2, 8,  8,  0xF5, {RegL}, {}, false, false},
    {2, 16, 16, 0xF6, {}, {RegH, RegL}, false, false},
    {2, 8,  8,  0xF7, {RegA}, {}, false, false},
    {2, 8,  8,  0xF8, {RegB}, {}, false, false},
    {2, 8,  8,  0xF9, {RegC}, {}, false, false},
    {2, 8,  8,  0xFA, {RegD}, {}, false, false},
    {2, 8,  8,  0xFB, {RegE}, {}, false, false},
    {2, 8,  8,  0xFC, {RegH}, {}, false, false},
    {2, 8,  8,  0xFD, {RegL}, {}, false, false},
    {2, 16, 16, 0xFE, {}, {RegH, RegL}, false, false},
    {2, 8,  8,  0xFF, {RegA}, {}, false, false}
 }};

static FORCE_INLINE uint8_t extract_immediate8(const uint8_t* instruction)
{
    return instruction[1];
}

static FORCE_INLINE uint16_t extract_immediate16(const uint8_t* instruction)
{
    return (static_cast<uint16_t>(instruction[1])) |
           (static_cast<uint16_t>(instruction[2]) << 8);
}

Instruction::Instruction(const uint8_t* bytes, size_t max_len)
{
    if (bytes[0] != 0xCB) m_opcode = &OPCODES[bytes[0]];
    else m_opcode = &CB_OPCODES[bytes[0]];

    if (m_opcode->len_bytes > max_len) m_opcode = nullptr;

    memcpy(m_bytes, bytes, 3);
}

const std::unordered_set<regid_t>& Instruction::write_addresses(DebugCore* debugger)
{
    if (m_bytes[0] != 0xCB)
    {
        switch (m_bytes[0])
        {
            case 0x02: m_write_addresses = {debugger->read(RegBC)}; break;
            case 0x08: m_write_addresses = {extract_immediate16(m_bytes)}; break;
            case 0x12: m_write_addresses = {debugger->read(RegDE)}; break;
            case 0x22: m_write_addresses = {debugger->read(RegHL)}; break;
            case 0x32: m_write_addresses = {debugger->read(RegHL)}; break;
            case 0x34: m_write_addresses = {debugger->read(RegHL)}; break;
            case 0x35: m_write_addresses = {debugger->read(RegHL)}; break;
            case 0x36: m_write_addresses = {debugger->read(RegHL)}; break;
            case 0x70: m_write_addresses = {debugger->read(RegHL)}; break;
            case 0x71: m_write_addresses = {debugger->read(RegHL)}; break;
            case 0x72: m_write_addresses = {debugger->read(RegHL)}; break;
            case 0x73: m_write_addresses = {debugger->read(RegHL)}; break;
            case 0x74: m_write_addresses = {debugger->read(RegHL)}; break;
            case 0x75: m_write_addresses = {debugger->read(RegHL)}; break;
            case 0x77: m_write_addresses = {debugger->read(RegHL)}; break;
            case 0xC5: m_write_addresses = {debugger->read(RegSP), debugger->read(RegSP) - 1}; break;
            case 0xD5: m_write_addresses = {debugger->read(RegSP), debugger->read(RegSP) - 1}; break;
            case 0xE0: m_write_addresses = {0xFF00 + extract_immediate8(m_bytes)}; break;
            case 0xE2: m_write_addresses = {0xFF00 + debugger->read(RegC)}; break;
            case 0xE5: m_write_addresses = {debugger->read(RegSP), debugger->read(RegSP) - 1}; break;
            case 0xEA: m_write_addresses = {extract_immediate16(m_bytes)}; break;
            case 0xF5: m_write_addresses = {debugger->read(RegSP), debugger->read(RegSP) - 1}; break;
            default: m_write_addresses = {}; break;
        }
    }
    else
    {
        switch (m_bytes[1])
        {
            case 0x06:
            case 0x0E:
            case 0x16:
            case 0x1E:
            case 0x26:
            case 0x2E:
            case 0x36:
            case 0x3E:
            case 0x86:
            case 0x8E:
            case 0x96:
            case 0x9E:
            case 0xA6:
            case 0xAE:
            case 0xB6:
            case 0xBE:
            case 0xC6:
            case 0xCE:
            case 0xD6:
            case 0xDE:
            case 0xE6:
            case 0xEE:
            case 0xF6:
            case 0xFE:
                m_write_addresses = {debugger->read(RegHL)}; break;
            default: m_write_addresses = {}; break;
        }
    }

    return m_write_addresses;
}

const std::unordered_set<regid_t>& Instruction::read_addresses(DebugCore* debugger)
{
    if (m_bytes[0] != 0xCB)
    {
        switch (m_bytes[0])
        {
            case 0x0A: m_read_addresses = {debugger->read(RegBC)}; break;
            case 0x1A: m_read_addresses = {debugger->read(RegDE)}; break;
            case 0x2A: m_read_addresses = {debugger->read(RegHL)}; break;
            case 0x3A: m_read_addresses = {debugger->read(RegHL)}; break;
            case 0x46: m_read_addresses = {debugger->read(RegHL)}; break;
            case 0x4E: m_read_addresses = {debugger->read(RegHL)}; break;
            case 0x56: m_read_addresses = {debugger->read(RegHL)}; break;
            case 0x5E: m_read_addresses = {debugger->read(RegHL)}; break;
            case 0x66: m_read_addresses = {debugger->read(RegHL)}; break;
            case 0x6E: m_read_addresses = {debugger->read(RegHL)}; break;
            case 0x7E: m_read_addresses = {debugger->read(RegHL)}; break;
            case 0x86: m_read_addresses = {debugger->read(RegHL)}; break;
            case 0x8E: m_read_addresses = {debugger->read(RegHL)}; break;
            case 0x96: m_read_addresses = {debugger->read(RegHL)}; break;
            case 0x9E: m_read_addresses = {debugger->read(RegHL)}; break;
            case 0xA6: m_read_addresses = {debugger->read(RegHL)}; break;
            case 0xAE: m_read_addresses = {debugger->read(RegHL)}; break;
            case 0xB6: m_read_addresses = {debugger->read(RegHL)}; break;
            case 0xBE: m_read_addresses = {debugger->read(RegHL)}; break;
            case 0xC1: m_read_addresses = {debugger->read(RegSP) - 1, debugger->read(RegSP) - 2}; break;
            case 0xD1: m_read_addresses = {debugger->read(RegSP) - 1, debugger->read(RegSP) - 2}; break;
            case 0xE1: m_read_addresses = {debugger->read(RegSP) - 1, debugger->read(RegSP) - 2}; break;
            case 0xF0: m_read_addresses = {0xFF00 + extract_immediate8(m_bytes)}; break;
            case 0xF1: m_read_addresses = {debugger->read(RegSP) - 1, debugger->read(RegSP) - 2}; break;
            case 0xF2: m_read_addresses = {0xFF00 + debugger->read(RegC)}; break;
            case 0xFA: m_read_addresses = {extract_immediate16(m_bytes)}; break;
        }
    }
    else
    {
        switch (m_bytes[1])
        {
            case 0x46:
            case 0x4E:
            case 0x56:
            case 0x5E:
            case 0x66:
            case 0x6E:
            case 0x76:
            case 0x7D:
                m_read_addresses = {debugger->read(RegHL)}; break;
            default: m_read_addresses = {}; break;
        }
    }

    return m_read_addresses;
}

const std::string& Instruction::disassembly()
{
    if (!is_valid()) m_disasm = "-";

    if (!is_valid() || m_disasm != "") return m_disasm;

    char imm8_[4];
    char imm16_[8];

    sprintf(imm8_, "%X", extract_immediate8(m_bytes));
    sprintf(imm16_, "%X", extract_immediate16(m_bytes));
    std::string imm8(imm8_);
    std::string imm16(imm16_);

    // Irregular opcodes:
    switch (*m_bytes)
    {
        case 0x00: m_disasm = "NOP"; return m_disasm;
        case 0x01: m_disasm = "LD BC, $" + imm16; return m_disasm;
        case 0x02: m_disasm = "LD (BC), A"; return m_disasm;
        case 0x03: m_disasm = "INC BC"; return m_disasm;
        case 0x04: m_disasm = "INC B"; return m_disasm;
        case 0x05: m_disasm = "DEC B"; return m_disasm;
        case 0x06: m_disasm = "LD B, $" + imm8; return m_disasm;
        case 0x07: m_disasm = "RLCA"; return m_disasm;
        case 0x08: m_disasm = "LD ($" + imm16 + "), SP"; return m_disasm;
        case 0x09: m_disasm = "ADD HL, BC"; return m_disasm;
        case 0x0A: m_disasm = "LD A, (BC)"; return m_disasm;
        case 0x0B: m_disasm = "DEC BC"; return m_disasm;
        case 0x0C: m_disasm = "INC C"; return m_disasm;
        case 0x0D: m_disasm = "DEC C"; return m_disasm;
        case 0x0E: m_disasm = "LD C, $" + imm8; return m_disasm;
        case 0x0F: m_disasm = "RRCA"; return m_disasm;
        case 0x10: m_disasm = "STOP"; return m_disasm;
        case 0x11: m_disasm = "LD DE, $" + imm16; return m_disasm;
        case 0x12: m_disasm = "LD (DE), A"; return m_disasm;
        case 0x13: m_disasm = "INC DE"; return m_disasm;
        case 0x14: m_disasm = "INC D"; return m_disasm;
        case 0x15: m_disasm = "DEC D"; return m_disasm;
        case 0x16: m_disasm = "LD D, $" + imm8; return m_disasm;
        case 0x17: m_disasm = "RLA"; return m_disasm;
        case 0x18: m_disasm = "JR $" + imm8; return m_disasm;
        case 0x19: m_disasm = "ADD HL, DE"; return m_disasm;
        case 0x1A: m_disasm = "LD A, (DE)"; return m_disasm;
        case 0x1B: m_disasm = "DEC DE"; return m_disasm;
        case 0x1C: m_disasm = "INC E"; return m_disasm;
        case 0x1D: m_disasm = "DEC E"; return m_disasm;
        case 0x1E: m_disasm = "LD E, $" + imm8; return m_disasm;
        case 0x1F: m_disasm = "RRA"; return m_disasm;
        case 0x20: m_disasm = "JR NZ, $" + imm8; return m_disasm;
        case 0x21: m_disasm = "LD HL, $" + imm16; return m_disasm;
        case 0x22: m_disasm = "LD (HLI), A"; return m_disasm;
        case 0x23: m_disasm = "INC HL"; return m_disasm;
        case 0x24: m_disasm = "INC H"; return m_disasm;
        case 0x25: m_disasm = "DEC H"; return m_disasm;
        case 0x26: m_disasm = "LD H, $" + imm8; return m_disasm;
        case 0x27: m_disasm = "DAA"; return m_disasm;
        case 0x28: m_disasm = "JR Z, $" + imm8; return m_disasm;
        case 0x29: m_disasm = "ADD HL, HL"; return m_disasm;
        case 0x2A: m_disasm = "LD A, (HLI)"; return m_disasm;
        case 0x2B: m_disasm = "DEC HL"; return m_disasm;
        case 0x2C: m_disasm = "INC L"; return m_disasm;
        case 0x2D: m_disasm = "DEC L"; return m_disasm;
        case 0x2E: m_disasm = "LD L, $" + imm8; return m_disasm;
        case 0x2F: m_disasm = "CPL"; return m_disasm;
        case 0x30: m_disasm = "JR NC, $" + imm8; return m_disasm;
        case 0x31: m_disasm = "LD SP, $" + imm16; return m_disasm;
        case 0x32: m_disasm = "LD (HLD), A"; return m_disasm;
        case 0x33: m_disasm = "INC SP"; return m_disasm;
        case 0x34: m_disasm = "INC (HL)"; return m_disasm;
        case 0x35: m_disasm = "DEC (HL)"; return m_disasm;
        case 0x36: m_disasm = "LD (HL), " + imm8; return m_disasm;
        case 0x37: m_disasm = "SCF"; return m_disasm;
        case 0x38: m_disasm = "JR C, $" + imm8; return m_disasm;
        case 0x39: m_disasm = "ADD HL, SP"; return m_disasm;
        case 0x3A: m_disasm = "LD A, (HLD)"; return m_disasm;
        case 0x3B: m_disasm = "DEC SP"; return m_disasm;
        case 0x3C: m_disasm = "INC A"; return m_disasm;
        case 0x3D: m_disasm = "DEC A"; return m_disasm;
        case 0x3E: m_disasm = "LD A, $" + imm8; return m_disasm;
        case 0x3F: m_disasm = "CCF"; return m_disasm;
        case 0x77: m_disasm = "LD (HL), A"; return m_disasm;
        case 0x76: m_disasm = "HALT"; return m_disasm;
        case 0xC0: m_disasm = "RET NZ"; return m_disasm;
        case 0xC1: m_disasm = "POP BC"; return m_disasm;
        case 0xC2: m_disasm = "JP NZ, $" + imm16; return m_disasm;
        case 0xC3: m_disasm = "JP $" + imm16; return m_disasm;
        case 0xC4: m_disasm = "CALL NZ, $" + imm16; return m_disasm;
        case 0xC5: m_disasm = "PUSH BC"; return m_disasm;
        case 0xC6: m_disasm = "ADD A, $" + imm8; return m_disasm;
        case 0xC7: m_disasm = "RST $00"; return m_disasm;
        case 0xC8: m_disasm = "RET Z"; return m_disasm;
        case 0xC9: m_disasm = "RET"; return m_disasm;
        case 0xCA: m_disasm = "JP Z, $" + imm16; return m_disasm;
        case 0xCC: m_disasm = "CALL Z, $" + imm16; return m_disasm;
        case 0xCD: m_disasm = "CALL, $" + imm16; return m_disasm;
        case 0xCE: m_disasm = "ADC A, $" + imm8; return m_disasm;
        case 0xCF: m_disasm = "RST $08"; return m_disasm;
        case 0xD0: m_disasm = "RET NC"; return m_disasm;
        case 0xD1: m_disasm = "POP DE"; return m_disasm;
        case 0xD2: m_disasm = "JP NC, $" + imm16; return m_disasm;
        case 0xD4: m_disasm = "CALL NC, $" + imm16; return m_disasm;
        case 0xD5: m_disasm = "PUSH DE"; return m_disasm;
        case 0xD6: m_disasm = "SUB $" + imm8; return m_disasm;
        case 0xD7: m_disasm = "RST $10"; return m_disasm;
        case 0xD8: m_disasm = "RET C"; return m_disasm;
        case 0xD9: m_disasm = "RETI"; return m_disasm;
        case 0xDA: m_disasm = "JP C, $" + imm8; return m_disasm;
        case 0xDC: m_disasm = "CALL C, $" + imm16; return m_disasm;
        case 0xDE: m_disasm = "BX A, $" + imm8; return m_disasm;
        case 0xDF: m_disasm = "RST $18"; return m_disasm;
        case 0xE0: m_disasm = "LD ($" + imm8 + "), A"; return m_disasm;
        case 0xE1: m_disasm = "POP HL"; return m_disasm;
        case 0xE2: m_disasm = "LD (C), A"; return m_disasm;
        case 0xE5: m_disasm = "PUSH HL"; return m_disasm;
        case 0xE6: m_disasm = "AND $" + imm8; return m_disasm;
        case 0xE7: m_disasm = "RST $20"; return m_disasm;
        case 0xE8: m_disasm = "ADD SP, $" + imm8; return m_disasm;
        case 0xE9: m_disasm = "JP (HL)"; return m_disasm;
        case 0xEA: m_disasm = "LD ($" + imm16 + "), A"; return m_disasm;
        case 0xEE: m_disasm = "XOR $" + imm8; return m_disasm;
        case 0xEF: m_disasm = "RST $28"; return m_disasm;
        case 0xF0: m_disasm = "LD A, ($" + imm8 + ")"; return m_disasm;
        case 0xF1: m_disasm = "POP AF"; return m_disasm;
        case 0xF2: m_disasm = "LD A, (C)"; return m_disasm;
        case 0xF3: m_disasm = "DI"; return m_disasm;
        case 0xF4: m_disasm = "PUSH AF"; return m_disasm;
        case 0xF5: m_disasm = "OR $" + imm8; return m_disasm;
        case 0xF6: m_disasm = "OR $" + imm8; return m_disasm;
        case 0xF7: m_disasm = "RST $30"; return m_disasm;
        case 0xF8: m_disasm = "LD HL, SP"; return m_disasm;
        case 0xF9: m_disasm = "LD SP, HL"; return m_disasm;
        case 0xFA: m_disasm = "LD A, ($" + imm16 + ")"; return m_disasm;
        case 0xFB: m_disasm = "EI"; return m_disasm;
        case 0xFE: m_disasm = "CP $" + imm8; return m_disasm;
        case 0xFF: m_disasm = "RST $38"; return m_disasm;
        default: break;
    }

    // Regular opcodes:
    if (*m_bytes >= 0x40 && *m_bytes <= 0xBF)
    {
        if (*m_bytes >= 0x40 && *m_bytes <= 0x47)
        {
            m_disasm += "LD B, ";
        }
        else if (*m_bytes >= 0x48 && *m_bytes <= 0x4F)
        {
            m_disasm += "LD C, ";
        }
        else if (*m_bytes >= 0x50 && *m_bytes <= 0x57)
        {
            m_disasm += "LD D, ";
        }
        else if (*m_bytes >= 0x58 && *m_bytes <= 0x5F)
        {
            m_disasm += "LD E, ";
        }
        else if (*m_bytes >= 0x60 && *m_bytes <= 0x67)
        {
            m_disasm += "LD H, ";
        }
        else if (*m_bytes >= 0x68 && *m_bytes <= 0x6F)
        {
            m_disasm += "";
        }
        else if (*m_bytes >= 0x70 && *m_bytes <= 0x76)
        {
            m_disasm += "LD (HL), ";
        }
        else if (*m_bytes >= 0x78 && *m_bytes <= 0x7F)
        {
            m_disasm += "LD A, ";
        }
        else if (*m_bytes >= 0x80 && *m_bytes <= 0x87)
        {
            m_disasm += "ADD A, ";
        }
        else if (*m_bytes >= 0x88 && *m_bytes <= 0x8F)
        {
            m_disasm += "ADC A, ";
        }
        else if (*m_bytes >= 0x90 && *m_bytes <= 0x97)
        {
            m_disasm += "SUB ";
        }
        else if (*m_bytes >= 0x98 && *m_bytes <= 0x9F)
        {
            m_disasm += "SBC, ";
        }
        else if (*m_bytes >= 0xA0 && *m_bytes <= 0xA7)
        {
            m_disasm += "AND ";
        }
        else if (*m_bytes >= 0xA8 && *m_bytes <= 0xAF)
        {
            m_disasm += "XOR ";
        }
        else if (*m_bytes >= 0xB0 && *m_bytes <= 0xB7)
        {
            m_disasm += "OR ";
        }
        else if (*m_bytes >= 0xB8 && *m_bytes <= 0xBF)
        {
            m_disasm += "CP ";
        }
    }
    else if (*m_bytes == 0xCB)
    {
        // Irregular instructions:
        switch (m_bytes[1])
        {
            case 0x37: m_disasm = "SWAP A"; return m_disasm;
        }

        // Regular instructions:
        if (m_bytes[1] <= 0x07)
        {
            m_disasm += "RLC ";
        }
        else if (m_bytes[1] >= 0x08 && m_bytes[1] <= 0x0F)
        {
            m_disasm += "RRC ";
        }
        else if (m_bytes[1] >= 0x10 && m_bytes[1] <= 0x17)
        {
            m_disasm += "RL";
        }
        else if (m_bytes[1] >= 0x18 && m_bytes[1] <= 0x1F)
        {
            m_disasm += "RR ";
        }
        else if (m_bytes[1] >= 0x20 && m_bytes[1] <= 0x27)
        {
            m_disasm += "SLA ";
        }
        else if (m_bytes[1] >= 0x28 && m_bytes[1] <= 0x2F)
        {
            m_disasm += "SRA ";
        }
        else if (m_bytes[1] >= 0x38 && m_bytes[1] <= 0x3F)
        {
            m_disasm += "SRL ";
        }
        else if (m_bytes[1] >= 0x40 && m_bytes[1] <= 0x47)
        {
            m_disasm += "BIT 0, ";
        }
        else if (m_bytes[1] >= 0x48 && m_bytes[1] <= 0x4F)
        {
            m_disasm += "BIT 1, ";
        }
        else if (m_bytes[1] >= 0x50 && m_bytes[1] <= 0x57)
        {
            m_disasm += "BIT 2, ";
        }
        else if (m_bytes[1] >= 0x58 && m_bytes[1] <= 0x5F)
        {
            m_disasm += "BIT 3, ";
        }
        else if (m_bytes[1] >= 0x60 && m_bytes[1] <= 0x67)
        {
            m_disasm += "BIT 4, ";
        }
        else if (m_bytes[1] >= 0x68 && m_bytes[1] <= 0x6F)
        {
            m_disasm += "BIT 5, ";
        }
        else if (m_bytes[1] >= 0x70 && m_bytes[1] <= 0x77)
        {
            m_disasm += "BIT 6, ";
        }
        else if (m_bytes[1] >= 0x78 && m_bytes[1] <= 0x7F)
        {
            m_disasm += "BIT 7, ";
        }
        else if (m_bytes[1] >= 0x80 && m_bytes[1] <= 0x87)
        {
            m_disasm += "RES 0, ";
        }
        else if (m_bytes[1] >= 0x88 && m_bytes[1] <= 0x8F)
        {
            m_disasm += "RES 1, ";
        }
        else if (m_bytes[1] >= 0x90 && m_bytes[1] <= 0x97)
        {
            m_disasm += "RES 2, ";
        }
        else if (m_bytes[1] >= 0x98 && m_bytes[1] <= 0x9F)
        {
            m_disasm += "RES 3, ";
        }
        else if (m_bytes[1] >= 0xA0 && m_bytes[1] <= 0xA7)
        {
            m_disasm += "RES 4, ";
        }
        else if (m_bytes[1] >= 0xA8 && m_bytes[1] <= 0xAF)
        {
            m_disasm += "RES 5, ";
        }
        else if (m_bytes[1] >= 0xB0 && m_bytes[1] <= 0xB7)
        {
            m_disasm += "RES 6, ";
        }
        else if (m_bytes[1] >= 0xB8 && m_bytes[1] <= 0xBF)
        {
            m_disasm += "RES 7, ";
        }
        else if (m_bytes[1] >= 0xC0 && m_bytes[1] <= 0xC7)
        {
            m_disasm += "SET 0, ";
        }
        else if (m_bytes[1] >= 0xC8 && m_bytes[1] <= 0xCF)
        {
            m_disasm += "SET 1, ";
        }
        else if (m_bytes[1] >= 0xD0 && m_bytes[1] <= 0xD7)
        {
            m_disasm += "SET 2, ";
        }
        else if (m_bytes[1] >= 0xD8 && m_bytes[1] <= 0xDF)
        {
            m_disasm += "SET 3, ";
        }
        else if (m_bytes[1] >= 0xE0 && m_bytes[1] <= 0xE7)
        {
            m_disasm += "SET 4, ";
        }
        else if (m_bytes[1] >= 0xE8 && m_bytes[1] <= 0xEF)
        {
            m_disasm += "SET 5, ";
        }
        else if (m_bytes[1] >= 0xF0 && m_bytes[1] <= 0xF7)
        {
            m_disasm += "SET 6, ";
        }
        else if (m_bytes[1] >= 0xF8)
        {
            m_disasm += "SET 7, ";
        }
        else
        {
            m_disasm = "-";
            return m_disasm;
        }
    }
    else
    {
        m_disasm = "-";
        return m_disasm;
    }

    uint8_t operand_nibble = 0x00;
    if (m_bytes[0] == 0xCB) operand_nibble = m_bytes[1] & 0x0F;
    else operand_nibble = m_bytes[0] & 0x0F;

    switch (operand_nibble)
    {
        case 0x00: case 0x08: m_disasm += "B"; return m_disasm;
        case 0x01: case 0x09: m_disasm += "C"; return m_disasm;
        case 0x02: case 0x0A: m_disasm += "D"; return m_disasm;
        case 0x03: case 0x0B: m_disasm += "E"; return m_disasm;
        case 0x04: case 0x0C: m_disasm += "H"; return m_disasm;
        case 0x05: case 0x0D: m_disasm += "L"; return m_disasm;
        case 0x06: case 0x0E: m_disasm += "(HL)"; return m_disasm;
        case 0x07: case 0x0F: m_disasm += "A"; return m_disasm;
    }

    return m_disasm;
}

const uint8_t* Instruction::bytes()
{
    return m_bytes;
}

const Opcode* Instruction::opcode()
{
    return m_opcode;
}

bool Instruction::is_valid()
{
    return m_opcode != nullptr;
}
