#include "emulator/emulator.hh"
#include "tests/common/serialfrontend.hh"

#include "gtest/gtest.h"

namespace
{

class BlarggsCpuInstrsTest
    : public ::testing::TestWithParam<std::string>
{
protected:
    void SetUp() override
    {
        rom_filepath_ = GetParam();

        emulator_.load_rom(rom_filepath_);
        emulator_.set_frontend(&serial_);
    }

    void TearDown() override
    {
    }

    std::string rom_filepath_;
    SerialFrontend serial_;
    Emulator emulator_;
};

TEST_P(BlarggsCpuInstrsTest, run)
{
    for (uint64_t i = 0; i < 100000000ULL; ++i)
    {
        emulator_.execute_next();

        if (serial_.get_received().find("Pass") != std::string::npos)
        {
            return;
        }

        ASSERT_FALSE(serial_.get_received().find("Fail") != std::string::npos);
    }

    bool timeout = true;
    ASSERT_FALSE(timeout);
}

INSTANTIATE_TEST_SUITE_P(
    ,
    BlarggsCpuInstrsTest,
    testing::Values(
        "gb-test-roms/cpu_instrs/individual/01-special.gb",
        "gb-test-roms/cpu_instrs/individual/02-interrupts.gb",
        "gb-test-roms/cpu_instrs/individual/03-op sp,hl.gb",
        "gb-test-roms/cpu_instrs/individual/04-op r,imm.gb",
        "gb-test-roms/cpu_instrs/individual/05-op rp.gb",
        "gb-test-roms/cpu_instrs/individual/06-ld r,r.gb",
        "gb-test-roms/cpu_instrs/individual/07-jr,jp,call,ret,rst.gb",
        "gb-test-roms/cpu_instrs/individual/08-misc instrs.gb",
        "gb-test-roms/cpu_instrs/individual/09-op r,r.gb",
        "gb-test-roms/cpu_instrs/individual/10-bit ops.gb",
        "gb-test-roms/cpu_instrs/individual/11-op a,(hl).gb"
    )
);

} // namespace