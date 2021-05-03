#include "core/emulator.hh"
#include "gui/mainwindow.hh"
#include <QApplication>

int main(int argc, char** argv)
{
    QApplication app = QApplication(argc, argv);
    antgb::MainWindow mainwindow;
    mainwindow.show();

    return app.exec();
}
