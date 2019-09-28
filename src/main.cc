#include "mach/cpu/cpu.hh"
#include "ui/mainwindow.hh"

#include <QApplication>
#include <cstdint>
#include <iostream>
#include <cstdio>

int main(int argc, char* argv[])
{
    GBMachine gb;

    QApplication app(argc, argv);
    MainWindow main_window(nullptr);
    main_window.show();

    app.exec();

    return 0;
}
