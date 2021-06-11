#pragma once

#include "debugger/ifrontend.hh"
#include "debugger/debugcore.hh"
#include <QGraphicsScene>

Q_DECLARE_METATYPE(iFrontend::Pixels);

class DisplayScene final
    : public QGraphicsScene
    , public iFrontend
{
    Q_OBJECT

public:    
    DisplayScene(DebugCore* debugger, QObject* parent = nullptr);
    virtual ~DisplayScene() override = default;
    void set_pixel(QImage& image, size_t x, size_t y, uint32_t color);

    virtual void render_callback(const iFrontend::Pixels& pixels) override;
    // TODO: This really, really, REALLY doesn't belong here.
    virtual void serial_callback(const uint8_t byte) override;

    static constexpr uint16_t RES_X = 160;
    static constexpr uint16_t RES_Y = 144;
    static constexpr uint16_t RES_UPSCALE_FACTOR = 4;
    static constexpr uint16_t REAL_RES_X = RES_X * RES_UPSCALE_FACTOR;
    static constexpr uint16_t REAL_RES_Y = RES_Y * RES_UPSCALE_FACTOR;

    DebugCore* debugger;
    QImage image;
    QGraphicsPixmapItem* item;
signals:
    void schedule_render_on_screen(const iFrontend::Pixels& pixels);

protected slots:
    void render_on_screen(const iFrontend::Pixels& pixels);
};
