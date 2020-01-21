#ifndef RENDERER_HH
#define RENDERER_HH

#include <array>
#include <cstdint>
#include <iostream>
#include <QObject>

using namespace std;

class Renderer : public QObject
{
    Q_OBJECT
public:
    Renderer(QObject* parent = nullptr);
    virtual ~Renderer() override = default;
    void render_frame();

    uint32_t* get_frame_buffer();
signals:
    void frame_ready();
private:
    uint32_t frame_buffer[144][160];
};

#endif // RENDERER_HH
