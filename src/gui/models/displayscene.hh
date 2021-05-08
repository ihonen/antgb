#pragma once

#include "debugger/debugcore.hh"
#include "emulator/renderer.hh"
#include <QGraphicsScene>

class DisplayScene : public QGraphicsScene
{
public slots:
    void on_frame_ready();
public:    
    DisplayScene(DebugCore* debugger, QObject* parent = nullptr);
    void set_pixel(int x, int y, uint32_t color);

    static constexpr uint16_t RES_X = 160;
    static constexpr uint16_t RES_Y = 144;
    static constexpr uint16_t RES_UPSCALE_FACTOR = 4;
    static constexpr uint16_t REAL_RES_X = RES_X * RES_UPSCALE_FACTOR;
    static constexpr uint16_t REAL_RES_Y = RES_Y * RES_UPSCALE_FACTOR;

    DebugCore* debugger;
    QImage image;
    QGraphicsPixmapItem* item;
};
