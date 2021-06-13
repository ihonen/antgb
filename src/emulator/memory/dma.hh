#pragma once

#include "emulator/interfaces/iemulatorcomponent.hh"
#include "memorybus.hh"

class Dma : public iEmulatorComponent
{
public:
    Dma(MemoryBus& memory_bus);

    void launch_oam_dma(addr_t destination, addr_t source, addr_t size);
    void end_oam_dma();
    virtual void pre_cpu_exec_tick() override {}
    virtual void post_cpu_exec_tick(emutime_t tcycles) override;
    void clear_dma_status();

protected:
    MemoryBus& memory_bus_;

    emutime_t unemulated_tcycles_;
    emutime_t tcycles_left_;
    addr_t src_pointer_;
    addr_t dest_pointer_;
    addr_t src_low_;
    addr_t src_high_;
    addr_t size_;
};
