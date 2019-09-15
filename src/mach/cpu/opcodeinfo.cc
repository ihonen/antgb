#ifndef OPCODES_HH
#define OPCODES_HH

#include "cpu.hh"
#include <cstdint>

class Cpu::OpcodeInfo
{
public:
    typedef struct
    {
        // The length of the instruction in bytes.
        uint8_t len = 1;
        // The duration of the instruction in clock cycles if failed.
        uint8_t dur_f = 0;
        // The duration of the instruction in clock cycles if successful.
        uint8_t dur_s = 0;
        // The function that is called when the instruction
        // is encountered.
        void (Cpu::*handler)() = nullptr;
    } OpcodeInfo_;

    const OpcodeInfo_& operator[](uint8_t index) { return OP_INFO[index]; }
private:
    const OpcodeInfo_ OP_INFO[256] =
    {
        {1, 0, 0, nullptr}, // 0x00
        {1, 0, 0, nullptr}, // 0x01
        {1, 0, 0, nullptr}, // 0x02
        {1, 0, 0, nullptr}, // 0x03
        {1, 0, 0, nullptr}, // 0x04
        {1, 0, 0, nullptr}, // 0x05
        {1, 0, 0, nullptr}, // 0x06
        {1, 0, 0, nullptr}, // 0x07
        {1, 0, 0, nullptr}, // 0x08
        {1, 0, 0, nullptr}, // 0x09
        {1, 0, 0, nullptr}, // 0x0A
        {1, 0, 0, nullptr}, // 0x0B
        {1, 0, 0, nullptr}, // 0x0C
        {1, 0, 0, nullptr}, // 0x0D
        {1, 0, 0, nullptr}, // 0x0E
        {1, 0, 0, nullptr}, // 0x0F

        {1, 0, 0, nullptr}, // 0x10
        {1, 0, 0, nullptr}, // 0x11
        {1, 0, 0, nullptr}, // 0x12
        {1, 0, 0, nullptr}, // 0x13
        {1, 0, 0, nullptr}, // 0x14
        {1, 0, 0, nullptr}, // 0x15
        {1, 0, 0, nullptr}, // 0x16
        {1, 0, 0, nullptr}, // 0x17
        {1, 0, 0, nullptr}, // 0x18
        {1, 0, 0, nullptr}, // 0x19
        {1, 0, 0, nullptr}, // 0x1A
        {1, 0, 0, nullptr}, // 0x1B
        {1, 0, 0, nullptr}, // 0x1C
        {1, 0, 0, nullptr}, // 0x1D
        {1, 0, 0, nullptr}, // 0x1E
        {1, 0, 0, nullptr}, // 0x1F

        {1, 0, 0, nullptr}, // 0x20
        {1, 0, 0, nullptr}, // 0x21
        {1, 0, 0, nullptr}, // 0x22
        {1, 0, 0, nullptr}, // 0x23
        {1, 0, 0, nullptr}, // 0x24
        {1, 0, 0, nullptr}, // 0x25
        {1, 0, 0, nullptr}, // 0x26
        {1, 0, 0, nullptr}, // 0x27
        {1, 0, 0, nullptr}, // 0x28
        {1, 0, 0, nullptr}, // 0x29
        {1, 0, 0, nullptr}, // 0x2A
        {1, 0, 0, nullptr}, // 0x2B
        {1, 0, 0, nullptr}, // 0x2C
        {1, 0, 0, nullptr}, // 0x2D
        {1, 0, 0, nullptr}, // 0x2E
        {1, 0, 0, nullptr}, // 0x2F

        {1, 0, 0, nullptr}, // 0x30
        {1, 0, 0, nullptr}, // 0x31
        {1, 0, 0, nullptr}, // 0x32
        {1, 0, 0, nullptr}, // 0x33
        {1, 0, 0, nullptr}, // 0x34
        {1, 0, 0, nullptr}, // 0x35
        {1, 0, 0, nullptr}, // 0x36
        {1, 0, 0, nullptr}, // 0x37
        {1, 0, 0, nullptr}, // 0x38
        {1, 0, 0, nullptr}, // 0x39
        {1, 0, 0, nullptr}, // 0x3A
        {1, 0, 0, nullptr}, // 0x3B
        {1, 0, 0, nullptr}, // 0x3C
        {1, 0, 0, nullptr}, // 0x3D
        {1, 0, 0, nullptr}, // 0x3E
        {1, 0, 0, nullptr}, // 0x3F

        {1, 0, 0, nullptr}, // 0x40
        {1, 0, 0, nullptr}, // 0x41
        {1, 0, 0, nullptr}, // 0x42
        {1, 0, 0, nullptr}, // 0x43
        {1, 0, 0, nullptr}, // 0x44
        {1, 0, 0, nullptr}, // 0x45
        {1, 0, 0, nullptr}, // 0x46
        {1, 0, 0, nullptr}, // 0x47
        {1, 0, 0, nullptr}, // 0x48
        {1, 0, 0, nullptr}, // 0x49
        {1, 0, 0, nullptr}, // 0x4A
        {1, 0, 0, nullptr}, // 0x4B
        {1, 0, 0, nullptr}, // 0x4C
        {1, 0, 0, nullptr}, // 0x4D
        {1, 0, 0, nullptr}, // 0x4E
        {1, 0, 0, nullptr}, // 0x4F

        {1, 0, 0, nullptr}, // 0x50
        {1, 0, 0, nullptr}, // 0x51
        {1, 0, 0, nullptr}, // 0x52
        {1, 0, 0, nullptr}, // 0x53
        {1, 0, 0, nullptr}, // 0x54
        {1, 0, 0, nullptr}, // 0x55
        {1, 0, 0, nullptr}, // 0x56
        {1, 0, 0, nullptr}, // 0x57
        {1, 0, 0, nullptr}, // 0x58
        {1, 0, 0, nullptr}, // 0x59
        {1, 0, 0, nullptr}, // 0x5A
        {1, 0, 0, nullptr}, // 0x5B
        {1, 0, 0, nullptr}, // 0x5C
        {1, 0, 0, nullptr}, // 0x5D
        {1, 0, 0, nullptr}, // 0x5E
        {1, 0, 0, nullptr}, // 0x5F

        {1, 0, 0, nullptr}, // 0x60
        {1, 0, 0, nullptr}, // 0x61
        {1, 0, 0, nullptr}, // 0x62
        {1, 0, 0, nullptr}, // 0x63
        {1, 0, 0, nullptr}, // 0x64
        {1, 0, 0, nullptr}, // 0x65
        {1, 0, 0, nullptr}, // 0x66
        {1, 0, 0, nullptr}, // 0x67
        {1, 0, 0, nullptr}, // 0x68
        {1, 0, 0, nullptr}, // 0x69
        {1, 0, 0, nullptr}, // 0x6A
        {1, 0, 0, nullptr}, // 0x6B
        {1, 0, 0, nullptr}, // 0x6C
        {1, 0, 0, nullptr}, // 0x6D
        {1, 0, 0, nullptr}, // 0x6E
        {1, 0, 0, nullptr}, // 0x6F

        {1, 0, 0, nullptr}, // 0x70
        {1, 0, 0, nullptr}, // 0x71
        {1, 0, 0, nullptr}, // 0x72
        {1, 0, 0, nullptr}, // 0x73
        {1, 0, 0, nullptr}, // 0x74
        {1, 0, 0, nullptr}, // 0x75
        {1, 0, 0, nullptr}, // 0x76
        {1, 0, 0, nullptr}, // 0x77
        {1, 0, 0, nullptr}, // 0x78
        {1, 0, 0, nullptr}, // 0x79
        {1, 0, 0, nullptr}, // 0x7A
        {1, 0, 0, nullptr}, // 0x7B
        {1, 0, 0, nullptr}, // 0x7C
        {1, 0, 0, nullptr}, // 0x7D
        {1, 0, 0, nullptr}, // 0x7E
        {1, 0, 0, nullptr}, // 0x7F

        {1, 0, 0, nullptr}, // 0x80
        {1, 0, 0, nullptr}, // 0x81
        {1, 0, 0, nullptr}, // 0x82
        {1, 0, 0, nullptr}, // 0x83
        {1, 0, 0, nullptr}, // 0x84
        {1, 0, 0, nullptr}, // 0x85
        {1, 0, 0, nullptr}, // 0x86
        {1, 0, 0, nullptr}, // 0x87
        {1, 0, 0, nullptr}, // 0x88
        {1, 0, 0, nullptr}, // 0x89
        {1, 0, 0, nullptr}, // 0x8A
        {1, 0, 0, nullptr}, // 0x8B
        {1, 0, 0, nullptr}, // 0x8C
        {1, 0, 0, nullptr}, // 0x8D
        {1, 0, 0, nullptr}, // 0x8E
        {1, 0, 0, nullptr}, // 0x8F

        {1, 0, 0, nullptr}, // 0x90
        {1, 0, 0, nullptr}, // 0x91
        {1, 0, 0, nullptr}, // 0x92
        {1, 0, 0, nullptr}, // 0x93
        {1, 0, 0, nullptr}, // 0x94
        {1, 0, 0, nullptr}, // 0x95
        {1, 0, 0, nullptr}, // 0x96
        {1, 0, 0, nullptr}, // 0x97
        {1, 0, 0, nullptr}, // 0x98
        {1, 0, 0, nullptr}, // 0x99
        {1, 0, 0, nullptr}, // 0x9A
        {1, 0, 0, nullptr}, // 0x9B
        {1, 0, 0, nullptr}, // 0x9C
        {1, 0, 0, nullptr}, // 0x9D
        {1, 0, 0, nullptr}, // 0x9E
        {1, 0, 0, nullptr}, // 0x9F

        {1, 0, 0, nullptr}, // 0xA0
        {1, 0, 0, nullptr}, // 0xA1
        {1, 0, 0, nullptr}, // 0xA2
        {1, 0, 0, nullptr}, // 0xA3
        {1, 0, 0, nullptr}, // 0xA4
        {1, 0, 0, nullptr}, // 0xA5
        {1, 0, 0, nullptr}, // 0xA6
        {1, 0, 0, nullptr}, // 0xA7
        {1, 0, 0, nullptr}, // 0xA8
        {1, 0, 0, nullptr}, // 0xA9
        {1, 0, 0, nullptr}, // 0xAA
        {1, 0, 0, nullptr}, // 0xAB
        {1, 0, 0, nullptr}, // 0xAC
        {1, 0, 0, nullptr}, // 0xAD
        {1, 0, 0, nullptr}, // 0xAE
        {1, 0, 0, nullptr}, // 0xAF

        {1, 0, 0, nullptr}, // 0xB0
        {1, 0, 0, nullptr}, // 0xB1
        {1, 0, 0, nullptr}, // 0xB2
        {1, 0, 0, nullptr}, // 0xB3
        {1, 0, 0, nullptr}, // 0xB4
        {1, 0, 0, nullptr}, // 0xB5
        {1, 0, 0, nullptr}, // 0xB6
        {1, 0, 0, nullptr}, // 0xB7
        {1, 0, 0, nullptr}, // 0xB8
        {1, 0, 0, nullptr}, // 0xB9
        {1, 0, 0, nullptr}, // 0xBA
        {1, 0, 0, nullptr}, // 0xBB
        {1, 0, 0, nullptr}, // 0xBC
        {1, 0, 0, nullptr}, // 0xBD
        {1, 0, 0, nullptr}, // 0xBE
        {1, 0, 0, nullptr}, // 0xBF

        {1, 0, 0, nullptr}, // 0xC0
        {1, 0, 0, nullptr}, // 0xC1
        {1, 0, 0, nullptr}, // 0xC2
        {1, 0, 0, nullptr}, // 0xC3
        {1, 0, 0, nullptr}, // 0xC4
        {1, 0, 0, nullptr}, // 0xC5
        {1, 0, 0, nullptr}, // 0xC6
        {1, 0, 0, nullptr}, // 0xC7
        {1, 0, 0, nullptr}, // 0xC8
        {1, 0, 0, nullptr}, // 0xC9
        {1, 0, 0, nullptr}, // 0xCA
        {1, 0, 0, nullptr}, // 0xCB
        {1, 0, 0, nullptr}, // 0xCC
        {1, 0, 0, nullptr}, // 0xCD
        {1, 0, 0, nullptr}, // 0xCE
        {1, 0, 0, nullptr}, // 0xCF

        {1, 0, 0, nullptr}, // 0xD0
        {1, 0, 0, nullptr}, // 0xD1
        {1, 0, 0, nullptr}, // 0xD2
        {1, 0, 0, nullptr}, // 0xD3
        {1, 0, 0, nullptr}, // 0xD4
        {1, 0, 0, nullptr}, // 0xD5
        {1, 0, 0, nullptr}, // 0xD6
        {1, 0, 0, nullptr}, // 0xD7
        {1, 0, 0, nullptr}, // 0xD8
        {1, 0, 0, nullptr}, // 0xD9
        {1, 0, 0, nullptr}, // 0xDA
        {1, 0, 0, nullptr}, // 0xDB
        {1, 0, 0, nullptr}, // 0xDC
        {1, 0, 0, nullptr}, // 0xDD
        {1, 0, 0, nullptr}, // 0xDE
        {1, 0, 0, nullptr}, // 0xDF

        {1, 0, 0, nullptr}, // 0xE0
        {1, 0, 0, nullptr}, // 0xE1
        {1, 0, 0, nullptr}, // 0xE2
        {1, 0, 0, nullptr}, // 0xE3
        {1, 0, 0, nullptr}, // 0xE4
        {1, 0, 0, nullptr}, // 0xE5
        {1, 0, 0, nullptr}, // 0xE6
        {1, 0, 0, nullptr}, // 0xE7
        {1, 0, 0, nullptr}, // 0xE8
        {1, 0, 0, nullptr}, // 0xE9
        {1, 0, 0, nullptr}, // 0xEA
        {1, 0, 0, nullptr}, // 0xEB
        {1, 0, 0, nullptr}, // 0xEC
        {1, 0, 0, nullptr}, // 0xED
        {1, 0, 0, nullptr}, // 0xEE
        {1, 0, 0, nullptr}, // 0xEF

        {1, 0, 0, nullptr}, // 0xF0
        {1, 0, 0, nullptr}, // 0xF1
        {1, 0, 0, nullptr}, // 0xF2
        {1, 0, 0, nullptr}, // 0xF3
        {1, 0, 0, nullptr}, // 0xF4
        {1, 0, 0, nullptr}, // 0xF5
        {1, 0, 0, nullptr}, // 0xF6
        {1, 0, 0, nullptr}, // 0xF7
        {1, 0, 0, nullptr}, // 0xF8
        {1, 0, 0, nullptr}, // 0xF9
        {1, 0, 0, nullptr}, // 0xFA
        {1, 0, 0, nullptr}, // 0xFB
        {1, 0, 0, nullptr}, // 0xFC
        {1, 0, 0, nullptr}, // 0xFD
        {1, 0, 0, nullptr}, // 0xFE
        {1, 0, 0, nullptr}  // 0xFF
    };
};

#endif // OPCODES_HH
