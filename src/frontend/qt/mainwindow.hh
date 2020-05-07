#pragma once

#include "../../core/emulator.hh"
#include "displaywidget.hh"

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
    explicit MainWindow(Emulator* machine, QWidget* parent = nullptr);
    virtual ~MainWindow() override;

    void init_menubar();
    void init_signals();
    void start_emulation();
    void stop_emulation();

    QGraphicsView*  display_view;
    QMenuBar*       menubar;
    QMenu*          file_menu;
    QAction*        load_rom_action;
    QMenu*          options_menu;
    DisplayWidget*        display;
    Emulator*        machine;
    QThread*        emulation_qthread;
    bool            is_emulation_on;
};
