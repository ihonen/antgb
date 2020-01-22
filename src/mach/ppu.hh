#ifndef PPU_HH
#define PPU_HH

#include "cpu.hh"
#include "irc.hh"
#include "mmu.hh"
#include "ppuregisters.hh"
#include <chrono>
#include <map>
#include <vector>

using std::vector;

class PPU
{
public:

    enum Mode
    {
        HBlanking = 0x00,
        VBlanking = 0x01,
        DrawingLine = 0x02,
        ScanningOAM = 0x03
    };

    const uint64_t CPU_CYCLES_PER_HBLANK_MODE = 85; // Minimum
    const uint64_t CPU_CYCLES_PER_VBLANK_MODE = 4560;
    const uint64_t CPU_CYCLES_PER_DRAWING_MODE = 291; // Maximum
    const uint64_t CPU_CYCLES_PER_OAM_SCAN_MODE = 80;

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

    PPU(MMU& mmu, IRC& irc);
    void emulate(uint64_t cpu_cycles);
    bool has_dma_request();
    memaddr_t dma_src_address();
    void launch_dma(memaddr_t src_address);
    void process_mode();
    bool mode_ending();
    void next_mode();
    void scan_oam();
    vector<vector<uint8_t>> read_tile(void* address);

    IRC& irc;
    MMU& mmu;

    PPUReg reg;
};

#endif // PPU_HH
