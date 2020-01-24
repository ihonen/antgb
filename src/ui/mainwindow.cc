#include "mainwindow.hh"

#include "../util/fileio.hh"
#include "keymappings.hh"
#include <thread>
#include <QFileDialog>

static const uint16_t RES_X = 160;
static const uint16_t RES_Y = 144;
static const uint16_t RES_UPSCALE_FACTOR = 4;

static uint8_t tetris_memdump1[0x10000];
static uint8_t tetris_memdump2[0x10000];

static void load_tetris_dumps()
{
    QString dump1_path = ":/dump/tetris_menu.dump";
    QString dump2_path = ":/dump/tetris_game.dump";
    load_rom(dump1_path, tetris_memdump1);
    load_rom(dump2_path, tetris_memdump2);
}

MainWindow::MainWindow(Machine& machine_, QWidget* parent) :
    QMainWindow(parent),
    machine(machine_),
    is_emulation_on(false),
    tick_interval(100)
{
    display_ = new Display(*machine.ppu, *machine.renderer);
    display_view_ = new QGraphicsView(display_, this);
    display_view_->resize(160 * 4 + 4, 144 * 4 + 4);
    display_view_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    display_view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setCentralWidget(display_view_);

    init_menubar();
    init_signals();

    QObject::connect(machine.renderer, &Renderer::frame_ready,
                     display_, &Display::on_frame_ready);

    load_tetris_dumps();
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
    QString filepath(":/rom/boot.bin");
    //QString filepath("C:\\Users\\anton\\Desktop\\antgb\\testbin\\cpu_instrs\\cpu_instrs\\cpu_instrs.gb");
    //QString filepath("C:\\Users\\anton\\Desktop\\antgb\\testbin\\tetris_jue_v1_1.gb");
    load_rom(filepath, machine.mmu->mem.data);
    //machine.load_rom(tetris_memdump2, 0x10000);
    //emulation_thread = new std::thread(&MainWindow::start_emulation, this);
    emulation_qthread = QThread::create([&]() { MainWindow::start_emulation(); });
    emulation_qthread->start();
    emulation_qthread->setPriority(QThread::TimeCriticalPriority);
}

void MainWindow::start_emulation()
{
    is_emulation_on = true;
    while (is_emulation_on)
    {
        machine.tick();
        //std::this_thread::sleep_for(std::chrono::milliseconds(tick_interval));
    }
}

void MainWindow::stop_emulation()
{
    if (emulation_qthread)
    {
        is_emulation_on = false;
        emulation_qthread->exit();
    }
    /*
    if (emulation_thread)
    {
        is_emulation_on = false;
        emulation_thread->join();
        delete emulation_thread;
        emulation_thread = nullptr;
    }
        */
    /*
    */
}
