#include "displayscene.hh"

#include "emulator/emulator.hh"
#include <QGraphicsRectItem>

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

static const std::array<uint32_t, 4> colors =
{{
    0xFFCDDE75, // light green
    0xFF98AA4B, // very light green
    0xFF406A3E, // green
    0xFF142B13, // dark green
}};

#include <chrono>

DisplayScene::DisplayScene(DebugCore* debugger, QObject* parent) :
    QGraphicsScene(parent),
    iFrontend(),
    debugger(debugger)
{
    qRegisterMetaType<iFrontend::Pixels>("iFrontend::Pixels");

    image = QImage(160 * 2, 144 * 2, QImage::Format_ARGB32);
    image.fill(QColor(colors[3]));
    item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    addItem(item);

    connect(this, &DisplayScene::schedule_render_on_screen,
            this, &DisplayScene::render_on_screen);
}

void DisplayScene::set_pixel(QImage& image, size_t x, size_t y, uint32_t color)
{
    for (auto x_offset = 0; x_offset < 2; ++x_offset)
    {
        for (auto y_offset = 0; y_offset < 2; ++y_offset)
        {
            image.setPixelColor(int(x) * 2 + x_offset,
                                int(y) * 2 + y_offset,
                                QColor(color));
        }
    }
}

void DisplayScene::render_callback(const Pixels& pixels)
{
    emit schedule_render_on_screen(pixels);
}

void DisplayScene::serial_callback(const uint8_t byte)
{
    std::cerr << byte;
}

void DisplayScene::render_on_screen(const Pixels& pixels)
{
    for (size_t x = 0; x < SCREEN_WIDTH; ++x)
    {
        for (size_t y = 0; y < iFrontend::SCREEN_HEIGHT; ++y)
        {
            set_pixel(image, x, y, colors.at(pixels.at(x).at(y)));
       }
    }

    item->setPixmap(QPixmap::fromImage(image));
}
