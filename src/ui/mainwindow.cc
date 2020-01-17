#include "mainwindow.hh"

#include "keymappings.hh"
#include <thread>
#include <QFileDialog>

static const uint16_t RES_X = 160;
static const uint16_t RES_Y = 144;
static const uint16_t RES_UPSCALE_FACTOR = 4;

MainWindow::MainWindow(Machine& machine_, QWidget* parent) :
    QMainWindow(parent),
    machine(machine_),
    is_emulation_on(false),
    tick_interval(100)
{
    display_view_ = new QGraphicsView(this);
    display_view_->resize(RES_X * RES_UPSCALE_FACTOR, RES_Y * RES_UPSCALE_FACTOR);

    main_layout_ = new QHBoxLayout(this);
    main_layout_->insertWidget(0, display_view_);
    setLayout(main_layout_);
    init_menubar();
    init_signals();
    resize(display_view_->size());
}

MainWindow::~MainWindow()
{
    stop_emulation();
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    // Handle joypad input:
    auto joypad_mapping = KEYMAP.find(event->key());
    if (joypad_mapping != KEYMAP.end())
    {
        machine.button_pressed(joypad_mapping->second);
        return;
    }

    // Handle GUI input:
    switch (event->key())
    {
        case Qt::Key_Plus:
            tick_interval = std::max(tick_interval - 5, 1);
            break;
        case Qt::Key_Minus:
            tick_interval = std::min(tick_interval + 5, 2000);
            break;
        default:
            break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    auto joypad_mapping = KEYMAP.find(event->key());
    if (joypad_mapping != KEYMAP.end())
    {
        machine.button_released(joypad_mapping->second);
    }
}

void MainWindow::init_menubar()
{
    menubar_ = new QMenuBar(this);

    file_menu_ = new QMenu("File", this);
    load_rom_act_ = new QAction("Load ROM", menubar_);
    file_menu_->addAction(load_rom_act_);

    options_menu_ = new QMenu("Options", this);

    menubar_->addMenu(file_menu_);
    menubar_->addMenu(options_menu_);
    setMenuBar(menubar_);
}

void MainWindow::init_signals()
{
    connect(load_rom_act_,
            &QAction::triggered,
            this,
            &MainWindow::load_rom_act);
}

void MainWindow::load_rom_act()
{
    /*
    auto filepath = QFileDialog::getOpenFileName(this,
                                                 "Choose Game Boy ROM",
                                                 ".",
                                                 "Game Boy ROMs (*.gb)");
    */
    QString filepath("C:\\Users\\anton\\Desktop\\antgb\\testbin\\tetris_jue_v1_1.gb");
    load_rom(filepath);
}

void MainWindow::load_rom(QString& filepath)
{
    if (filepath.size() == 0) return;

    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly))
    {
        assert(false && "Couldn't open ROM file");
    }

    QByteArray executable = file.readAll();
    machine.load_rom(executable.data(), (size_t)executable.size() - 1);
    emulation_thread = new std::thread(&MainWindow::start_emulation, this);
}

void MainWindow::start_emulation()
{
    is_emulation_on = true;
    while (is_emulation_on)
    {
        machine.tick();
        std::this_thread::sleep_for(std::chrono::milliseconds(tick_interval));
    }
}

void MainWindow::stop_emulation()
{
    if (emulation_thread)
    {
        is_emulation_on = false;
        emulation_thread->join();
        delete emulation_thread;
        emulation_thread = nullptr;
    }
}
