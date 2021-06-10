#include "cpu.hh"

#include "emulator/memory/memorybus.hh"
#include <array>
#include <iomanip>
#include <iostream>

Cpu::Cpu(MemoryBus* mem, CpuRegisters& registers)
    : mem(mem)
    , reg(registers)
{
    trace_log.open("trace.log", std::ofstream::out);
    hard_reset();
}

Cpu::~Cpu()
{
    std::cerr << "vblank interrupts: " << vblank_irqs << std::endl;
    std::cerr << "lcdstat interrupts: " << lcdstat_irqs << std::endl;
    std::cerr << "timer interrupts: " << timer_irqs << std::endl;
    std::cerr << "serial interrupts: " << serial_irqs << std::endl;
    std::cerr << "joypad interrupts: " << joypad_irqs << std::endl;
}

void Cpu::set_PC(uint16_t value)
{
    reg.write_PC(value);
}

void Cpu::restart()
{
    hard_reset();
}

void Cpu::hard_reset()
{
    reg = CpuRegisters();
    reg.post_bootram_reset();

    current_instruction = nullptr;
    branch_taken = false;
    DI_countdown = NO_COUNTDOWN;
    EI_countdown = NO_COUNTDOWN;
    is_halted = false;
    is_stopped = false;
    clock_cycles = 0;
}

void Cpu::execute(const uint8_t* instruction)
{
    if (!instruction) instruction = mem->get(reg.read_PC());
    assert(instruction);
    current_instruction = instruction;
    branch_taken = false;

    if (EI_countdown > 0)
        --EI_countdown;

    if (has_pending_requests())
    {
        auto interrupt = next_request();

        if (interrupt.source != NoInterrupt)
        {
            bool was_halted = is_halted;
            is_halted = false;
            if (interrupt.source == JoypadInterrupt)
            {
                // TODO: Does this require an actual
                // interrupt or just a button press?
                is_stopped = false;
            }

            if (reg.read_IME())
            {
                if (was_halted)
                    clock_cycles += 4;
                reg.write_IME(0);
                clear_interrupt(interrupt.source);
                jump_to_isr(interrupt.vector_address);
                return;
            }
        }
    }

    if (!is_halted && !is_stopped)
    {
        const InstructionInfo* op_info = (*current_instruction == 0xCB) ?
                                    &CB_INSTRUCTION_TABLE[current_instruction[1]] :
                                    &INSTRUCTION_TABLE[*current_instruction];

        reg.write_PC(reg.read_PC() + op_info->len_bytes);

        if (op_info->handler) (this->*(op_info->handler))();
        else invalid_opcode();

        if (branch_taken)
            clock_cycles += op_info->cycles_on_action;
        else
            clock_cycles += op_info->cycles_on_no_action;
    }
    else
    {
        // Halted or stopped, so advance time by one machine cycle.
        clock_cycles += 4;
    }

    if (DI_countdown == 0)
    {
        reg.write_IME(0);
        DI_countdown = NO_COUNTDOWN;
        EI_countdown = NO_COUNTDOWN;
    }
    else if (EI_countdown == 0)
    {
        reg.write_IME(1);
        DI_countdown = NO_COUNTDOWN;
        EI_countdown = NO_COUNTDOWN;
    }
}

void Cpu::invalid_opcode()
{
    throw OpcodeError(0, *current_instruction);
}

void Cpu::jump_to_isr(addr_t vector_address)
{
    if (vector_address == 0x00) return;

    // cerr << "Jumping to interrupt vector @ " << std::hex << vector_address << endl;
    // trace_log << "Jumping to interrupt vector @ " << std::hex << vector_address << endl;

    reg.write_IME(0);
    PUSH_r16(&reg.get_PC());
    reg.write_PC(vector_address);
    current_instruction = mem->get(reg.read_PC());
    DI_countdown = NO_COUNTDOWN;
    EI_countdown = NO_COUNTDOWN;
    clock_cycles += 20;
}

const std::array<const Cpu::InstructionInfo, 256> Cpu::INSTRUCTION_TABLE =
{{
    {1, 4,  4,  &Cpu::op_00},
    {3, 12, 12, &Cpu::op_01},
    {1, 8,  8,  &Cpu::op_02},
    {1, 8,  8,  &Cpu::op_03},
    {1, 4,  4,  &Cpu::op_04},
    {1, 4,  4,  &Cpu::op_05},
    {2, 8,  8,  &Cpu::op_06},
    {1, 4,  4,  &Cpu::op_07},
    {3, 20, 20, &Cpu::op_08},
    {1, 8,  8,  &Cpu::op_09},
    {1, 8,  8,  &Cpu::op_0A},
    {1, 8,  8,  &Cpu::op_0B},
    {1, 4,  4,  &Cpu::op_0C},
    {1, 4,  4,  &Cpu::op_0D},
    {2, 8,  8,  &Cpu::op_0E},
    {1, 4,  4,  &Cpu::op_0F},
    {2, 4,  4,  &Cpu::op_10},
    {3, 12, 12, &Cpu::op_11},
    {1, 8,  8,  &Cpu::op_12},
    {1, 8,  8,  &Cpu::op_13},
    {1, 4,  4,  &Cpu::op_14},
    {1, 4,  4,  &Cpu::op_15},
    {2, 8,  8,  &Cpu::op_16},
    {1, 4,  4,  &Cpu::op_17},
    {2, 12, 12, &Cpu::op_18},
    {1, 8,  8,  &Cpu::op_19},
    {1, 8,  8,  &Cpu::op_1A},
    {1, 8,  8,  &Cpu::op_1B},
    {1, 4,  4,  &Cpu::op_1C},
    {1, 4,  4,  &Cpu::op_1D},
    {2, 8,  8,  &Cpu::op_1E},
    {1, 4,  4,  &Cpu::op_1F},
    {2, 12, 8,  &Cpu::op_20},
    {3, 12, 12, &Cpu::op_21},
    {1, 8,  8,  &Cpu::op_22},
    {1, 8,  8,  &Cpu::op_23},
    {1, 4,  4,  &Cpu::op_24},
    {1, 4,  4,  &Cpu::op_25},
    {2, 8,  8,  &Cpu::op_26},
    {1, 4,  4,  &Cpu::op_27},
    {2, 12, 8,  &Cpu::op_28},
    {1, 8,  8,  &Cpu::op_29},
    {1, 8,  8,  &Cpu::op_2A},
    {1, 8,  8,  &Cpu::op_2B},
    {1, 4,  4,  &Cpu::op_2C},
    {1, 4,  4,  &Cpu::op_2D},
    {2, 8,  8,  &Cpu::op_2E},
    {1, 4,  4,  &Cpu::op_2F},
    {2, 12, 8,  &Cpu::op_30},
    {3, 12, 12, &Cpu::op_31},
    {1, 8,  8,  &Cpu::op_32},
    {1, 8,  8,  &Cpu::op_33},
    {1, 12, 12, &Cpu::op_34},
    {1, 12, 12, &Cpu::op_35},
    {2, 12, 12, &Cpu::op_36},
    {1, 4,  4,  &Cpu::op_37},
    {2, 12, 8,  &Cpu::op_38},
    {1, 8,  8,  &Cpu::op_39},
    {1, 8,  8,  &Cpu::op_3A},
    {1, 8,  8,  &Cpu::op_3B},
    {1, 4,  4,  &Cpu::op_3C},
    {1, 4,  4,  &Cpu::op_3D},
    {2, 8,  8,  &Cpu::op_3E},
    {1, 4,  4,  &Cpu::op_3F},
    {1, 4,  4,  &Cpu::op_40},
    {1, 4,  4,  &Cpu::op_41},
    {1, 4,  4,  &Cpu::op_42},
    {1, 4,  4,  &Cpu::op_43},
    {1, 4,  4,  &Cpu::op_44},
    {1, 4,  4,  &Cpu::op_45},
    {1, 8,  8,  &Cpu::op_46},
    {1, 4,  4,  &Cpu::op_47},
    {1, 4,  4,  &Cpu::op_48},
    {1, 4,  4,  &Cpu::op_49},
    {1, 4,  4,  &Cpu::op_4A},
    {1, 4,  4,  &Cpu::op_4B},
    {1, 4,  4,  &Cpu::op_4C},
    {1, 4,  4,  &Cpu::op_4D},
    {1, 8,  8,  &Cpu::op_4E},
    {1, 4,  4,  &Cpu::op_4F},
    {1, 4,  4,  &Cpu::op_50},
    {1, 4,  4,  &Cpu::op_51},
    {1, 4,  4,  &Cpu::op_52},
    {1, 4,  4,  &Cpu::op_53},
    {1, 4,  4,  &Cpu::op_54},
    {1, 4,  4,  &Cpu::op_55},
    {1, 8,  8,  &Cpu::op_56},
    {1, 4,  4,  &Cpu::op_57},
    {1, 4,  4,  &Cpu::op_58},
    {1, 4,  4,  &Cpu::op_59},
    {1, 4,  4,  &Cpu::op_5A},
    {1, 4,  4,  &Cpu::op_5B},
    {1, 4,  4,  &Cpu::op_5C},
    {1, 4,  4,  &Cpu::op_5D},
    {1, 8,  8,  &Cpu::op_5E},
    {1, 4,  4,  &Cpu::op_5F},
    {1, 4,  4,  &Cpu::op_60},
    {1, 4,  4,  &Cpu::op_61},
    {1, 4,  4,  &Cpu::op_62},
    {1, 4,  4,  &Cpu::op_63},
    {1, 4,  4,  &Cpu::op_64},
    {1, 4,  4,  &Cpu::op_65},
    {1, 8,  8,  &Cpu::op_66},
    {1, 4,  4,  &Cpu::op_67},
    {1, 4,  4,  &Cpu::op_68},
    {1, 4,  4,  &Cpu::op_69},
    {1, 4,  4,  &Cpu::op_6A},
    {1, 4,  4,  &Cpu::op_6B},
    {1, 4,  4,  &Cpu::op_6C},
    {1, 4,  4,  &Cpu::op_6D},
    {1, 8,  8,  &Cpu::op_6E},
    {1, 4,  4,  &Cpu::op_6F},
    {1, 8,  8,  &Cpu::op_70},
    {1, 8,  8,  &Cpu::op_71},
    {1, 8,  8,  &Cpu::op_72},
    {1, 8,  8,  &Cpu::op_73},
    {1, 8,  8,  &Cpu::op_74},
    {1, 8,  8,  &Cpu::op_75},
    {1, 4,  4,  &Cpu::op_76},
    {1, 8,  8,  &Cpu::op_77},
    {1, 4,  4,  &Cpu::op_78},
    {1, 4,  4,  &Cpu::op_79},
    {1, 4,  4,  &Cpu::op_7A},
    {1, 4,  4,  &Cpu::op_7B},
    {1, 4,  4,  &Cpu::op_7C},
    {1, 4,  4,  &Cpu::op_7D},
    {1, 8,  8,  &Cpu::op_7E},
    {1, 4,  4,  &Cpu::op_7F},
    {1, 4,  4,  &Cpu::op_80},
    {1, 4,  4,  &Cpu::op_81},
    {1, 4,  4,  &Cpu::op_82},
    {1, 4,  4,  &Cpu::op_83},
    {1, 4,  4,  &Cpu::op_84},
    {1, 4,  4,  &Cpu::op_85},
    {1, 8,  8,  &Cpu::op_86},
    {1, 4,  4,  &Cpu::op_87},
    {1, 4,  4,  &Cpu::op_88},
    {1, 4,  4,  &Cpu::op_89},
    {1, 4,  4,  &Cpu::op_8A},
    {1, 4,  4,  &Cpu::op_8B},
    {1, 4,  4,  &Cpu::op_8C},
    {1, 4,  4,  &Cpu::op_8D},
    {1, 8,  8,  &Cpu::op_8E},
    {1, 4,  4,  &Cpu::op_8F},
    {1, 4,  4,  &Cpu::op_90},
    {1, 4,  4,  &Cpu::op_91},
    {1, 4,  4,  &Cpu::op_92},
    {1, 4,  4,  &Cpu::op_93},
    {1, 4,  4,  &Cpu::op_94},
    {1, 4,  4,  &Cpu::op_95},
    {1, 8,  8,  &Cpu::op_96},
    {1, 4,  4,  &Cpu::op_97},
    {1, 4,  4,  &Cpu::op_98},
    {1, 4,  4,  &Cpu::op_99},
    {1, 4,  4,  &Cpu::op_9A},
    {1, 4,  4,  &Cpu::op_9B},
    {1, 4,  4,  &Cpu::op_9C},
    {1, 4,  4,  &Cpu::op_9D},
    {1, 8,  8,  &Cpu::op_9E},
    {1, 4,  4,  &Cpu::op_9F},
    {1, 4,  4,  &Cpu::op_A0},
    {1, 4,  4,  &Cpu::op_A1},
    {1, 4,  4,  &Cpu::op_A2},
    {1, 4,  4,  &Cpu::op_A3},
    {1, 4,  4,  &Cpu::op_A4},
    {1, 4,  4,  &Cpu::op_A5},
    {1, 8,  8,  &Cpu::op_A6},
    {1, 4,  4,  &Cpu::op_A7},
    {1, 4,  4,  &Cpu::op_A8},
    {1, 4,  4,  &Cpu::op_A9},
    {1, 4,  4,  &Cpu::op_AA},
    {1, 4,  4,  &Cpu::op_AB},
    {1, 4,  4,  &Cpu::op_AC},
    {1, 4,  4,  &Cpu::op_AD},
    {1, 8,  8,  &Cpu::op_AE},
    {1, 4,  4,  &Cpu::op_AF},
    {1, 4,  4,  &Cpu::op_B0},
    {1, 4,  4,  &Cpu::op_B1},
    {1, 4,  4,  &Cpu::op_B2},
    {1, 4,  4,  &Cpu::op_B3},
    {1, 4,  4,  &Cpu::op_B4},
    {1, 4,  4,  &Cpu::op_B5},
    {1, 8,  8,  &Cpu::op_B6},
    {1, 4,  4,  &Cpu::op_B7},
    {1, 4,  4,  &Cpu::op_B8},
    {1, 4,  4,  &Cpu::op_B9},
    {1, 4,  4,  &Cpu::op_BA},
    {1, 4,  4,  &Cpu::op_BB},
    {1, 4,  4,  &Cpu::op_BC},
    {1, 4,  4,  &Cpu::op_BD},
    {1, 8,  8,  &Cpu::op_BE},
    {1, 4,  4,  &Cpu::op_BF},
    {1, 20, 8,  &Cpu::op_C0},
    {1, 12, 12, &Cpu::op_C1},
    {3, 16, 12, &Cpu::op_C2},
    {3, 16, 16, &Cpu::op_C3},
    {3, 24, 12, &Cpu::op_C4},
    {1, 16, 16, &Cpu::op_C5},
    {2, 8,  8,  &Cpu::op_C6},
    {1, 16, 16, &Cpu::op_C7},
    {1, 20, 8,  &Cpu::op_C8},
    {1, 16, 16, &Cpu::op_C9},
    {3, 16, 12, &Cpu::op_CA},
    {1, 4,  4,  &Cpu::op_CB},
    {3, 24, 12, &Cpu::op_CC},
    {3, 24, 24, &Cpu::op_CD},
    {2, 8,  8,  &Cpu::op_CE},
    {1, 16, 16, &Cpu::op_CF},
    {1, 20, 8,  &Cpu::op_D0},
    {1, 12, 12, &Cpu::op_D1},
    {3, 16, 12, &Cpu::op_D2},
    {0, 0,  0,  &Cpu::invalid_opcode},
    {3, 24, 12, &Cpu::op_D4},
    {1, 16, 16, &Cpu::op_D5},
    {2, 8,  8,  &Cpu::op_D6},
    {1, 16, 16, &Cpu::op_D7},
    {1, 20, 8,  &Cpu::op_D8},
    {1, 16, 16, &Cpu::op_D9},
    {3, 16, 12, &Cpu::op_DA},
    {0, 0,  0,  &Cpu::invalid_opcode},
    {3, 24, 12, &Cpu::op_DC},
    {0, 0,  0,  &Cpu::invalid_opcode},
    {2, 8,  8,  &Cpu::op_DE},
    {1, 16, 16, &Cpu::op_DF},
    {2, 12, 12, &Cpu::op_E0},
    {1, 12, 12, &Cpu::op_E1},
    {1, 8,  8,  &Cpu::op_E2},
    {0, 0,  0,  &Cpu::invalid_opcode},
    {0, 0,  0,  &Cpu::invalid_opcode},
    {1, 16, 16, &Cpu::op_E5},
    {2, 8,  8,  &Cpu::op_E6},
    {1, 16, 16, &Cpu::op_E7},
    {2, 16, 16, &Cpu::op_E8},
    {1, 4,  4,  &Cpu::op_E9},
    {3, 16, 16, &Cpu::op_EA},
    {0, 0,  0,  &Cpu::invalid_opcode},
    {0, 0,  0,  &Cpu::invalid_opcode},
    {0, 0,  0,  &Cpu::invalid_opcode},
    {2, 8,  8,  &Cpu::op_EE},
    {1, 16, 16, &Cpu::op_EF},
    {2, 12, 12, &Cpu::op_F0},
    {1, 12, 12, &Cpu::op_F1},
    {1, 8,  8,  &Cpu::op_F2},
    {1, 4,  4,  &Cpu::op_F3},
    {0, 0,  0,  &Cpu::invalid_opcode},
    {1, 16, 16, &Cpu::op_F5},
    {2, 8,  8,  &Cpu::op_F6},
    {1, 16, 16, &Cpu::op_F7},
    {2, 12, 12, &Cpu::op_F8},
    {1, 8,  8,  &Cpu::op_F9},
    {3, 16, 16, &Cpu::op_FA},
    {1, 1,  4,  &Cpu::op_FB},
    {0, 0,  0,  &Cpu::invalid_opcode},
    {0, 0,  0,  &Cpu::invalid_opcode},
    {2, 8,  8,  &Cpu::op_FE},
    {1, 16, 16, &Cpu::op_FF}
}};

const std::array<const Cpu::InstructionInfo, 256> Cpu::CB_INSTRUCTION_TABLE =
{{
    {2, 8,  8,  &Cpu::op_CB_00},
    {2, 8,  8,  &Cpu::op_CB_01},
    {2, 8,  8,  &Cpu::op_CB_02},
    {2, 8,  8,  &Cpu::op_CB_03},
    {2, 8,  8,  &Cpu::op_CB_04},
    {2, 8,  8,  &Cpu::op_CB_05},
    {2, 16, 16, &Cpu::op_CB_06},
    {2, 8,  8,  &Cpu::op_CB_07},
    {2, 8,  8,  &Cpu::op_CB_08},
    {2, 8,  8,  &Cpu::op_CB_09},
    {2, 8,  8,  &Cpu::op_CB_0A},
    {2, 8,  8,  &Cpu::op_CB_0B},
    {2, 8,  8,  &Cpu::op_CB_0C},
    {2, 8,  8,  &Cpu::op_CB_0D},
    {2, 16, 16, &Cpu::op_CB_0E},
    {2, 8,  8,  &Cpu::op_CB_0F},
    {2, 8,  8,  &Cpu::op_CB_10},
    {2, 8,  8,  &Cpu::op_CB_11},
    {2, 8,  8,  &Cpu::op_CB_12},
    {2, 8,  8,  &Cpu::op_CB_13},
    {2, 8,  8,  &Cpu::op_CB_14},
    {2, 8,  8,  &Cpu::op_CB_15},
    {2, 16, 16, &Cpu::op_CB_16},
    {2, 8,  8,  &Cpu::op_CB_17},
    {2, 8,  8,  &Cpu::op_CB_18},
    {2, 8,  8,  &Cpu::op_CB_19},
    {2, 8,  8,  &Cpu::op_CB_1A},
    {2, 8,  8,  &Cpu::op_CB_1B},
    {2, 8,  8,  &Cpu::op_CB_1C},
    {2, 8,  8,  &Cpu::op_CB_1D},
    {2, 16, 16, &Cpu::op_CB_1E},
    {2, 8,  8,  &Cpu::op_CB_1F},
    {2, 8,  8,  &Cpu::op_CB_20},
    {2, 8,  8,  &Cpu::op_CB_21},
    {2, 8,  8,  &Cpu::op_CB_22},
    {2, 8,  8,  &Cpu::op_CB_23},
    {2, 8,  8,  &Cpu::op_CB_24},
    {2, 8,  8,  &Cpu::op_CB_25},
    {2, 16, 16, &Cpu::op_CB_26},
    {2, 8,  8,  &Cpu::op_CB_27},
    {2, 8,  8,  &Cpu::op_CB_28},
    {2, 8,  8,  &Cpu::op_CB_29},
    {2, 8,  8,  &Cpu::op_CB_2A},
    {2, 8,  8,  &Cpu::op_CB_2B},
    {2, 8,  8,  &Cpu::op_CB_2C},
    {2, 8,  8,  &Cpu::op_CB_2D},
    {2, 16, 16, &Cpu::op_CB_2E},
    {2, 8,  8,  &Cpu::op_CB_2F},
    {2, 8,  8,  &Cpu::op_CB_30},
    {2, 8,  8,  &Cpu::op_CB_31},
    {2, 8,  8,  &Cpu::op_CB_32},
    {2, 8,  8,  &Cpu::op_CB_33},
    {2, 8,  8,  &Cpu::op_CB_34},
    {2, 8,  8,  &Cpu::op_CB_35},
    {2, 16, 16, &Cpu::op_CB_36},
    {2, 8,  8,  &Cpu::op_CB_37},
    {2, 8,  8,  &Cpu::op_CB_38},
    {2, 8,  8,  &Cpu::op_CB_39},
    {2, 8,  8,  &Cpu::op_CB_3A},
    {2, 8,  8,  &Cpu::op_CB_3B},
    {2, 8,  8,  &Cpu::op_CB_3C},
    {2, 8,  8,  &Cpu::op_CB_3D},
    {2, 16, 16, &Cpu::op_CB_3E},
    {2, 8,  8,  &Cpu::op_CB_3F},
    {2, 8,  8,  &Cpu::op_CB_40},
    {2, 8,  8,  &Cpu::op_CB_41},
    {2, 8,  8,  &Cpu::op_CB_42},
    {2, 8,  8,  &Cpu::op_CB_43},
    {2, 8,  8,  &Cpu::op_CB_44},
    {2, 8,  8,  &Cpu::op_CB_45},
    {2, 12, 12, &Cpu::op_CB_46},
    {2, 8,  8,  &Cpu::op_CB_47},
    {2, 8,  8,  &Cpu::op_CB_48},
    {2, 8,  8,  &Cpu::op_CB_49},
    {2, 8,  8,  &Cpu::op_CB_4A},
    {2, 8,  8,  &Cpu::op_CB_4B},
    {2, 8,  8,  &Cpu::op_CB_4C},
    {2, 8,  8,  &Cpu::op_CB_4D},
    {2, 12, 12, &Cpu::op_CB_4E},
    {2, 8,  8,  &Cpu::op_CB_4F},
    {2, 8,  8,  &Cpu::op_CB_50},
    {2, 8,  8,  &Cpu::op_CB_51},
    {2, 8,  8,  &Cpu::op_CB_52},
    {2, 8,  8,  &Cpu::op_CB_53},
    {2, 8,  8,  &Cpu::op_CB_54},
    {2, 8,  8,  &Cpu::op_CB_55},
    {2, 12, 12, &Cpu::op_CB_56},
    {2, 8,  8,  &Cpu::op_CB_57},
    {2, 8,  8,  &Cpu::op_CB_58},
    {2, 8,  8,  &Cpu::op_CB_59},
    {2, 8,  8,  &Cpu::op_CB_5A},
    {2, 8,  8,  &Cpu::op_CB_5B},
    {2, 8,  8,  &Cpu::op_CB_5C},
    {2, 8,  8,  &Cpu::op_CB_5D},
    {2, 12, 12, &Cpu::op_CB_5E},
    {2, 8,  8,  &Cpu::op_CB_5F},
    {2, 8,  8,  &Cpu::op_CB_60},
    {2, 8,  8,  &Cpu::op_CB_61},
    {2, 8,  8,  &Cpu::op_CB_62},
    {2, 8,  8,  &Cpu::op_CB_63},
    {2, 8,  8,  &Cpu::op_CB_64},
    {2, 8,  8,  &Cpu::op_CB_65},
    {2, 12, 12, &Cpu::op_CB_66},
    {2, 8,  8,  &Cpu::op_CB_67},
    {2, 8,  8,  &Cpu::op_CB_68},
    {2, 8,  8,  &Cpu::op_CB_69},
    {2, 8,  8,  &Cpu::op_CB_6A},
    {2, 8,  8,  &Cpu::op_CB_6B},
    {2, 8,  8,  &Cpu::op_CB_6C},
    {2, 8,  8,  &Cpu::op_CB_6D},
    {2, 12, 12, &Cpu::op_CB_6E},
    {2, 8,  8,  &Cpu::op_CB_6F},
    {2, 8,  8,  &Cpu::op_CB_70},
    {2, 8,  8,  &Cpu::op_CB_71},
    {2, 8,  8,  &Cpu::op_CB_72},
    {2, 8,  8,  &Cpu::op_CB_73},
    {2, 8,  8,  &Cpu::op_CB_74},
    {2, 8,  8,  &Cpu::op_CB_75},
    {2, 12, 12, &Cpu::op_CB_76},
    {2, 8,  8,  &Cpu::op_CB_77},
    {2, 8,  8,  &Cpu::op_CB_78},
    {2, 8,  8,  &Cpu::op_CB_79},
    {2, 8,  8,  &Cpu::op_CB_7A},
    {2, 8,  8,  &Cpu::op_CB_7B},
    {2, 8,  8,  &Cpu::op_CB_7C},
    {2, 8,  8,  &Cpu::op_CB_7D},
    {2, 12, 12, &Cpu::op_CB_7E},
    {2, 8,  8,  &Cpu::op_CB_7F},
    {2, 8,  8,  &Cpu::op_CB_80},
    {2, 8,  8,  &Cpu::op_CB_81},
    {2, 8,  8,  &Cpu::op_CB_82},
    {2, 8,  8,  &Cpu::op_CB_83},
    {2, 8,  8,  &Cpu::op_CB_84},
    {2, 8,  8,  &Cpu::op_CB_85},
    {2, 16, 16, &Cpu::op_CB_86},
    {2, 8,  8,  &Cpu::op_CB_87},
    {2, 8,  8,  &Cpu::op_CB_88},
    {2, 8,  8,  &Cpu::op_CB_89},
    {2, 8,  8,  &Cpu::op_CB_8A},
    {2, 8,  8,  &Cpu::op_CB_8B},
    {2, 8,  8,  &Cpu::op_CB_8C},
    {2, 8,  8,  &Cpu::op_CB_8D},
    {2, 16, 16, &Cpu::op_CB_8E},
    {2, 8,  8,  &Cpu::op_CB_8F},
    {2, 8,  8,  &Cpu::op_CB_90},
    {2, 8,  8,  &Cpu::op_CB_91},
    {2, 8,  8,  &Cpu::op_CB_92},
    {2, 8,  8,  &Cpu::op_CB_93},
    {2, 8,  8,  &Cpu::op_CB_94},
    {2, 8,  8,  &Cpu::op_CB_95},
    {2, 16, 16, &Cpu::op_CB_96},
    {2, 8,  8,  &Cpu::op_CB_97},
    {2, 8,  8,  &Cpu::op_CB_98},
    {2, 8,  8,  &Cpu::op_CB_99},
    {2, 8,  8,  &Cpu::op_CB_9A},
    {2, 8,  8,  &Cpu::op_CB_9B},
    {2, 8,  8,  &Cpu::op_CB_9C},
    {2, 8,  8,  &Cpu::op_CB_9D},
    {2, 16, 16, &Cpu::op_CB_9E},
    {2, 8,  8,  &Cpu::op_CB_9F},
    {2, 8,  8,  &Cpu::op_CB_A0},
    {2, 8,  8,  &Cpu::op_CB_A1},
    {2, 8,  8,  &Cpu::op_CB_A2},
    {2, 8,  8,  &Cpu::op_CB_A3},
    {2, 8,  8,  &Cpu::op_CB_A4},
    {2, 8,  8,  &Cpu::op_CB_A5},
    {2, 16, 16, &Cpu::op_CB_A6},
    {2, 8,  8,  &Cpu::op_CB_A7},
    {2, 8,  8,  &Cpu::op_CB_A8},
    {2, 8,  8,  &Cpu::op_CB_A9},
    {2, 8,  8,  &Cpu::op_CB_AA},
    {2, 8,  8,  &Cpu::op_CB_AB},
    {2, 8,  8,  &Cpu::op_CB_AC},
    {2, 8,  8,  &Cpu::op_CB_AD},
    {2, 16, 16, &Cpu::op_CB_AE},
    {2, 8,  8,  &Cpu::op_CB_AF},
    {2, 8,  8,  &Cpu::op_CB_B0},
    {2, 8,  8,  &Cpu::op_CB_B1},
    {2, 8,  8,  &Cpu::op_CB_B2},
    {2, 8,  8,  &Cpu::op_CB_B3},
    {2, 8,  8,  &Cpu::op_CB_B4},
    {2, 8,  8,  &Cpu::op_CB_B5},
    {2, 16, 16, &Cpu::op_CB_B6},
    {2, 8,  8,  &Cpu::op_CB_B7},
    {2, 8,  8,  &Cpu::op_CB_B8},
    {2, 8,  8,  &Cpu::op_CB_B9},
    {2, 8,  8,  &Cpu::op_CB_BA},
    {2, 8,  8,  &Cpu::op_CB_BB},
    {2, 8,  8,  &Cpu::op_CB_BC},
    {2, 8,  8,  &Cpu::op_CB_BD},
    {2, 16, 16, &Cpu::op_CB_BE},
    {2, 8,  8,  &Cpu::op_CB_BF},
    {2, 8,  8,  &Cpu::op_CB_C0},
    {2, 8,  8,  &Cpu::op_CB_C1},
    {2, 8,  8,  &Cpu::op_CB_C2},
    {2, 8,  8,  &Cpu::op_CB_C3},
    {2, 8,  8,  &Cpu::op_CB_C4},
    {2, 8,  8,  &Cpu::op_CB_C5},
    {2, 16, 16, &Cpu::op_CB_C6},
    {2, 8,  8,  &Cpu::op_CB_C7},
    {2, 8,  8,  &Cpu::op_CB_C8},
    {2, 8,  8,  &Cpu::op_CB_C9},
    {2, 8,  8,  &Cpu::op_CB_CA},
    {2, 8,  8,  &Cpu::op_CB_CB},
    {2, 8,  8,  &Cpu::op_CB_CC},
    {2, 8,  8,  &Cpu::op_CB_CD},
    {2, 16, 16, &Cpu::op_CB_CE},
    {2, 8,  8,  &Cpu::op_CB_CF},
    {2, 8,  8,  &Cpu::op_CB_D0},
    {2, 8,  8,  &Cpu::op_CB_D1},
    {2, 8,  8,  &Cpu::op_CB_D2},
    {2, 8,  8,  &Cpu::op_CB_D3},
    {2, 8,  8,  &Cpu::op_CB_D4},
    {2, 8,  8,  &Cpu::op_CB_D5},
    {2, 16, 16, &Cpu::op_CB_D6},
    {2, 8,  8,  &Cpu::op_CB_D7},
    {2, 8,  8,  &Cpu::op_CB_D8},
    {2, 8,  8,  &Cpu::op_CB_D9},
    {2, 8,  8,  &Cpu::op_CB_DA},
    {2, 8,  8,  &Cpu::op_CB_DB},
    {2, 8,  8,  &Cpu::op_CB_DC},
    {2, 8,  8,  &Cpu::op_CB_DD},
    {2, 16, 16, &Cpu::op_CB_DE},
    {2, 8,  8,  &Cpu::op_CB_DF},
    {2, 8,  8,  &Cpu::op_CB_E0},
    {2, 8,  8,  &Cpu::op_CB_E1},
    {2, 8,  8,  &Cpu::op_CB_E2},
    {2, 8,  8,  &Cpu::op_CB_E3},
    {2, 8,  8,  &Cpu::op_CB_E4},
    {2, 8,  8,  &Cpu::op_CB_E5},
    {2, 16, 16, &Cpu::op_CB_E6},
    {2, 8,  8,  &Cpu::op_CB_E7},
    {2, 8,  8,  &Cpu::op_CB_E8},
    {2, 8,  8,  &Cpu::op_CB_E9},
    {2, 8,  8,  &Cpu::op_CB_EA},
    {2, 8,  8,  &Cpu::op_CB_EB},
    {2, 8,  8,  &Cpu::op_CB_EC},
    {2, 8,  8,  &Cpu::op_CB_ED},
    {2, 16, 16, &Cpu::op_CB_EE},
    {2, 8,  8,  &Cpu::op_CB_EF},
    {2, 8,  8,  &Cpu::op_CB_F0},
    {2, 8,  8,  &Cpu::op_CB_F1},
    {2, 8,  8,  &Cpu::op_CB_F2},
    {2, 8,  8,  &Cpu::op_CB_F3},
    {2, 8,  8,  &Cpu::op_CB_F4},
    {2, 8,  8,  &Cpu::op_CB_F5},
    {2, 16, 16, &Cpu::op_CB_F6},
    {2, 8,  8,  &Cpu::op_CB_F7},
    {2, 8,  8,  &Cpu::op_CB_F8},
    {2, 8,  8,  &Cpu::op_CB_F9},
    {2, 8,  8,  &Cpu::op_CB_FA},
    {2, 8,  8,  &Cpu::op_CB_FB},
    {2, 8,  8,  &Cpu::op_CB_FC},
    {2, 8,  8,  &Cpu::op_CB_FD},
    {2, 16, 16, &Cpu::op_CB_FE},
    {2, 8,  8,  &Cpu::op_CB_FF}
}};
