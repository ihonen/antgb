#ifndef DISPLAY_HH
#define DISPLAY_HH

#include "mach/ppu.hh"
#include <QGraphicsScene>

class Display : public QGraphicsScene
{
public:
    Display(PPU& ppu, QObject* parent = nullptr);
    void set_pixel(int x, int y, uint32_t color);

    PPU& ppu;
    QImage image;
    QGraphicsPixmapItem* item;
};

#endif // DISPLAY_HH
