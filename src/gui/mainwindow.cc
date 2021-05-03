#include "mainwindow.hh"

#include "../core/cartridge.hh"
#include "displaywidget.hh"

#include <QKeyEvent>
#include <QThread>

namespace antgb
{
MainWindow::MainWindow()
{
    _displaywidget = new DisplayWidget(this);
    _emulator      = new Emulator(_displaywidget);

    _keep_emulating   = true;
    _emulation_thread = QThread::create([&]() { emulate(); });
    _emulation_thread->start();
    _emulation_thread->setPriority(QThread::TimeCriticalPriority);

    setCentralWidget(_displaywidget);
}

MainWindow::~MainWindow()
{
    _keep_emulating = false;
    _emulation_thread->wait();
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
    case Qt::Key_A: _emulator->button_pressed(JoypadLeft); break;
    case Qt::Key_D: _emulator->button_pressed(JoypadRight); break;
    case Qt::Key_W: _emulator->button_pressed(JoypadUp); break;
    case Qt::Key_S: _emulator->button_pressed(JoypadDown); break;
    case Qt::Key_Return: _emulator->button_pressed(JoypadA); break;
    case Qt::Key_Backspace: _emulator->button_pressed(JoypadB); break;
    case Qt::Key_Comma: _emulator->button_pressed(JoypadSelect); break;
    case Qt::Key_Period: _emulator->button_pressed(JoypadStart); break;
    default: break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    switch (event->key())
    {
    case Qt::Key_A: _emulator->button_released(JoypadLeft); break;
    case Qt::Key_D: _emulator->button_released(JoypadRight); break;
    case Qt::Key_W: _emulator->button_released(JoypadUp); break;
    case Qt::Key_S: _emulator->button_released(JoypadDown); break;
    case Qt::Key_Return: _emulator->button_released(JoypadA); break;
    case Qt::Key_Backspace: _emulator->button_released(JoypadB); break;
    case Qt::Key_Comma: _emulator->button_released(JoypadSelect); break;
    case Qt::Key_Period: _emulator->button_released(JoypadStart); break;
    default: break;
    }
}

void MainWindow::emulate()
{
    int64_t cycles = 0;
    while (_keep_emulating)
    {
        if (cycles >= 70000)
        {
            cycles = 0;
            std::this_thread::sleep_for(std::chrono::milliseconds(17));
        }
        cycles += _emulator->execute_next();
    }
}

} // namespace antgb
