#include "cpu.hh"

const array<const GBMachine::CPU::IntInfo,5> GBMachine::CPU::INTERRUPT_TABLE =
{{
    {IntID::VBLANK,      1, 0, 0x40},
    {IntID::LCDC_STATUS, 2, 1, 0x48},
    {IntID::TIMER_OVF,   3, 2, 0x50},
    {IntID::SERIAL,      4, 3, 0x58},
    {IntID::KEYPAD,      5, 4, 0x60}
}};

void GBMachine::CPU::request_interrupt(uint8_t line)
{
    mem[HWREG_IF_ADDR] |= 0x01 << line;
}

const GBMachine::CPU::IntInfo* GBMachine::CPU::check_interrupts()
{
    if (!IME_flag) return nullptr;

    // The interrupt information is stored in an order of descending priority.
    for (size_t i = 0; i < INTERRUPT_TABLE.size(); ++i)
    {
        const IntInfo* int_info = &INTERRUPT_TABLE[i];
        if (((mem[HWREG_IF_ADDR] >> int_info->flag_position) & 0x01)
            && ((mem[HWREG_IE_ADDR] >> int_info->flag_position) & 0x01))
            return &INTERRUPT_TABLE[i];
    }

    return nullptr;
}

void GBMachine::CPU::handle_interrupt(const GBMachine::CPU::IntInfo* int_info)
{
    if (!int_info) return;

    is_interrupted = true;
    mem[HWREG_IF_ADDR] &=  ~(0x01 << int_info->flag_position);
    disable_interrupts_now();
    PUSH_r16(PC);
    PC = int_info->jump_address;
    curr_instr = &mem[PC];
    DI_status = IMEStatus::DO_NOTHING;
    EI_status = IMEStatus::DO_NOTHING;
    clock_cycles += 20;
}

void GBMachine::CPU::disable_interrupts_now()
{
    IME_flag_reset();
    DI_status = IMEStatus::DO_NOTHING;
}

void GBMachine::CPU::enable_interrupts_now()
{
    IME_flag_set();
    EI_status = IMEStatus::DO_NOTHING;
}
