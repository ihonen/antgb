#include "mainwindow.hh"

#include "../../core/cartridge.hh"
#include "../../core/fileio.hh"
#include "keybindings.hh"
#include "colors.hh"
#include "fonts.hh"
#include "pixmaps.hh"
#include <thread>
#include <QFileDialog>
#include <QToolBar>


MainWindow::MainWindow(Emulator* emulator, DebugCore* debugger, QWidget* parent) :
    QMainWindow(parent),
    emu(emulator),
    debugger(debugger)
{
    Colors::load_all();
    Fonts::load_all();
    Pixmaps::load_all();

    emuwidget = new EmulatorWidget(emu, debugger, this);
    setCentralWidget(emuwidget);

    init_actions();
    init_toolbar();
    init_menubar();
    init_signals();

    setWindowState(Qt::WindowMaximized);
    setWindowTitle("AntGB");
    setWindowIcon(QIcon(QPixmap::fromImage(QImage(":/img/ant_icon4.png"))));
}

MainWindow::~MainWindow()
{
    debugger->pause();
}

void MainWindow::init_actions()
{
    debug_mode_action = new QAction("Debug Mode", this);

    load_rom_action = new QAction("Load ROM", this);
    QIcon load_rom_icon;
    load_rom_icon.addPixmap(*Pixmaps::ROM);
    load_rom_icon.addPixmap(*Pixmaps::ROM_HOVER, QIcon::Active);
    load_rom_action->setIcon(load_rom_icon);

    settings_action = new QAction("Settings", this);
    QIcon settings_icon;
    settings_icon.addPixmap(*Pixmaps::GEAR);
    settings_icon.addPixmap(*Pixmaps::GEAR_HOVER, QIcon::Active);
    settings_action->setIcon(settings_icon);

    save_game_action = new QAction("Save game", this);
    QIcon save_game_icon;
    save_game_icon.addPixmap(*Pixmaps::SAVE_GAME);
    save_game_icon.addPixmap(*Pixmaps::SAVE_GAME_HOVER, QIcon::Active);
    save_game_action->setIcon(save_game_icon);

    accelerate_action = new QAction("Increase Emulation Speed", this);
    QIcon accelerate_icon;
    accelerate_icon.addPixmap(*Pixmaps::ACCELERATE);
    accelerate_icon.addPixmap(*Pixmaps::ACCELERATE_HOVER, QIcon::Active);
    accelerate_action->setIcon(accelerate_icon);

    decelerate_action = new QAction("Decrease Emulation Speed", this);
    QIcon decelerate_icon;
    decelerate_icon.addPixmap(*Pixmaps::DECELERATE);
    decelerate_icon.addPixmap(*Pixmaps::DECELERATE_HOVER, QIcon::Active);
    decelerate_action->setIcon(decelerate_icon);

    run_action = new QAction("Run", this);
    QIcon run_icon;
    run_icon.addPixmap(*(Pixmaps::RUN));
    run_icon.addPixmap(*(Pixmaps::RUN_HOVER), QIcon::Active);
    run_action->setIcon(run_icon);

    terminate_action = new QAction("Terminate", this);
    QIcon terminate_icon;
    terminate_icon.addPixmap(*Pixmaps::TERMINATE);
    terminate_icon.addPixmap(*Pixmaps::TERMINATE_HOVER, QIcon::Active);
    terminate_action->setIcon(terminate_icon);

    step_over_action = new QAction("Step Over", this);
    QIcon step_over_icon;
    step_over_icon.addPixmap(*Pixmaps::STEP_OVER);
    step_over_icon.addPixmap(*Pixmaps::STEP_OVER_HOVER, QIcon::Active);
    step_over_action->setIcon(step_over_icon);

    step_into_action = new QAction("Step Into", this);
    QIcon step_into_icon;
    step_into_icon.addPixmap(*Pixmaps::STEP_INTO);
    step_into_icon.addPixmap(*Pixmaps::STEP_INTO_HOVER, QIcon::Active);
    step_into_action->setIcon(step_into_icon);

    step_out_action = new QAction("Step Out", this);
    QIcon step_out_icon;
    step_out_icon.addPixmap(*Pixmaps::STEP_OUT);
    step_out_icon.addPixmap(*Pixmaps::STEP_OUT_HOVER, QIcon::Active);
    step_out_action->setIcon(step_out_icon);

    restart_action = new QAction("Restart debugging session", this);
    QIcon restart_icon;
    restart_icon.addPixmap(*Pixmaps::RESTART);
    restart_icon.addPixmap(*Pixmaps::RESTART_HOVER, QIcon::Active);
    restart_action->setIcon(restart_icon);

    memory_dump_action = new QAction("Dump Memory", this);
    QIcon memory_dump_icon;
    memory_dump_icon.addPixmap(*Pixmaps::DUMP);
    memory_dump_icon.addPixmap(*Pixmaps::DUMP_HOVER, QIcon::Active);
    memory_dump_action->setIcon(memory_dump_icon);
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    // Handle joypad input:
    auto joypad_mapping = JOYPAD_KEYMAP.find(event->key());
    if (joypad_mapping != JOYPAD_KEYMAP.end())
    {
        emuwidget->button_input_widget->keyPressEvent(event);
        return;
    }
    else
    {
        switch (event->key())
        {
            case Qt::Key_Plus:
                break;
            case Qt::Key_Minus:
                break;
            default:
                break;
        }
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    auto joypad_mapping = JOYPAD_KEYMAP.find(event->key());
    if (joypad_mapping != JOYPAD_KEYMAP.end())
    {
        emuwidget->button_input_widget->keyReleaseEvent(event);
    }
}

void MainWindow::init_menubar()
{
    menubar = new QMenuBar(this);

    file_menu = new QMenu("File", this);
    file_menu->addAction(load_rom_action);

    options_menu = new QMenu("Options", this);
    options_menu->addAction(debug_mode_action);
    debug_mode_action->setCheckable(true);
    debug_mode_action->setChecked(true);

    debug_menu = new QMenu("Debug", this);
    debug_menu->addAction(step_over_action);
    debug_menu->addAction(step_into_action);
    debug_menu->addAction(step_out_action);

    menubar->addMenu(file_menu);
    menubar->addMenu(options_menu);
    menubar->addMenu(debug_menu);
    setMenuBar(menubar);
}

void MainWindow::init_toolbar()
{
    toolbar = new QToolBar(this);
    toolbar->addAction(load_rom_action);
    toolbar->addAction(save_game_action);
    toolbar->addAction(decelerate_action);
    toolbar->addAction(accelerate_action);
    toolbar->addAction(run_action);
    toolbar->addAction(terminate_action);
    toolbar->addAction(restart_action);
    toolbar->addAction(step_over_action);
    toolbar->addAction(step_into_action);
    toolbar->addAction(step_out_action);
    toolbar->addAction(memory_dump_action);
    toolbar->addAction(settings_action);

    toolbar->setIconSize(QSize(20, 20));

    addToolBar(toolbar);
}

void MainWindow::init_signals()
{
    connect(load_rom_action,
            &QAction::triggered,
            this,
            &MainWindow::load_rom_act);
    connect(emu->ppu->renderer,
            &Renderer::frame_ready,
            emuwidget->display.scene,
            &DisplayScene::on_frame_ready);
    connect(debug_mode_action,
            &QAction::triggered,
            this,
            &MainWindow::debug_mode_act);
    connect(step_into_action,
            &QAction::triggered,
            this,
            &MainWindow::step_into_act);
    connect(run_action,
            &QAction::triggered,
            this,
            &MainWindow::run_act);
    connect(terminate_action,
            &QAction::triggered,
            this,
            &MainWindow::terminate_act);
}

void MainWindow::load_rom_act()
{
    /*
    auto filepath = QFileDialog::getOpenFileName(this,
                                                 "Choose Game Boy ROM",
                                                 ".",
                                                 "Game Boy ROMs (*.gb)");
    */
    // QString filepath("C:\\Users\\anton\\Desktop\\antgb\\testbin\\blargg\\cpu_instrs\\cpu_instrs.gb");
    QString filepath("C:\\Users\\anton\\Desktop\\antgb\\testbin\\tetris_jue_v1_1.gb");

    // PASSED:
    // QString filepath("C:\\Users\\anton\\Desktop\\antgb\\testbin\\blargg\\cpu_instrs\\individual\\01-special.gb");
    // PASSED:
    // QString filepath("C:\\Users\\anton\\Desktop\\antgb\\testbin\\blargg\\cpu_instrs\\individual\\02-interrupts.gb");
    // PASSED:
    // QString filepath("C:\\Users\\anton\\Desktop\\antgb\\testbin\\blargg\\cpu_instrs\\individual\\03-op sp,hl.gb");
    // PASSED:
    // QString filepath("C:\\Users\\anton\\Desktop\\antgb\\testbin\\blargg\\cpu_instrs\\individual\\04-op r,imm.gb");
    // PASSED:
    // QString filepath("C:\\Users\\anton\\Desktop\\antgb\\testbin\\blargg\\cpu_instrs\\individual\\05-op rp.gb");
    // PASSED:
    // QString filepath("C:\\Users\\anton\\Desktop\\antgb\\testbin\\blargg\\cpu_instrs\\individual\\06-ld r,r.gb");
    // PASSED:
    // QString filepath("C:\\Users\\anton\\Desktop\\antgb\\testbin\\blargg\\cpu_instrs\\individual\\07-jr,jp,call,ret,rst.gb");
    // PASSED:
    // QString filepath("C:\\Users\\anton\\Desktop\\antgb\\testbin\\blargg\\cpu_instrs\\individual\\08-misc instrs.gb");
    // PASSED:
    // QString filepath("C:\\Users\\anton\\Desktop\\antgb\\testbin\\blargg\\cpu_instrs\\individual\\09-op r,r.gb");
    // PASSED:
    // QString filepath("C:\\Users\\anton\\Desktop\\antgb\\testbin\\blargg\\cpu_instrs\\individual\\10-bit ops.gb");
    // PASSED:
    // QString filepath("C:\\Users\\anton\\Desktop\\antgb\\testbin\\blargg\\cpu_instrs\\individual\\11-op a,(hl).gb");

    // PASSED:
    // QString filepath("C:\\Users\\anton\\Desktop\\antgb\\testbin\\blargg\\instr_timing\\instr_timing.gb");

    // NOT PASSED:
    //QString filepath("C:\\Users\\anton\\Desktop\\antgb\\testbin\\blargg\\interrupt_time\\interrupt_time.gb");

    emuwidget->load_rom(filepath);
}

void MainWindow::debug_mode_act()
{
    /*
    emuwidget->toggle_debug_mode();
    debug_menu->setHidden(!debug_menu->isHidden());
    debug_menu->setVisible(!debug_menu->isVisible());
    step_over_action->setVisible(!step_over_action->isVisible());
    step_into_action->setVisible(!step_into_action->isVisible());
    step_out_action->setVisible(!step_out_action->isVisible());
    */
}

void MainWindow::step_into_act()
{
    debugger->step_into();
}

void MainWindow::run_act()
{
    debugger->run();
}

void MainWindow::pause_act()
{
    debugger->pause();
}

void MainWindow::terminate_act()
{
    // TODO: Implement properly.
    debugger->pause();
}
