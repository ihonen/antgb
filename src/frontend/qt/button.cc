#include "button.hh"

#include <QIcon>

#include "pixmaps.hh"

Button::Button(Joypad::Button id, QWidget* parent) :
    QLabel(parent),
    id(id)
{
    setFixedSize(40, 40);
    set_default_pixmap();
}

void Button::set_pixmap(const char* img_url)
{
    setPixmap(QPixmap::fromImage(QImage(img_url)));
}

void Button::set_default_pixmap()
{
    switch (id)
    {
        case Joypad::Button::Up:
            setPixmap(Pixmaps::JOYPAD_UP->scaled(width(), height()));
            break;
        case Joypad::Button::Down:
            setPixmap(Pixmaps::JOYPAD_DOWN->scaled(width(), height()));
            break;
        case Joypad::Button::Left:
            setPixmap(Pixmaps::JOYPAD_LEFT->scaled(width(), height()));
            break;
        case Joypad::Button::Right:
            setPixmap(Pixmaps::JOYPAD_RIGHT->scaled(width(), height()));
            break;
        case Joypad::Button::Start:
            setPixmap(Pixmaps::JOYPAD_START->scaled(width(), height()));
            break;
        case Joypad::Button::Select:
            setPixmap(Pixmaps::JOYPAD_SELECT->scaled(width(), height()));
            break;
        case Joypad::Button::A:
            setPixmap(Pixmaps::JOYPAD_A->scaled(width(), height()));
            break;
        case Joypad::Button::B:
            setPixmap(Pixmaps::JOYPAD_B->scaled(width(), height()));
            break;
        default:
            setPixmap(Pixmaps::JOYPAD_CENTER->scaled(width(), height()));
            break;
    }
}

void Button::set_hover_pixmap()
{
    switch (id)
    {
        case Joypad::Button::Up:
            setPixmap(Pixmaps::JOYPAD_UP_HOVER->scaled(width(), height()));
            break;
        case Joypad::Button::Down:
            setPixmap(Pixmaps::JOYPAD_DOWN_HOVER->scaled(width(), height()));
            break;
        case Joypad::Button::Left:
            setPixmap(Pixmaps::JOYPAD_LEFT_HOVER->scaled(width(), height()));
            break;
        case Joypad::Button::Right:
            setPixmap(Pixmaps::JOYPAD_RIGHT_HOVER->scaled(width(), height()));
            break;
        case Joypad::Button::Start:
            setPixmap(Pixmaps::JOYPAD_START_HOVER->scaled(width(), height()));
            break;
        case Joypad::Button::Select:
            setPixmap(Pixmaps::JOYPAD_SELECT_HOVER->scaled(width(), height()));
            break;
        case Joypad::Button::A:
            setPixmap(Pixmaps::JOYPAD_A_HOVER->scaled(width(), height()));
            break;
        case Joypad::Button::B:
            setPixmap(Pixmaps::JOYPAD_B_HOVER->scaled(width(), height()));
            break;
        default:
            setPixmap(Pixmaps::JOYPAD_CENTER->scaled(width(), height()));
            break;
    }
}

void Button::set_press_pixmap()
{
    switch (id)
    {
        case Joypad::Button::Up:
            setPixmap(Pixmaps::JOYPAD_UP_PRESS->scaled(width(), height()));
            break;
        case Joypad::Button::Down:
            setPixmap(Pixmaps::JOYPAD_DOWN_PRESS->scaled(width(), height()));
            break;
        case Joypad::Button::Left:
            setPixmap(Pixmaps::JOYPAD_LEFT_PRESS->scaled(width(), height()));
            break;
        case Joypad::Button::Right:
            setPixmap(Pixmaps::JOYPAD_RIGHT_PRESS->scaled(width(), height()));
            break;
        case Joypad::Button::Start:
            setPixmap(Pixmaps::JOYPAD_START_PRESS->scaled(width(), height()));
            break;
        case Joypad::Button::Select:
            setPixmap(Pixmaps::JOYPAD_SELECT_PRESS->scaled(width(), height()));
            break;
        case Joypad::Button::A:
            setPixmap(Pixmaps::JOYPAD_A_PRESS->scaled(width(), height()));
            break;
        case Joypad::Button::B:
            setPixmap(Pixmaps::JOYPAD_B_PRESS->scaled(width(), height()));
            break;
        default:
            setPixmap(Pixmaps::JOYPAD_CENTER->scaled(width(), height()));
            break;
    }
}

void Button::enterEvent(QEvent* event)
{
    set_hover_pixmap();
    QLabel::enterEvent(event);
}

void Button::leaveEvent(QEvent* event)
{
    set_default_pixmap();
    QLabel::leaveEvent(event);
}

void Button::mousePressEvent(QMouseEvent* event)
{
    set_press_pixmap();
    QLabel::mousePressEvent(event);
}

void Button::mouseReleaseEvent(QMouseEvent* event)
{
    set_hover_pixmap();
    QLabel::mouseReleaseEvent(event);
}

void Button::keyPressEvent(QKeyEvent* event)
{
    set_press_pixmap();
    QLabel::keyPressEvent(event);
}

void Button::keyReleaseEvent(QKeyEvent* event)
{
    set_default_pixmap();
    QLabel::keyReleaseEvent(event);
}
