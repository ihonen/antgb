#include "mainwindow.hh"

#include "../../core/cartridge.hh"
#include "../../core/fileio.hh"
#include "keybindings.hh"
#include <thread>
#include <QFileDialog>

MainWindow::MainWindow(Emulator* machine_, QWidget* parent) :
    QMainWindow(parent),
    machine(machine_),
    is_emulation_on(false)
{
    display = new DisplayWidget(machine->mem, machine->ppu->renderer);
    display_view = new QGraphicsView(display, this);
    display_view->resize(DisplayWidget::REAL_RES_X + display_view->frameWidth(),
                          DisplayWidget::REAL_RES_Y + display_view->frameWidth());
    display_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    display_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setCentralWidget(display_view);

    init_menubar();
    init_signals();
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
        machine->button_pressed(joypad_mapping->second);
        return;
    }

    // Handle GUI input:
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

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    auto joypad_mapping = KEYMAP.find(event->key());
    if (joypad_mapping != KEYMAP.end())
    {
        machine->button_released(joypad_mapping->second);
    }
}

void MainWindow::init_menubar()
{
    menubar = new QMenuBar(this);

    file_menu = new QMenu("File", this);
    load_rom_action = new QAction("Load ROM", menubar);
    file_menu->addAction(load_rom_action);

    options_menu = new QMenu("Options", this);

    menubar->addMenu(file_menu);
    menubar->addMenu(options_menu);
    setMenuBar(menubar);
}

void MainWindow::init_signals()
{
    connect(load_rom_action,
            &QAction::triggered,
            this,
            &MainWindow::load_rom_act);
    connect(machine->ppu->renderer,
            &Renderer::frame_ready,
            display,
            &DisplayWidget::on_frame_ready);
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
    // QString filepath("C:\\Users\\anton\\Desktop\\antgb\\testbin\\blargg\\interrupt_time\\interrupt_time.gb");


    auto cartridge = new Cartridge();
    load_rom(filepath, cartridge->data);
    cartridge->size = 0x8000;
    machine->insert_cartridge(cartridge);
    emulation_qthread = QThread::create([&]() { MainWindow::start_emulation(); });
    emulation_qthread->start();
    emulation_qthread->setPriority(QThread::TimeCriticalPriority);
}

void MainWindow::start_emulation()
{
    is_emulation_on = true;
    while (is_emulation_on)
    {
        machine->tick();
    }
}

void MainWindow::stop_emulation()
{
    if (emulation_qthread)
    {
        is_emulation_on = false;
        emulation_qthread->wait();
        delete emulation_qthread;
        emulation_qthread = nullptr;
    }
}
