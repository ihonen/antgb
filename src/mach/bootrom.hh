#ifndef BOOTROM_HH
#define BOOTROM_HH

#include "../util/fileio.hh"

struct BootROM
{
    BootROM()
    {
        QString fpath = ":/rom/boot.bin";
        //QString fpath = ":/rom/extended_boot.bin";
        load_rom(fpath, data);
    }

    uint8_t data[0x0133];
    //uint8_t data[256];
};

#endif // BOOTROM_HH
