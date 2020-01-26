#include "mach/machine.hh"
#include "ui/mainwindow.hh"

#include <QApplication>
#include <cstdint>
#include <iostream>
#include <cstdio>

#include "gfx/sprite.hh"

using namespace std;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    Memory* mem = new Memory();
    Machine gb;
    MainWindow main_window(&gb, nullptr);
    main_window.show();

    app.exec();

    return 0;
}
