#ifndef DISPLAY_HH
#define DISPLAY_HH

#include "../gfx/renderer.hh"
#include "mach/ppu.hh"
#include <QGraphicsScene>

class Display : public QGraphicsScene
{
public slots:
    void on_frame_ready();
public:
    Display(PPU& ppu, Renderer* renderer, QObject* parent = nullptr);
    void set_pixel(int x, int y, uint32_t color);

    Renderer* renderer;
    PPU& ppu;
    QImage image;
    QGraphicsPixmapItem* item;
};

#endif // DISPLAY_HH
