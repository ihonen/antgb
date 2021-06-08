#pragma once

#include "emulator/interfaces/imemorybusnode.hh"
#include "emulator/common/types.hh"

#include <cstdint>

class CpuRegisters : public iMemoryBusNode
{
public:
    enum class Reg8
    {
        B, C, D, E, H, L, A, F, IF, IE, IME
    };

    enum class Reg16
    {
        BC, DE, HL, AF, PC, SP
    };

    enum class FFlag
    {
        C = 4,
        H = 5,
        N = 6,
        Z = 7,
    };

    enum class IEFlag
    {
        VBlank  = 0,
        LcdStat = 1,
        Timer   = 2,
        Serial  = 3,
        Joypad  = 4,
    };

    enum class IFFlag
    {
        VBlank  = 0,
        LcdStat = 1,
        Timer   = 2,
        Serial  = 3,
        Joypad  = 4,
    };

    CpuRegisters();
    virtual ~CpuRegisters() override;

    void post_bootram_reset();

    virtual bool owns(addr_t address) override;
    virtual uint8_t* get(addr_t address) override;
    virtual uint8_t read(addr_t address) override;
    virtual void write(addr_t address, uint8_t value) override;

    uint8_t& get_reg(Reg8 reg);
    uint8_t read_reg(Reg8 reg);
    void write_reg(Reg8 reg);

    uint16_t& get_reg(Reg16 reg);
    uint8_t read_reg(Reg16 reg);
    void write_reg(Reg16 reg);

    /*
    bool get_flag(FFlag flag);
    void clear_flag(FFlag flag);
    void set_flag(FFlag flag);
    void update_flag(FFlag flag);

    bool get_flag(IEFlag flag);
    void clear_flag(IEFlag flag);
    void set_flag(IEFlag flag);
    void update_flag(IEFlag flag);

    bool get_flag(FFlag flag);
    void clear_flag(FFlag flag);
    void set_flag(FFlag flag);
    void update_flag(FFlag flag);
    */

    uint8_t& get_B();
    uint8_t read_B();
    void write_B(uint8_t value);

    uint8_t& get_C();
    uint8_t read_C();
    void write_C(uint8_t value);

    uint8_t& get_D();
    uint8_t read_D();
    void write_D(uint8_t value);

    uint8_t& get_E();
    uint8_t read_E();
    void write_E(uint8_t value);

    uint8_t& get_H();
    uint8_t read_H();
    void write_H(uint8_t value);

    uint8_t& get_L();
    uint8_t read_L();
    void write_L(uint8_t value);

    uint8_t& get_A();
    uint8_t read_A();
    void write_A(uint8_t value);

    uint8_t& get_F();
    uint8_t read_F();
    void write_F(uint8_t value);

    uint16_t& get_BC();
    uint16_t read_BC();
    void write_BC(uint16_t value);

    uint16_t& get_DE();
    uint16_t read_DE();
    void write_DE(uint16_t value);

    uint16_t& get_HL();
    uint16_t read_HL();
    void write_HL(uint16_t value);

    uint16_t& get_AF();
    uint16_t read_AF();
    void write_AF(uint16_t value);

    uint16_t& get_PC();
    uint16_t read_PC();
    void write_PC(uint16_t value);

    uint16_t& get_SP();
    uint16_t read_SP();
    void write_SP(uint16_t value);

    uint8_t& get_IE();
    uint8_t read_IE();
    void write_IE(uint8_t value);

    uint8_t& get_IF();
    uint8_t read_IF();
    void write_IF(uint8_t value);

    uint8_t& get_IME();
    uint8_t read_IME();
    void write_IME(uint8_t value);

    bool read_C_flag();
    void clear_C_flag();
    void set_C_flag();
    void update_C_flag(bool condition);

    bool read_H_flag();
    void clear_H_flag();
    void set_H_flag();
    void update_H_flag(bool condition);

    bool read_N_flag();
    void clear_N_flag();
    void set_N_flag();
    void update_N_flag(bool condition);

    bool read_Z_flag();
    void clear_Z_flag();
    void set_Z_flag();
    void update_Z_flag(bool condition);

    bool read_IF_vblank_flag();
    void clear_IF_vblank_flag();
    void set_IF_vblank_flag();

    bool read_IF_lcdstat_flag();
    void clear_IF_lcdstat_flag();
    void set_IF_lcdstat_flag();

    bool read_IF_timer_flag();
    void clear_IF_timer_flag();
    void set_IF_timer_flag();

    bool read_IF_serial_flag();
    void clear_IF_serial_flag();
    void set_IF_serial_flag();

    bool read_IF_joypad_flag();
    void clear_IF_joypad_flag();
    void set_IF_joypad_flag();

    bool read_IE_vblank_flag();
    void clear_IE_vblank_flag();
    void set_IE_vblank_flag();

    bool read_IE_lcdstat_flag();
    void clear_IE_lcdstat_flag();
    void set_IE_lcdstat_flag();

    bool read_IE_timer_flag();
    void clear_IE_timer_flag();
    void set_IE_timer_flag();

    bool read_IE_serial_flag();
    void clear_IE_serial_flag();
    void set_IE_serial_flag();

    bool read_IE_joypad_flag();
    void clear_IE_joypad_flag();
    void set_IE_joypad_flag();

protected:
    union
    {
        uint16_t BC;
        struct { uint8_t C; uint8_t B; } __attribute__((packed));
    };
    union
    {
        uint16_t DE;
        struct { uint8_t E; uint8_t D; } __attribute__((packed));
    };
    union
    {
        uint16_t HL;
        struct { uint8_t L; uint8_t H; } __attribute__((packed));
    };
    union
    {
        uint16_t AF;
        struct { uint8_t F; uint8_t A; } __attribute__((packed));
    };

    uint16_t SP;
    uint16_t PC;

    uint8_t IF;
    uint8_t IE;
    uint8_t IME;

    void assert_invariants();
};
