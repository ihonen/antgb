#include "emulator/emulator.hh"

#include "emulator/joypad.hh"
#include "emulator/ppu.hh"
#include "emulator/serial.hh"
#include "emulator/timer.hh"

#include "emulator/cpu/cpu.hh"
#include "emulator/cpu/cpuregisters.hh"

#include "emulator/memory/addresses.hh"
#include "emulator/memory/bootrom.hh"
#include "emulator/memory/cartridge.hh"
#include "emulator/memory/hram.hh"
#include "emulator/memory/memorybus.hh"
#include "emulator/memory/oam.hh"
#include "emulator/memory/vram.hh"
#include "emulator/memory/wram0.hh"
#include "emulator/memory/wram1.hh"

#include "emulator/peripherals/apuregisters.hh"
#include "emulator/peripherals/joypadregisters.hh"
#include "emulator/peripherals/serialregisters.hh"
#include "emulator/peripherals/timerregisters.hh"

#include "emulator/ppu/ppuregisters.hh"


Emulator::Emulator()
    : bootrom_(std::make_unique<BootRom>())
    , hram_(std::make_unique<Hram>())
    , oam_(std::make_unique<Oam>())
    , vram_(std::make_unique<Vram>())
    , wram0_(std::make_unique<Wram0>())
    , wram1_(std::make_unique<Wram1>())
    , cartridge_(std::make_unique<Cartridge>())
    , apu_registers_(std::make_unique<ApuRegisters>())
    , cpu_registers_(std::make_unique<CpuRegisters>())
    , joypad_registers_(std::make_unique<JoypadRegisters>())
    , ppu_registers_(std::make_unique<PpuRegisters>())
    , serial_registers_(std::make_unique<SerialRegisters>())
    , timer_registers_(std::make_unique<TimerRegisters>())
    , memory_bus_(std::make_unique<MemoryBus>(
        *bootrom_,
        *hram_,
        *oam_,
        *vram_,
        *wram0_,
        *wram1_,
        *cartridge_,
        *apu_registers_,
        *cpu_registers_,
        *joypad_registers_,
        *ppu_registers_,
        *serial_registers_,
        *timer_registers_))
    , cpu_(std::make_unique<Cpu>(*cpu_registers_, *memory_bus_))
    , joypad_(std::make_unique<Joypad>(*joypad_registers_))
    , ppu_(std::make_unique<Ppu>(*ppu_registers_, *memory_bus_))
    , serial_(std::make_unique<Serial>(*serial_registers_))
    , timer_(std::make_unique<Timer>(*timer_registers_))
{
}

Emulator::~Emulator() = default;

void Emulator::set_frontend(iFrontend* frontend)
{
    ppu_->set_frontend(frontend);
}

void Emulator::load_rom(const std::string& filepath)
{
    cartridge_= std::make_unique<Cartridge>(filepath);

    //memory_bus_->set_cartridge(cartridge.get());
    reset_emulation();
}

void Emulator::button_pressed(JoypadButton button)
{
    joypad_->button_pressed(button);
}

void Emulator::button_released(JoypadButton button)
{
    joypad_->button_released(button);
}

void Emulator::reset_emulation()
{
    memory_bus_->hard_reset();
    cpu_->hard_reset();
}

int Emulator::execute_next()
{
    uint64_t cpu_cycle_count_before = cpu_->get_cycles();
    cpu_->execute();
    uint64_t cpu_cycle_count_after = cpu_->get_cycles();
    uint64_t clock_cycles = cpu_cycle_count_after - cpu_cycle_count_before;

    timer_->emulate(clock_cycles);
    memory_bus_->emulate(clock_cycles);
    ppu_->step(clock_cycles);
    serial_->emulate(clock_cycles);

    return static_cast<int>(clock_cycles);
}

uint16_t Emulator::read(regid_t register_id)
{
    if (register_id <= 0xFFFF)
        return memory_bus_->read(static_cast<memaddr_t>(register_id));

    switch (register_id)
    {
        case RegA:
            return cpu_->A;
        case RegF:
            return cpu_->F;
        case RegAF:
            return cpu_->AF;
        case RegB:
            return cpu_->B;
        case RegC:
            return cpu_->C;
        case RegBC:
            return cpu_->BC;
        case RegD:
            return cpu_->D;
        case RegE:
            return cpu_->E;
        case RegDE:
            return cpu_->DE;
        case RegH:
            return cpu_->H;
        case RegL:
            return cpu_->L;
        case RegHL:
            return cpu_->HL;
        case RegPC:
            return cpu_->PC;
        case RegSP:
            return cpu_->SP;
        case RegIME:
            return cpu_->IME;
        default:
            cerr << std::hex << register_id << endl;
            assert(false);
            break;
    }

    return 0xFFFF;
}

void Emulator::write(regid_t register_id, uint16_t value)
{
    if (register_id <= 0xFFFF)
    {
        memory_bus_->write(static_cast<memaddr_t>(register_id), static_cast<uint8_t>(value));
        return;
    }

    switch (register_id)
    {
        case RegA:
            cpu_->A = static_cast<uint8_t>(value);
            break;
        case RegF:
            // Only the bottom 4 bits of F are writable.
            cpu_->F |= (0x0F) & static_cast<uint8_t>(value);
            break;
        case RegAF:
            cpu_->AF = value;
            break;
        case RegB:
            cpu_->B = static_cast<uint8_t>(value);
            break;
        case RegC:
            cpu_->C = static_cast<uint8_t>(value);
            break;
        case RegBC:
            cpu_->BC = value;
            break;
        case RegD:
            cpu_->D = static_cast<uint8_t>(value);
            break;
        case RegE:
            cpu_->E = static_cast<uint8_t>(value);
            break;
        case RegDE:
            cpu_->DE = value;
            break;
        case RegH:
            cpu_->H = static_cast<uint8_t>(value);
            break;
        case RegL:
            cpu_->L = static_cast<uint8_t>(value);
            break;
        case RegHL:
            cpu_->HL = value;
            break;
        case RegPC:
            cpu_->PC = value;
            break;
        case RegSP:
            cpu_->SP = value;
            break;
        case RegIME:
            cpu_->IME = (value != 0) ? 1 : 0;
            break;
        default:
            cerr << std::hex << register_id << endl;
            assert(false);
            break;
    }
}
