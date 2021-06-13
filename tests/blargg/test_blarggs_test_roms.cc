#include "emulator/emulator.hh"

#include "gtest/gtest.h"

namespace
{

class BlarggsTestRomsTest
    : public ::testing::TestWithParam<std::string>
{
protected:
    void SetUp() override
    {
        rom_filepath_ = GetParam();

        emulator_.load_rom(rom_filepath_);
        emulator_.set_serial_callback([&](auto p) { serial_received_ += static_cast<char>(p); });
    }

    void TearDown() override
    {
    }

    std::string rom_filepath_;
    std::string serial_received_;
    Emulator emulator_;
};

TEST_P(BlarggsTestRomsTest, run)
{


    for (uint64_t i = 0; i < 100000000ULL; ++i)
    {
        emulator_.execute_next();

        if (serial_received_.find("Pass") != std::string::npos)
        {
            return;
        }

        ASSERT_FALSE(serial_received_.find("Fail") != std::string::npos);
    }

    bool timeout = true;
    ASSERT_FALSE(timeout);
}

INSTANTIATE_TEST_SUITE_P(
    ,
    BlarggsTestRomsTest,
    testing::Values(
        // cpu_instrs
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
        "gb-test-roms/cpu_instrs/individual/11-op a,(hl).gb",
        // instr_timing
        "gb-test-roms/instr_timing/instr_timing.gb"
    )
);

} // namespace