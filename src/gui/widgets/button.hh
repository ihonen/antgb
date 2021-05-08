#pragma once

#include "gui/util/keybindings.hh"
#include <QLabel>

class Button : public QLabel
{
    Q_OBJECT
signals:
    void button_pressed(JoypadButton button_id);
public:
    JoypadButton id;

    Button(JoypadButton id, QWidget* parent = nullptr);
    void set_pixmap(const char* img_url);
    void set_default_pixmap();
    void set_hover_pixmap();
    void set_press_pixmap();

protected:
    virtual void enterEvent(QEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void keyReleaseEvent(QKeyEvent* event) override;

    // To give it access to the mouse events.
    friend class ButtonInputWidget;
};
