#pragma once

#include "../core/irenderer.hh"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QWidget>

namespace antgb
{
class DisplayWidget : public QWidget, public iRenderer
{
    Q_OBJECT
signals:
    void frame_ready(framebuf_t framebuf);
public slots:
    void on_frame_ready(framebuf_t framebuf);

public:
    DisplayWidget(QWidget* parent = nullptr);
    virtual ~DisplayWidget() override = default;
    virtual void do_render(framebuf_t framebuf) override;

private:
    void set_pixel(int x, int y, uint32_t color);

    QGraphicsScene*      _scene;
    QGraphicsView*       _view;
    QGraphicsPixmapItem* _item;
    QImage*              _image;
};

} // namespace antgb

Q_DECLARE_METATYPE(antgb::framebuf_t)
