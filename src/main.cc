#include "mach/machine.hh"
#include "ui/mainwindow.hh"

#include <QApplication>
#include <cstdint>
#include <iostream>
#include <cstdio>

#include "mach/sprite.hh"

using namespace std;

static uint8_t tetris_memdump1[0x10000];
static uint8_t tetris_memdump2[0x10000];

static void load_tetris_dumps()
{
    QString dump1_path = ":/dump/tetris_menu.dump";
    QString dump2_path = ":/dump/tetris_game.dump";
    MainWindow::load_rom(dump1_path, tetris_memdump1);
    MainWindow::load_rom(dump2_path, tetris_memdump2);
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    Machine gb;
    MainWindow main_window(gb, nullptr);
    main_window.show();

    load_tetris_dumps();

    gb.renderer->set_memory(tetris_memdump1);

    app.exec();

    return 0;
}
