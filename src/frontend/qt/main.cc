#include "core/emulator.hh"
#include "mainwindow.hh"

#include <QApplication>
#include <cstdint>
#include <iostream>
#include <cstdio>

#include "core/sprite.hh"

using namespace std;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    Emulator gb;
    MainWindow main_window(&gb, nullptr);
    main_window.show();

    app.exec();

    return 0;
}
