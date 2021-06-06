#include "cpuregisters.hh"

#include "addresses.hh"
#include "bitmanip.hh"

#include <cassert>

namespace
{

constexpr uint8_t F_MASK  = 0b11110000;
constexpr uint8_t IE_MASK = 0b00011111;
constexpr uint8_t IF_MASK = 0b00011111;

constexpr uint8_t FLAG_C_POS = 4;
constexpr uint8_t FLAG_H_POS = 5;
constexpr uint8_t FLAG_N_POS = 6;
constexpr uint8_t FLAG_Z_POS = 7;

constexpr uint8_t FLAG_VBLANK_POS = 0;
constexpr uint8_t FLAG_LCDSTAT_POS = 0;
constexpr uint8_t FLAG_TIMER_POS = 0;
constexpr uint8_t FLAG_SERIAL_POS = 0;
constexpr uint8_t FLAG_JOYPAD_POS = 0;

uint8_t mask(memaddr_t address)
{
    switch (address)
    {
        case IE_ADDR:
            return IE_MASK;
        case IF_ADDR:
            return IF_MASK;
    }
    return 0xFF;
}


} // namespace

CpuRegisters::CpuRegisters()
    : BC(0x0000)
    , DE(0x0000)
    , HL(0x0000)
    , AF(0x0000)
    , SP(0x0000)
    , PC(0x0000)
    , IF(0x00)
    , IE(0x00)
    , IME(0)
{
}

void CpuRegisters::post_bootram_reset()
{
    AF = 0x01B0;
    BC = 0x0013;
    DE = 0x00D8;
    HL = 0x014D;
    SP = 0xFFFE;
    PC = 0x0100;
    IE = 0x00;
    IF = 0x00;
    IME = 0;
}

CpuRegisters::~CpuRegisters() = default;

uint8_t* CpuRegisters::get(memaddr_t address)
{
    switch (address)
    {
        case IE_ADDR:
            return &IE;
        case IF_ADDR:
            return &IF;
    }
    assert(false);
    return nullptr;
}

uint8_t CpuRegisters::read(memaddr_t address)
{
    if (auto* byte = get(address))
    {
        return *byte & mask(address);
    }
    assert(false);
    return 0xFF;
}

void CpuRegisters::write(memaddr_t address, uint8_t value)
{
    if (auto* byte = get(address))
    {
        *byte = value & mask(address);
    }
    assert(false);
}

uint8_t& CpuRegisters::get_B()
{
    return B;
}

uint8_t CpuRegisters::read_B()
{
    return B;
}

void CpuRegisters::write_B(uint8_t value)
{
    B = value;
}

uint8_t& CpuRegisters::get_C()
{
    return C;
}

uint8_t CpuRegisters::read_C()
{
    return C;
}

void CpuRegisters::write_C(uint8_t value)
{
    C = value;
}

uint8_t& CpuRegisters::get_D()
{
    return D;
}

uint8_t CpuRegisters::read_D()
{
    return D;
}

void CpuRegisters::write_D(uint8_t value)
{
    D = value;
}

uint8_t& CpuRegisters::get_E()
{
    return E;
}

uint8_t CpuRegisters::read_E()
{
    return E;
}

void CpuRegisters::write_E(uint8_t value)
{
    E = value;
}

uint8_t& CpuRegisters::get_H()
{
    return H;
}

uint8_t CpuRegisters::read_H()
{
    return H;
}

void CpuRegisters::write_H(uint8_t value)
{
    H = value;
}

uint8_t& CpuRegisters::get_L()
{
    return L;
}

uint8_t CpuRegisters::read_L()
{
    return L;
}

void CpuRegisters::write_L(uint8_t value)
{
    L = value;
}

uint8_t& CpuRegisters::get_A()
{
    return A;
}

uint8_t CpuRegisters::read_A()
{
    return A;
}

void CpuRegisters::write_A(uint8_t value)
{
    A = value;
}

uint8_t& CpuRegisters::get_F()
{
    return F;
}

uint8_t CpuRegisters::read_F()
{
    return F;
}

void CpuRegisters::write_F(uint8_t value)
{
    F = F_MASK & value;
    assert_invariants();
}

uint16_t& CpuRegisters::get_BC()
{
    return BC;
}

uint16_t CpuRegisters::read_BC()
{
    return BC;
}

void CpuRegisters::write_BC(uint16_t value)
{
    BC = value;
}

uint16_t& CpuRegisters::get_DE()
{
    return DE;
}

uint16_t CpuRegisters::read_DE()
{
    return DE;
}

void CpuRegisters::write_DE(uint16_t value)
{
    DE = value;
}

uint16_t& CpuRegisters::get_HL()
{
    return HL;
}

uint16_t CpuRegisters::read_HL()
{
    return HL;
}

void CpuRegisters::write_HL(uint16_t value)
{
    HL = value;
}

uint16_t& CpuRegisters::get_AF()
{
    return AF;
}

uint16_t CpuRegisters::read_AF()
{
    return AF;
}

void CpuRegisters::write_AF(uint16_t value)
{
    write_A(value >> 8);
    write_F(static_cast<uint8_t>(value));
    assert_invariants();
}

uint16_t& CpuRegisters::get_PC()
{
    return PC;
}

uint16_t CpuRegisters::read_PC()
{
    return PC;
}

void CpuRegisters::write_PC(uint16_t value)
{
    PC = value;
}

uint16_t& CpuRegisters::get_SP()
{
    return SP;
}

uint16_t CpuRegisters::read_SP()
{
    return SP;
}

void CpuRegisters::write_SP(uint16_t value)
{
    SP = value;
}

uint8_t& CpuRegisters::get_IE()
{
    return IE;
}

uint8_t CpuRegisters::read_IE()
{
    return IE;
}

void CpuRegisters::write_IE(uint8_t value)
{
    IE = IE_MASK & value;
    assert_invariants();
}

uint8_t& CpuRegisters::get_IF()
{
    return IF;
}

uint8_t CpuRegisters::read_IF()
{
    return IF;
}

void CpuRegisters::write_IF(uint8_t value)
{
    IF = IF_MASK & value;
    assert_invariants();
}

uint8_t& CpuRegisters::get_IME()
{
    return IME;
}

uint8_t CpuRegisters::read_IME()
{
    return IME;
}

void CpuRegisters::write_IME(uint8_t value)
{
    IME = uint8_t(bool(value));
    assert_invariants();
}

bool CpuRegisters::read_C_flag()
{
    return bool(get_bit(&F, FLAG_C_POS));
}

void CpuRegisters::clear_C_flag()
{
    clear_bit(&F, FLAG_C_POS);
}

void CpuRegisters::set_C_flag()
{
    set_bit(&F, FLAG_C_POS);
}

void CpuRegisters::update_C_flag(bool condition)
{
    if (condition)
    {
        set_bit(&F, FLAG_C_POS);
    }
    else
    {
        clear_bit(&F, FLAG_C_POS);
    }
}

bool CpuRegisters::read_H_flag()
{
    return bool(get_bit(&F, FLAG_H_POS));
}

void CpuRegisters::clear_H_flag()
{
    clear_bit(&F, FLAG_H_POS);
}

void CpuRegisters::set_H_flag()
{
    set_bit(&F, FLAG_H_POS);
}

void CpuRegisters::update_H_flag(bool condition)
{
    if (condition)
    {
        set_bit(&F, FLAG_H_POS);
    }
    else
    {
        clear_bit(&F, FLAG_H_POS);
    }
}

bool CpuRegisters::read_N_flag()
{
    return bool(get_bit(&F, FLAG_N_POS));
}

void CpuRegisters::clear_N_flag()
{
    clear_bit(&F, FLAG_N_POS);
}

void CpuRegisters::set_N_flag()
{
    set_bit(&F, FLAG_N_POS);
}

void CpuRegisters::update_N_flag(bool condition)
{
    if (condition)
    {
        set_bit(&F, FLAG_N_POS);
    }
    else
    {
        clear_bit(&F, FLAG_N_POS);
    }
}

bool CpuRegisters::read_Z_flag()
{
    return bool(get_bit(&F, FLAG_Z_POS));
}

void CpuRegisters::clear_Z_flag()
{
    clear_bit(&F, FLAG_Z_POS);
}

void CpuRegisters::set_Z_flag()
{
    set_bit(&F, FLAG_Z_POS);
}

void CpuRegisters::update_Z_flag(bool condition)
{
    if (condition)
    {
        set_bit(&F, FLAG_Z_POS);
    }
    else
    {
        clear_bit(&F, FLAG_Z_POS);
    }
}

bool CpuRegisters::read_IF_vblank_flag()
{
    return bool(get_bit(&IF, FLAG_VBLANK_POS));
}

void CpuRegisters::clear_IF_vblank_flag()
{
    clear_bit(&IF, FLAG_VBLANK_POS);
}

void CpuRegisters::set_IF_vblank_flag()
{
    set_bit(&IF, FLAG_VBLANK_POS);
}

bool CpuRegisters::read_IF_lcdstat_flag()
{
    return bool(get_bit(&IF, FLAG_LCDSTAT_POS));
}

void CpuRegisters::clear_IF_lcdstat_flag()
{
    clear_bit(&IF, FLAG_LCDSTAT_POS);
}

void CpuRegisters::set_IF_lcdstat_flag()
{
    set_bit(&IF, FLAG_LCDSTAT_POS);
}

bool CpuRegisters::read_IF_timer_flag()
{
    return bool(get_bit(&IF, FLAG_TIMER_POS));
}

void CpuRegisters::clear_IF_timer_flag()
{
    clear_bit(&IF, FLAG_TIMER_POS);
}

void CpuRegisters::set_IF_timer_flag()
{
    set_bit(&IF, FLAG_TIMER_POS);
}

bool CpuRegisters::read_IF_serial_flag()
{
    return bool(get_bit(&IF, FLAG_SERIAL_POS));
}

void CpuRegisters::clear_IF_serial_flag()
{
    clear_bit(&IF, FLAG_SERIAL_POS);
}

void CpuRegisters::set_IF_serial_flag()
{
    clear_bit(&IF, FLAG_SERIAL_POS);
}

bool CpuRegisters::read_IF_joypad_flag()
{
    return bool(get_bit(&IF, FLAG_JOYPAD_POS));
}

void CpuRegisters::clear_IF_joypad_flag()
{
    clear_bit(&IF, FLAG_JOYPAD_POS);
}

void CpuRegisters::set_IF_joypad_flag()
{
    set_bit(&IF, FLAG_JOYPAD_POS);
}

bool CpuRegisters::read_IE_vblank_flag()
{
    return bool(get_bit(&IE, FLAG_VBLANK_POS));
}

void CpuRegisters::clear_IE_vblank_flag()
{
    clear_bit(&IE, FLAG_VBLANK_POS);
}

void CpuRegisters::set_IE_vblank_flag()
{
    set_bit(&IE, FLAG_VBLANK_POS);
}

bool CpuRegisters::read_IE_lcdstat_flag()
{
    return bool(get_bit(&IE, FLAG_LCDSTAT_POS));
}

void CpuRegisters::clear_IE_lcdstat_flag()
{
    clear_bit(&IE, FLAG_LCDSTAT_POS);
}

void CpuRegisters::set_IE_lcdstat_flag()
{
    set_bit(&IE, FLAG_LCDSTAT_POS);
}

bool CpuRegisters::read_IE_timer_flag()
{
    return bool(get_bit(&IE, FLAG_TIMER_POS));
}

void CpuRegisters::clear_IE_timer_flag()
{
    clear_bit(&IE, FLAG_TIMER_POS);
}

void CpuRegisters::set_IE_timer_flag()
{
    set_bit(&IE, FLAG_TIMER_POS);
}

bool CpuRegisters::read_IE_serial_flag()
{
    return bool(get_bit(&IE, FLAG_SERIAL_POS));
}

void CpuRegisters::clear_IE_serial_flag()
{
    clear_bit(&IE, FLAG_SERIAL_POS);
}

void CpuRegisters::set_IE_serial_flag()
{
    clear_bit(&IE, FLAG_SERIAL_POS);
}

bool CpuRegisters::read_IE_joypad_flag()
{
    return bool(get_bit(&IE, FLAG_JOYPAD_POS));
}

void CpuRegisters::clear_IE_joypad_flag()
{
    clear_bit(&IE, FLAG_JOYPAD_POS);
}

void CpuRegisters::set_IE_joypad_flag()
{
    set_bit(&IE, FLAG_JOYPAD_POS);
}

void CpuRegisters::assert_invariants()
{
    assert((F  & ~F_MASK)  == 0x00);
    assert((IE & ~IE_MASK) == 0x00);
    assert((IF & ~IF_MASK) == 0x00);
    assert(IME == 0 || IME == 1);
}
