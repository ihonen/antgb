#include "display.hh"

#include <QGraphicsRectItem>

static const uint32_t BLACK = 0xFF000000;
static const uint32_t DARK_GRAY = 0xFF555555;
static const uint32_t LIGHT_GRAY = 0xFFAAAAAA;
static const uint32_t WHITE = 0xFFFFFFFF;

#include <chrono>

void Display::on_frame_ready()
{
    auto frame_buffer = renderer.get_frame_buffer();

    for (size_t y = 0; y < 144; ++y)
    {
        for (size_t x = 0; x < 160; ++x)
        {
            set_pixel(x, y, frame_buffer[y * 160 + x]);
        }
    }

    item->setPixmap(QPixmap::fromImage(image));
}

Display::Display(PPU& ppu_, Renderer& renderer_, QObject* parent) :
    QGraphicsScene(parent),
    renderer(renderer_),
    ppu(ppu_)
{
    image = QImage(160 * 4, 144 * 4, QImage::Format_ARGB32);
    image.fill(QColor(BLACK));
    item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    addItem(item);
}

void Display::set_pixel(int x, int y, uint32_t color)
{
    for (auto x_offset = 0; x_offset < 4; ++x_offset)
    {
        for (auto y_offset = 0; y_offset < 4; ++y_offset)
        {
            image.setPixelColor(x * 4 + x_offset,
                                y * 4 + y_offset,
                                QColor(color));
        }
    }
}
