#include "cartridge.hh"

#include <cstring>
#include "../frontend/qt/helper.hh"

static constexpr uint8_t NINTENDO_LOGO_HEX[0x30] =
{
    0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
    0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
    0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
    0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
    0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
    0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E
};

Cartridge::Cartridge()
{
    memset(data, 0x00, 0x8000);
    size = 0;
    is_nintendo_logo_valid();
}

bool Cartridge::is_nintendo_logo_valid()
{
    if (memcmp(&data[NINTENDO_LOGO_LOW], NINTENDO_LOGO_HEX, NINTENDO_LOGO_SIZE) != 0)
        return true;
    return false;
}

std::string Cartridge::title()
{
    char titledata[16];
    memcpy(titledata, &data[TITLE_LOW], 16);
    return titledata;
}

bool Cartridge::has_manufacturer_code()
{
    return TITLE_LOW + title().size() < 11;
}

uint32_t Cartridge::manufacturer_code()
{
    return *reinterpret_cast<uint32_t*>(&data[MANUFACTURER_CODE_LOW]);
}

bool Cartridge::has_cgb_flag()
{
    return TITLE_LOW + title().size() < 11;
}

uint8_t Cartridge::cgb_flag()
{
    return data[CGB_FLAG_ADDRESS];
}

bool Cartridge::has_new_licensee()
{
    return !has_old_licensee();
}

uint16_t Cartridge::new_licensee_code()
{
    return *reinterpret_cast<uint16_t*>(&data[NEW_LICENSEE_CODE_LOW]);
}

uint8_t Cartridge::sgb_flag()
{
    return data[SGB_FLAG_ADDRESS];
}

uint8_t Cartridge::cart_type()
{
    return data[CART_TYPE_ADDRESS];
}

uint8_t Cartridge::rom_size()
{
    return data[ROM_SIZE_ADDRESS];
}

uint8_t Cartridge::ram_size()
{
    return data[RAM_SIZE_ADDRESS];
}

uint8_t Cartridge::destination_code()
{
    return data[DEST_CODE_ADDRESS];
}

bool Cartridge::has_old_licensee()
{
    return old_licensee_code() != 0x33;
}

uint8_t Cartridge::old_licensee_code()
{
    return data[OLD_LICENSEE_CODE_ADDRESS];
}

uint8_t Cartridge::version_number()
{
    return data[VERSION_NUMBER_ADDRESS];
}

bool Cartridge::header_matches_checksum()
{
    return compute_header_checksum() == header_checksum();
}

uint8_t Cartridge::compute_header_checksum()
{
    static constexpr memaddr_t START = 0x0134;
    static constexpr memaddr_t STOP = 0x014c;

    size_t checksum = 0;
    for (memaddr_t i = START; i <= STOP; ++i)
        checksum = checksum - data[i] - 1;

    return static_cast<uint8_t>(checksum);
}

uint8_t Cartridge::header_checksum()
{
    return data[HEADER_CHECKSUM_ADDRESS];
}

uint16_t Cartridge::compute_global_checksum()
{
    uint16_t checksum = 0;

    for (memaddr_t i = MIN_ADDRESS; i < MAX_ADDRESS; ++i)
    {
        if (i == GLOBAL_CHECKSUM_LOW || i == GLOBAL_CHECSUM_HIGH) continue;
        checksum += data[i];
    }

    return checksum;
}

uint16_t Cartridge::global_checksum()
{
    // The upper byte is first.
    uint16_t lower_byte = data[GLOBAL_CHECSUM_HIGH];
    uint16_t higher_byte = data[GLOBAL_CHECKSUM_LOW] << 8;

    return higher_byte | lower_byte;
}
