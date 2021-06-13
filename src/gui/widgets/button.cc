#include "button.hh"

#include <QIcon>

#include "gui/util/pixmaps.hh"

Button::Button(JoypadButton id, QWidget* parent) :
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
        case JoypadUp:
            setPixmap(Pixmaps::JOYPAD_UP->scaled(width(), height()));
            break;
        case JoypadDown:
            setPixmap(Pixmaps::JOYPAD_DOWN->scaled(width(), height()));
            break;
        case JoypadLeft:
            setPixmap(Pixmaps::JOYPAD_LEFT->scaled(width(), height()));
            break;
        case JoypadRight:
            setPixmap(Pixmaps::JOYPAD_RIGHT->scaled(width(), height()));
            break;
        case JoypadStart:
            setPixmap(Pixmaps::JOYPAD_START->scaled(width(), height()));
            break;
        case JoypadSelect:
            setPixmap(Pixmaps::JOYPAD_SELECT->scaled(width(), height()));
            break;
        case JoypadA:
            setPixmap(Pixmaps::JOYPAD_A->scaled(width(), height()));
            break;
        case JoypadB:
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
        case JoypadUp:
            setPixmap(Pixmaps::JOYPAD_UP_HOVER->scaled(width(), height()));
            break;
        case JoypadDown:
            setPixmap(Pixmaps::JOYPAD_DOWN_HOVER->scaled(width(), height()));
            break;
        case JoypadLeft:
            setPixmap(Pixmaps::JOYPAD_LEFT_HOVER->scaled(width(), height()));
            break;
        case JoypadRight:
            setPixmap(Pixmaps::JOYPAD_RIGHT_HOVER->scaled(width(), height()));
            break;
        case JoypadStart:
            setPixmap(Pixmaps::JOYPAD_START_HOVER->scaled(width(), height()));
            break;
        case JoypadSelect:
            setPixmap(Pixmaps::JOYPAD_SELECT_HOVER->scaled(width(), height()));
            break;
        case JoypadA:
            setPixmap(Pixmaps::JOYPAD_A_HOVER->scaled(width(), height()));
            break;
        case JoypadB:
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
        case JoypadUp:
            setPixmap(Pixmaps::JOYPAD_UP_PRESS->scaled(width(), height()));
            break;
        case JoypadDown:
            setPixmap(Pixmaps::JOYPAD_DOWN_PRESS->scaled(width(), height()));
            break;
        case JoypadLeft:
            setPixmap(Pixmaps::JOYPAD_LEFT_PRESS->scaled(width(), height()));
            break;
        case JoypadRight:
            setPixmap(Pixmaps::JOYPAD_RIGHT_PRESS->scaled(width(), height()));
            break;
        case JoypadStart:
            setPixmap(Pixmaps::JOYPAD_START_PRESS->scaled(width(), height()));
            break;
        case JoypadSelect:
            setPixmap(Pixmaps::JOYPAD_SELECT_PRESS->scaled(width(), height()));
            break;
        case JoypadA:
            setPixmap(Pixmaps::JOYPAD_A_PRESS->scaled(width(), height()));
            break;
        case JoypadB:
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
    if (event != nullptr)
    {
        event->accept();
    }
}

void Button::mouseReleaseEvent(QMouseEvent* event)
{
    set_hover_pixmap();
    if (event != nullptr)
    {
        event->accept();
    }
}

void Button::keyPressEvent(QKeyEvent* event)
{
    set_press_pixmap();
    if (event != nullptr)
    {
        event->accept();
    }
}

void Button::keyReleaseEvent(QKeyEvent* event)
{
    set_default_pixmap();
    if (event != nullptr)
    {
        event->accept();
    }
}
