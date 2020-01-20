#include "graphicsfetcher.hh"


GraphicsFetcher::GraphicsFetcher(MMU& mmu_, PPUReg& reg_) :
    background_buffer(Background(&bg_tilemap)),
    window_buffer(Background(&window_tilemap)),
    mmu(mmu_),
    reg(reg_)
{
    background_buffer = Background(&bg_tilemap);
    window_buffer = Background(&window_tilemap);
}
