#pragma once

#include "renderer.hh"
#include "cpu.hh"
#include "interrupts.hh"
#include "memory.hh"
#include <chrono>
#include <map>
#include <vector>

using std::vector;

class Ppu
{
public:

    enum Mode
    {
        HBlanking = 0x00,
        VBlanking = 0x01,
        DrawingLine = 0x02,
        ScanningOAM = 0x03
    };

    static constexpr uint64_t CPU_CYCLES_PER_LY_INCREMENT = 456;

    static constexpr uint64_t CPU_CYCLES_PER_HBLANK_MODE = 85; // Minimum
    static constexpr uint64_t CPU_CYCLES_PER_VBLANK_MODE = 4560;
    static constexpr uint64_t CPU_CYCLES_PER_DRAWING_MODE = 291; // Maximum
    static constexpr uint64_t CPU_CYCLES_PER_OAM_SCAN_MODE = 80;

    std::map<Mode, uint64_t> MODE_DURATION
    {
        {HBlanking, 85},
        {VBlanking, 4560},
        {DrawingLine, 291},
        {ScanningOAM, 80}
    };

    struct
    {
        bool mode_task_complete;
        bool frame_ready;
        uint64_t cpu_cycles_spent_in_mode;
        Mode current_mode;
        uint64_t unemulated_cpu_cycles;
    } status;

    const uint8_t MODE_FLAG_MASK = 0x03; // Bits 0-1

    Ppu(Memory* mem, Irc* irc);
    ~Ppu();
    void hard_reset();
    void emulate(uint64_t cpu_cycles);
    void emulate_current_mode(uint64_t cpu_cycles);
    void emulate_mode0(uint64_t cpu_cycles);
    void emulate_mode1(uint64_t cpu_cycles);
    void emulate_mode2(uint64_t cpu_cycles);
    void emulate_mode3(uint64_t cpu_cycles);
    bool has_dma_request();
    memaddr_t dma_src_address();
    void launch_dma(memaddr_t src_address);
    bool mode_ending();
    Mode get_next_mode();
    void transition_to_mode(Mode mode);
    void scan_oam();
    vector<vector<uint8_t>> read_tile(void* address);

    Irc* irc;
    Memory* mem;
    Renderer* renderer;
};
