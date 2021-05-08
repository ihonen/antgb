#pragma once

#include "../debugger/debugcore.hh"
#include <QFrame>
#include <QLabel>
#include <QGridLayout>
#include <QTabWidget>

class VramViewer : public QFrame
{
public:
    DebugCore* debugger = nullptr;
    QLabel* title_label = nullptr;

    QTabWidget* tabs = nullptr;

    VramViewer(DebugCore* debugger, QWidget* parent = nullptr);
    void update();
};
