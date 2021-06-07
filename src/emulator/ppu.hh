#pragma once

#include "bitmanip.hh"
#include "cpu.hh"
#include "types.hh"
#include <chrono>
#include <map>
#include <vector>

using namespace std;

using std::vector;

class iFrontend;
class Memory;
class PpuRegisters;
class Renderer;

class Ppu
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

    PpuRegisters& reg;

    std::map<Mode, uint64_t> MODE_DURATION{{Hblank, 85},
                                           {Vblank, 4560},
                                           {LineScan, 291},
                                           {OamScan, 80}};

    bool mode_task_complete;
    bool frame_ready;
    uint64_t cpu_cycles_spent_in_mode;
    Mode current_mode;
    uint64_t unemulated_cpu_cycles;

    uint64_t cpu_cycles_left_in_mode;
    uint64_t cpu_cycles_until_ly;

    uint64_t clocksum = 0;
    uint64_t scanline = 0;

    const uint8_t MODE_FLAG_MASK = 0x03; // Bits 0-1

    Ppu(Memory* mem, PpuRegisters& reg, Cpu* cpu, iFrontend* renderer = nullptr);
    ~Ppu();
    void set_frontend(iFrontend* frontend);
    void hard_reset();
    void step(uint64_t cpu_cycles);
    bool has_dma_request();
    memaddr_t dma_src_address();
    void launch_dma(memaddr_t src_address);

    Cpu* cpu;
    Memory* mem;
    Renderer* renderer;
};
