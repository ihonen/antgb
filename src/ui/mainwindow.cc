#include "mainwindow.hh"

static const uint16_t RES_X = 160;
static const uint16_t RES_Y = 144;
static const uint16_t RES_UPSCALE_FACTOR = 4;

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    display_view_ = new QGraphicsView(this);
    display_view_->resize(RES_X * RES_UPSCALE_FACTOR, RES_Y * RES_UPSCALE_FACTOR);

    main_layout_ = new QHBoxLayout(this);
    main_layout_->insertWidget(0, display_view_);
    this->setLayout(main_layout_);
    this->init_menubar();
    this->resize(display_view_->size());
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
    this->setMenuBar(menubar_);
}
