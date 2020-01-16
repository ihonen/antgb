#include "mainwindow.hh"

#include <QFileDialog>

static const uint16_t RES_X = 160;
static const uint16_t RES_Y = 144;
static const uint16_t RES_UPSCALE_FACTOR = 4;

MainWindow::MainWindow(Machine& machine_, QWidget* parent) :
    QMainWindow(parent),
    machine(machine_)
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
    auto filepath = QFileDialog::getOpenFileName(this,
                                                 "Choose Game Boy ROM",
                                                 ".",
                                                 "Game Boy ROMs (*.gb)");
    load_rom(filepath);
}

void MainWindow::load_rom(QString& filepath)
{
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly))
    {
        assert(false && "Couldn't open ROM file");
    }

    QByteArray executable = file.readAll();
    machine.load_rom(executable.data(), (size_t)executable.size() - 1);
    start_emulation();
}

void MainWindow::start_emulation()
{
    for (uint64_t i = 0; i < 10000; ++i)
    {
        cout << machine.cpu->PC << endl;
        machine.tick();
    }
}
