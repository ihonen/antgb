#pragma once

#include "debugger/debugcore.hh"
#include "button.hh"
#include <QGridLayout>
#include <QMetaType>
#include <QObject>
#include <QWidget>

Q_DECLARE_METATYPE(JoypadButton)

class ButtonInputWidget : public QFrame
{
    Q_OBJECT
public slots:
    void button_hover_enter();
    void button_hover_exit();
    void button_clicked();
public:
    DebugCore* debugger = nullptr;

    Button* start_button = nullptr;
    Button* select_button = nullptr;
    Button* a_button = nullptr;
    Button* b_button = nullptr;
    Button* up_button = nullptr;
    Button* down_button = nullptr;
    Button* left_button = nullptr;
    Button* right_button = nullptr;
    Button* center_button = nullptr;

    QMap<JoypadButton, Button*> buttons;

    QGridLayout* layout = nullptr;

    ButtonInputWidget(DebugCore* debugger, QWidget* parent = nullptr);
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;

    void joypad_press_callback(JoypadButton button);
    void joypad_release_callback(JoypadButton button);

signals:
    void joypad_pressed(JoypadButton);
    void joypad_released(JoypadButton);
private slots:
    void on_joypad_pressed(JoypadButton button);
    void on_joypad_released(JoypadButton button);
};
