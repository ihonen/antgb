#include "displaywidget.hh"

#include <chrono>
#include <iostream>
#include <QGraphicsPixmapItem>
#include <QGridLayout>
#include <QPixmap>

namespace antgb
{
/*
static const uint32_t colors[4] =
{
    0xFFE0F8D0, // very light green
    0xFF88C070, // light green
    0xFF346856, // green
    0xFF081820, // dark green
};

static const uint32_t colors[4] =
    {
        0xFFABC418, // very light green
        0xFF9DB618, // light green
        0xFF3B7440, // green
        0xFF0C4714, // dark green
};
*/

static const uint32_t colors[4] = {
    0xFFCDDE75, // light green
    0xFF98AA4B, // very light green
    0xFF406A3E, // green
    0xFF142B13, // dark green
};

DisplayWidget::DisplayWidget(QWidget* parent) : QWidget(parent), iRenderer()
{
    qRegisterMetaType<antgb::framebuf_t>("framebuf_t");

    _image = new QImage(160 * 2, 144 * 2, QImage::Format_ARGB32);
    _image->fill(QColor(colors[1]));

    _item = new QGraphicsPixmapItem(QPixmap::fromImage(*_image));

    _scene = new QGraphicsScene(this);
    _scene->addItem(_item);

    _view = new QGraphicsView(_scene);

    _view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _view->resize(160 * 2, 144 * 2);

    resize(160 * 2 + 2 * _view->frameWidth(), 144 * 2 + 2 * _view->frameWidth());

    connect(this, &DisplayWidget::frame_ready, this, &DisplayWidget::on_frame_ready);

    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(_view);
}

void DisplayWidget::on_frame_ready(framebuf_t framebuf)
{
    for (size_t y = 0; y < 144; ++y)
    {
        for (size_t x = 0; x < 160; ++x)
        {
            set_pixel(x, y, colors[framebuf[y][x]]);
        }
    }

    _item->setPixmap(QPixmap::fromImage(*_image));
}

void DisplayWidget::set_pixel(int x, int y, uint32_t color)
{
    for (auto x_offset = 0; x_offset < 2; ++x_offset)
    {
        for (auto y_offset = 0; y_offset < 2; ++y_offset)
        {
            _image->setPixelColor(x * 2 + x_offset, y * 2 + y_offset, QColor(color));
        }
    }
}

void DisplayWidget::do_render(antgb::framebuf_t framebuf)
{
    emit(frame_ready(framebuf));
}

} // namespace antgb
