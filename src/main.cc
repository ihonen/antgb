#include "emulator/emulator.hh"
#include "gui/widgets/mainwindow.hh"

#include <QApplication>
#include <QTextStream>
#include <cstdint>
#include <iostream>
#include <cstdio>

#include "emulator/sprite.hh"

using namespace std;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QFile file(":/dark/dark.qss");
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream(&file);
    app.setStyleSheet(stream.readAll());

    Emulator emu;
    DebugCore debugger(&emu);
    MainWindow main_window(&emu, &debugger);
    main_window.show();

    app.exec();

    return 0;
}
