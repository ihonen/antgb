#include "mach/machine.hh"
#include "ui/mainwindow.hh"

#include <QApplication>
#include <cstdint>
#include <iostream>
#include <cstdio>

#include "mach/sprite.hh"

using namespace std;

int main(int argc, char* argv[])
{
    Machine gb;

    QApplication app(argc, argv);
    MainWindow main_window(nullptr);
    main_window.show();
    app.exec();

    return 0;
}
