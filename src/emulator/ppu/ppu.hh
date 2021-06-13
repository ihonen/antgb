#pragma once

#include "debugger/iemulator.hh"
#include "emulator/interfaces/iemulatorcomponent.hh"
#include "emulator/common/bitmanip.hh"
#include "emulator/cpu/interrupts.hh"
#include "emulator/memory/dma.hh"
#include "emulator/common/types.hh"
#include <chrono>
#include <map>
#include <memory>
#include <vector>

using namespace std;

using std::vector;

class MemoryBus;
class PpuRegisters;
class Renderer;

class Ppu : public iEmulatorComponent
{
public:
    enum Mode
    {
        Hblank   = 0x00,
        Vblank   = 0x01,
        LineScan = 0x02,
        OamScan  = 0x03
    };

    static constexpr uint64_t CPU_CYCLES_PER_LY_INCREMENT = 456;

    static constexpr uint64_t CPU_CYCLES_PER_HBLANK_MODE = 85; // Minimum
    static constexpr uint64_t CPU_CYCLES_PER_VBLANK_MODE = 4560;
    static constexpr uint64_t CPU_CYCLES_PER_DRAWING_MODE = 291; // Maximum
    static constexpr uint64_t CPU_CYCLES_PER_OAM_SCAN_MODE = 80;


    std::map<Mode, uint64_t> MODE_DURATION{{Hblank, 85},
                                           {Vblank, 4560},
                                           {LineScan, 291},
                                           {OamScan, 80}};

    bool mode_task_complete;
    bool frame_ready;
    emutime_t tcycles_spent_in_mode;
    Mode current_mode;
    emutime_t unemulated_tcycles;

    emutime_t tcycles_left_in_mode;
    emutime_t tcycles_until_ly;

    emutime_t clocksum = 0;
    uint64_t scanline = 0;

    const uint8_t MODE_FLAG_MASK = 0x03; // Bits 0-1

    Ppu(PpuRegisters& reg,
        Interrupts& interrupts,
        MemoryBus& mem,
        Dma& dma);
    virtual ~Ppu() override;

    void set_render_callback(iFrontend::RenderCallback callback);

    void hard_reset();

    virtual void pre_cpu_exec_tick() override {}
    virtual void post_cpu_exec_tick(emutime_t cpu_cycles) override;

    bool has_dma_request();
    addr_t dma_src_address();
    void launch_dma(addr_t src_address);

    PpuRegisters& reg;
    Interrupts& interrupts;
    MemoryBus& mem;
    Dma& dma;
    std::unique_ptr<Renderer> renderer;
};
