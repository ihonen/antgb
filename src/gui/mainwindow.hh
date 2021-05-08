#pragma once

#include "../emulator/emulator.hh"
#include "../debugger/debugcore.hh"
#include "displayscene.hh"
#include "emulatorwidget.hh"

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
    void debug_mode_act();
    void step_into_act();
    void run_act();
    void pause_act();
    void terminate_act();
protected:
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void keyReleaseEvent(QKeyEvent* event) override;

public:
    explicit MainWindow(Emulator* emulator, DebugCore* debugger, QWidget* parent = nullptr);
    virtual ~MainWindow() override;

    void init_actions();
    void init_menubar();
    void init_toolbar();
    void init_signals();
    void start_emulation();
    void stop_emulation();

    QMenuBar*       menubar;
    QMenu*          file_menu;
    QMenu*          options_menu;
    QMenu*          debug_menu;
    Emulator*       emu;
    DebugCore*      debugger;
    EmulatorWidget* emuwidget;
    QToolBar*       toolbar;
    QAction*        debug_mode_action;
    QAction*        load_rom_action;
    QAction*        settings_action;
    QAction*        accelerate_action;
    QAction*        decelerate_action;
    QAction*        save_game_action;
    QAction*        memory_dump_action;
    QAction*        run_action;
    QAction*        terminate_action;
    QAction*        step_over_action;
    QAction*        step_into_action;
    QAction*        step_out_action;
    QAction*        restart_action;
};
