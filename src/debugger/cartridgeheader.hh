#pragma once

#include "emulator/common/types.hh"
#include <string>

class CartridgeHeader
{
public:
    static constexpr addr_t NINTENDO_LOGO_LOW = 0x0103;
    static constexpr addr_t NINTENDO_LOGO_HIGH = 0x0133;
    static constexpr size_t NINTENDO_LOGO_SIZE =
            NINTENDO_LOGO_HIGH - NINTENDO_LOGO_LOW + 1;

    static constexpr addr_t TITLE_LOW = 0x0134;
    static constexpr addr_t TITLE_HIGH = 0x0143;

    static constexpr addr_t MANUFACTURER_CODE_LOW = 0x013F;
    static constexpr addr_t MANUFACTURER_CODE_HIGH = 0x0142;

    static constexpr addr_t CGB_FLAG_ADDRESS = 0x0143;

    static constexpr addr_t NEW_LICENSEE_CODE_LOW = 0x0144;
    static constexpr addr_t NEW_LICENSEE_CODE_HIGH = 0x0145;

    static constexpr addr_t SGB_FLAG_ADDRESS = 0x0146;

    static constexpr addr_t CART_TYPE_ADDRESS = 0x0147;

    static constexpr addr_t ROM_SIZE_ADDRESS = 0x0148;

    static constexpr addr_t RAM_SIZE_ADDRESS = 0x0149;

    static constexpr addr_t DEST_CODE_ADDRESS = 0x014A;

    static constexpr addr_t OLD_LICENSEE_CODE_ADDRESS = 0x014B;

    static constexpr addr_t VERSION_NUMBER_ADDRESS = 0x014C;

    static constexpr addr_t HEADER_CHECKSUM_ADDRESS = 0x014D;

    static constexpr addr_t GLOBAL_CHECKSUM_LOW = 0x014E;
    static constexpr addr_t GLOBAL_CHECSUM_HIGH = 0x014F;

    // Note: only without bank switching.
    static constexpr addr_t MIN_ADDRESS = 0x0000;
    static constexpr addr_t MAX_ADDRESS = 0x7FFF;
    static constexpr addr_t SIZE = 0x8000;

    uint8_t data[SIZE];
    size_t size = SIZE;

    enum CgbSupport : uint8_t
    {
        CartCgbSupported = 0x80,
        CartCgbOnly = 0xC0
    };

    enum SgbFeatures : uint8_t
    {
        CartNoSgbFeatures = 0x00,
        CartHasSgbFeatures = 0x03
    };

    enum DestinationCode : uint8_t
    {
        CartJapanese = 0x00,
        CartNonJapanese = 0x01
    };

    enum CartridgeType : uint8_t
    {
        CartRomOnly = 0x00,
        CartMbc1 = 0x01,
        CartMbc1_Ram = 0x02,
        CartMbc1_Ram_Battery = 0x03,
        CartMbc2 = 0x05,
        CartMbc2_Battery = 0x06,
        CartRom_Ram = 0x08,
        CartRom_Ram_Battery = 0x09,
        CartMmm01 = 0x0B,
        CartMmm01_Ram = 0x0C,
        CartMmm01_Ram_Battery = 0x0D,
        CartMbc3_Timer_Battery = 0x0F,
        CartMbc3_Timer_Ram_Battery = 0x10,
        CartMbc3 = 0x11,
        CartMbc3_Ram = 0x12,
        CartMbc3_Ram_Battery = 0x13,
        CartMbc5 = 0x19,
        CartMbc5_Ram = 0x1A,
        CartMbc5_Ram_battery = 0x1B,
        CartMbc5_Rumble = 0x1C,
        CartMbc5_Rumble_Ram = 0x1D,
        CartMbc5_Rumble_Ram_Battery = 0x1E,
        CartMbc6 = 0x20,
        CartMbc7_Sensor_Rumble_Ram_Battery = 0x22,
        CartPocketCamera = 0xFC,
        CartBandaiTama5 = 0xFD,
        CartHuc3 = 0xFE,
        CartHuC1_Ram_Battery = 0xFF
    };

    enum class RomSize : uint8_t
    {
        Cart32KB = 0x00,
        Cart64KB = 0x01,
        Cart128KB = 0x02,
        Cart256KB = 0x03,
        Cart512KB = 0x04,
        Cart1MB = 0x05,
        Cart2MB = 0x06,
        Cart4MB = 0x07,
        Cart8MB = 0x08,
        Cart1_1MB = 0x52,
        Cart1_2MB = 0x53,
        Cart1_5MB = 0x54
    };

    enum class RamSize : uint8_t
    {
        CartNone = 0x00,
        Cart2KB = 0x01,
        Cart8KB = 0x02,
        Cart32KB = 0x03,
        Cart128KB = 0x04,
        Cart64KB = 0x05
    };

    CartridgeHeader();
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
