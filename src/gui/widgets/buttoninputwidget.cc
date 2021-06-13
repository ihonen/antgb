#include "buttoninputwidget.hh"

#include "gui/util/keybindings.hh"

void ButtonInputWidget::button_hover_enter()
{

}

void ButtonInputWidget::button_hover_exit()
{

}

void ButtonInputWidget::button_clicked()
{

}

ButtonInputWidget::ButtonInputWidget(DebugCore* debugger, QWidget* parent) :
    QFrame(parent),
    debugger(debugger)
{
    qRegisterMetaType<JoypadButton>("JoypadButton");

    setStyleSheet("background-color: #232629");

    up_button = new Button(JoypadUp, this);
    down_button = new Button(JoypadDown, this);
    left_button = new Button(JoypadLeft, this);
    right_button = new Button(JoypadRight, this);
    center_button = new Button(JoypadNone, this);
    select_button = new Button(JoypadSelect, this);
    start_button = new Button(JoypadStart, this);
    a_button = new Button(JoypadA, this);
    b_button = new Button(JoypadB, this);

    buttons =
    {
        {JoypadUp, up_button},
        {JoypadDown, down_button},
        {JoypadLeft, left_button},
        {JoypadRight, right_button},
        {JoypadStart, start_button},
        {JoypadSelect, select_button},
        {JoypadA, a_button},
        {JoypadB, b_button}
    };

    layout = new QGridLayout(this);
    // Placeholder widgets that can be stretched.
    auto placeholderwidget1 = new QWidget(this);
    auto placeholderwidget2 = new QWidget(this);
    auto placeholderwidget3 = new QWidget(this);
    auto placeholderwidget4 = new QWidget(this);
    layout->addWidget(placeholderwidget1, 0, 0, 1, 5);
    layout->addWidget(placeholderwidget2, 4, 0, 1, 5);
    layout->addWidget(placeholderwidget3, 1, 0, 3, 1);
    layout->addWidget(placeholderwidget4, 1, 6, 3, 1);

    layout->addWidget(up_button, 1, 2);
    layout->addWidget(down_button, 3, 2);
    layout->addWidget(left_button, 2, 1);
    layout->addWidget(right_button, 2, 3);
    layout->addWidget(center_button, 2, 2);
    layout->addWidget(select_button, 3, 4);
    layout->addWidget(start_button, 3, 5);
    layout->addWidget(b_button, 2, 4);
    layout->addWidget(a_button, 2, 5);
    layout->setSpacing(0);
    layout->setMargin(0);

    connect(this, &ButtonInputWidget::joypad_pressed,
            this, &ButtonInputWidget::on_joypad_pressed);
    connect(this, &ButtonInputWidget::joypad_released,
            this, &ButtonInputWidget::on_joypad_released);
}

void ButtonInputWidget::keyPressEvent(QKeyEvent* event)
{
    auto button = buttons[JOYPAD_KEYMAP.at(event->key())];
    debugger->button_pressed(button->id);
    event->accept();
}

void ButtonInputWidget::keyReleaseEvent(QKeyEvent* event)
{
    auto button = buttons[JOYPAD_KEYMAP.at(event->key())];
    debugger->button_released(button->id);
    event->accept();
}

void ButtonInputWidget::joypad_press_callback(JoypadButton button)
{
    emit joypad_pressed(button);
}

void ButtonInputWidget::joypad_release_callback(JoypadButton button)
{
    emit joypad_released(button);
}

void ButtonInputWidget::on_joypad_pressed(JoypadButton button)
{
    Button* ui_button = nullptr;

    switch (button)
    {
        case JoypadA: ui_button = a_button; break;
        case JoypadB: ui_button = b_button; break;
        case JoypadStart: ui_button = start_button; break;
        case JoypadSelect: ui_button = select_button; break;
        case JoypadUp: ui_button = up_button; break;
        case JoypadDown: ui_button = down_button; break;
        case JoypadLeft: ui_button = left_button; break;
        case JoypadRight: ui_button = right_button; break;
        case JoypadNone: break;
    }
    if (ui_button != nullptr)
    {
        // TODO: Find a better solution.
        ui_button->keyPressEvent(nullptr);
    }
}

void ButtonInputWidget::on_joypad_released(JoypadButton button)
{
    Button* ui_button = nullptr;

    switch (button)
    {
        case JoypadA: ui_button = a_button; break;
        case JoypadB: ui_button = b_button; break;
        case JoypadStart: ui_button = start_button; break;
        case JoypadSelect: ui_button = select_button; break;
        case JoypadUp: ui_button = up_button; break;
        case JoypadDown: ui_button = down_button; break;
        case JoypadLeft: ui_button = left_button; break;
        case JoypadRight: ui_button = right_button; break;
        case JoypadNone: break;
    }
    if (ui_button != nullptr)
    {
        // TODO: Find a better solution.
        ui_button->keyReleaseEvent(nullptr);
    }
}
