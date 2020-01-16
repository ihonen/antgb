#ifndef PPU_HH
#define PPU_HH

#include "mmu.hh"
#include <vector>

using std::vector;

class PPU
{
public:
    PPU(MMU& mmu);
    vector<vector<uint8_t>> read_tile(void* address);

    MMU mmu;
};

#endif // PPU_HH
