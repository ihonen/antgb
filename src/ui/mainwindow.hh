#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "../mach/machine.hh"
#include "display.hh"

#include <QMainWindow>
#include <QGraphicsView>
#include <QBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QMenuBar>
#include <QString>
#include <QThread>

#include <thread>

#include <iostream>
using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void load_rom_act();

protected:
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void keyReleaseEvent(QKeyEvent* event) override;

public:

    explicit MainWindow(Machine& machine, QWidget* parent = nullptr);
    virtual ~MainWindow() override;

    void init_menubar();
    void init_signals();
    static void load_rom(QString& filepath, uint8_t* memory);
    void start_emulation();
    void stop_emulation();


    QGraphicsView* display_view_;
    QHBoxLayout* main_layout_;
    QLabel* debug_window_;
    QMenuBar* menubar_;

    QMenu* file_menu_;
    QAction* load_rom_act_;

    QMenu* options_menu_;

    Display* display_;

    Machine& machine;

    std::thread* emulation_thread;

    QThread* emulation_qthread;

    bool is_emulation_on;

    int tick_interval;
};

#endif // MAINWINDOW_HH
