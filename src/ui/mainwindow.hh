#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "../mach/machine.hh"

#include <QMainWindow>
#include <QGraphicsView>
#include <QBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QMenuBar>
#include <QString>

#include <thread>

#include <iostream>
using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void load_rom_act();
public:
    explicit MainWindow(Machine& machine, QWidget* parent = nullptr);
    virtual ~MainWindow() override;
protected:
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void keyReleaseEvent(QKeyEvent* event) override;
private:
    void init_menubar();
    void init_signals();
    void load_rom(QString& filepath);
    void start_emulation();
    void stop_emulation();


    QGraphicsView* display_view_;
    QHBoxLayout* main_layout_;
    QLabel* debug_window_;
    QMenuBar* menubar_;

    QMenu* file_menu_;
    QAction* load_rom_act_;

    QMenu* options_menu_;

    Machine& machine;

    std::thread* emulation_thread;

    bool is_emulation_on;

    int tick_interval;
};

#endif // MAINWINDOW_HH
