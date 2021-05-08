#include "vramviewer.hh"

#include <QTabBar>

VramViewer::VramViewer(DebugCore* debugger, QWidget* parent) :
    QFrame(parent),
    debugger(debugger)
{
    tabs = new QTabWidget(this);
    tabs->addTab(new QWidget(this), "BG Map");
    tabs->addTab(new QWidget(this), "Tiles");
    tabs->addTab(new QWidget(this), "OAM");
    tabs->addTab(new QWidget(this), "Palettes");
    tabs->setUsesScrollButtons(false);

    setFixedHeight(320);
    setStyleSheet("background-color: #232629");
}
