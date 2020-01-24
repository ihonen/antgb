#ifndef MMU_HH
#define MMU_HH

#include "bootrom.hh"
#include "memory.hh"
#include <cstdint>
#include <array>
#include <vector>

using namespace std;

class MMU
{
public:
    MMU();

    uint8_t read(memaddr_t address);
    bool write(memaddr_t address, uint8_t value);
    bool can_read(memaddr_t address);
    bool can_write(memaddr_t address);
    void launch_oam_dma(memaddr_t destination,
                        memaddr_t source,
                        memaddr_t size);
    bool is_in_locked_region(memaddr_t address);
    void lock_region(memaddr_t low, memaddr_t high);
    void unlock_region(memaddr_t low, memaddr_t high);
    void emulate(uint64_t cpu_cycles);
    void emulate_oam_dma(uint64_t cpu_cycles);
    void end_oam_dma();
    void clear_dma_status();

    Memory mem;
    BootROM bootrom;

    struct
    {
        uint64_t unemulated_cpu_cycles;
        uint64_t cpu_cycles_left;
        memaddr_t src_pointer;
        memaddr_t dest_pointer;
        memaddr_t src_low;
        memaddr_t src_high;
        memaddr_t size;
    } dma;

    vector<pair<memaddr_t, memaddr_t>> locked_regions;
    bool is_bootrom_enabled;
};

#endif // MMU_HH
