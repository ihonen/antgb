#pragma once

#include "types.hh"
#include <string>

class Cartridge
{
public:
    static constexpr memaddr_t NINTENDO_LOGO_LOW = 0x0103;
    static constexpr memaddr_t NINTENDO_LOGO_HIGH = 0x0133;
    static constexpr size_t NINTENDO_LOGO_SIZE =
            NINTENDO_LOGO_HIGH - NINTENDO_LOGO_LOW + 1;

    static constexpr memaddr_t TITLE_LOW = 0x0134;
    static constexpr memaddr_t TITLE_HIGH = 0x0143;

    static constexpr memaddr_t MANUFACTURER_CODE_LOW = 0x013F;
    static constexpr memaddr_t MANUFACTURER_CODE_HIGH = 0x0142;

    static constexpr memaddr_t CGB_FLAG_ADDRESS = 0x0143;

    static constexpr memaddr_t NEW_LICENSEE_CODE_LOW = 0x0144;
    static constexpr memaddr_t NEW_LICENSEE_CODE_HIGH = 0x0145;

    static constexpr memaddr_t SGB_FLAG_ADDRESS = 0x0146;

    static constexpr memaddr_t CART_TYPE_ADDRESS = 0x0147;

    static constexpr memaddr_t ROM_SIZE_ADDRESS = 0x0148;

    static constexpr memaddr_t RAM_SIZE_ADDRESS = 0x0149;

    static constexpr memaddr_t DEST_CODE_ADDRESS = 0x014A;

    static constexpr memaddr_t OLD_LICENSEE_CODE_ADDRESS = 0x014B;

    static constexpr memaddr_t VERSION_NUMBER_ADDRESS = 0x014C;

    static constexpr memaddr_t HEADER_CHECKSUM_ADDRESS = 0x014D;

    static constexpr memaddr_t GLOBAL_CHECKSUM_LOW = 0x014E;
    static constexpr memaddr_t GLOBAL_CHECSUM_HIGH = 0x014F;

    // Note: only without bank switching.
    static constexpr memaddr_t MIN_ADDRESS = 0x0000;
    static constexpr memaddr_t MAX_ADDRESS = 0x7FFF;
    static constexpr memaddr_t SIZE = 0x8000;

    uint8_t data[SIZE];
    size_t size = SIZE;

    enum CgbSupport : uint8_t
    {
        CgbSupported = 0x80,
        CgbOnly = 0xC0
    };

    enum SgbFeatures : uint8_t
    {
        NoSgbFeatures = 0x00,
        HasSgbFeatures = 0x03
    };

    enum DestinationCode : uint8_t
    {
        Japanese = 0x00,
        NonJapanese = 0x01
    };

    enum CartridgeType : uint8_t
    {
        RomOnly = 0x00,
        Mbc1 = 0x01,
        Mbc1_Ram = 0x02,
        Mbc1_Ram_Battery = 0x03,
        Mbc2 = 0x05,
        Mbc2_Battery = 0x06,
        Rom_Ram = 0x08,
        Rom_Ram_Battery = 0x09,
        Mmm01 = 0x0B,
        Mmm01_Ram = 0x0C,
        Mmm01_Ram_Battery = 0x0D,
        Mbc3_Timer_Battery = 0x0F,
        Mbc3_Timer_Ram_Battery = 0x10,
        Mbc3 = 0x11,
        Mbc3_Ram = 0x12,
        Mbc3_Ram_Battery = 0x13,
        Mbc5 = 0x19,
        Mbc5_Ram = 0x1A,
        Mbc5_Ram_battery = 0x1B,
        Mbc5_Rumble = 0x1C,
        Mbc5_Rumble_Ram = 0x1D,
        Mbc5_Rumble_Ram_Battery = 0x1E,
        Mbc6 = 0x20,
        Mbc7_Sensor_Rumble_Ram_Battery = 0x22,
        PocketCamera = 0xFC,
        BandaiTama5 = 0xFD,
        Huc3 = 0xFE,
        HuC1_Ram_Battery = 0xFF
    };

    enum class RomSize : uint8_t
    {
        KB32 = 0x00,
        KB64 = 0x01,
        KB128 = 0x02,
        KB256 = 0x03,
        KB512 = 0x04,
        MB1 = 0x05,
        MB2 = 0x06,
        MB4 = 0x07,
        MB8 = 0x08,
        MB1_1 = 0x52,
        MB1_2 = 0x53,
        MB1_5 = 0x54
    };

    enum class RamSize : uint8_t
    {
        None = 0x00,
        KB2 = 0x01,
        KB8 = 0x02,
        KB32 = 0x03,
        KB128 = 0x04,
        KB64 = 0x05
    };

    Cartridge();
    bool is_nintendo_logo_valid();
    std::string title();
    bool has_manufacturer_code();
    uint32_t manufacturer_code();
    bool has_cgb_flag();
    uint8_t cgb_flag();
    bool has_new_licensee();
    uint16_t new_licensee_code();
    uint8_t sgb_flag();
    uint8_t cart_type();
    uint8_t rom_size();
    uint8_t ram_size();
    uint8_t destination_code();
    bool has_old_licensee();
    uint8_t old_licensee_code();
    uint8_t version_number();
    bool header_matches_checksum();
    uint8_t compute_header_checksum();
    uint8_t header_checksum();
    uint16_t compute_global_checksum();
    uint16_t global_checksum();
};
