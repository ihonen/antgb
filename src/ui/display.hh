#ifndef DISPLAY_HH
#define DISPLAY_HH

#include "../gfx/renderer.hh"
#include <QGraphicsScene>

class Display : public QGraphicsScene
{
public slots:
    void on_frame_ready();
public:    
    Display(Memory* memory, Renderer* renderer, QObject* parent = nullptr);
    void set_pixel(int x, int y, uint32_t color);

    static constexpr uint16_t RES_X = 160;
    static constexpr uint16_t RES_Y = 144;
    static constexpr uint16_t RES_UPSCALE_FACTOR = 4;
    static constexpr uint16_t REAL_RES_X = RES_X * RES_UPSCALE_FACTOR;
    static constexpr uint16_t REAL_RES_Y = RES_Y * RES_UPSCALE_FACTOR;

    Renderer* renderer;
    Memory* mem;
    QImage image;
    QGraphicsPixmapItem* item;
};

#endif // DISPLAY_HH
