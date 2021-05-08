#pragma once

#include "../debugger/debugcore.hh"
#include <QFrame>
#include <QLabel>
#include <QGridLayout>
#include <QPlainTextEdit>

class SerialViewer : public QPlainTextEdit
{
    Q_OBJECT
public:
    DebugCore* debugger = nullptr;

    SerialViewer(DebugCore* debugger, QWidget* parent = nullptr);
    virtual ~SerialViewer() override;
    void update();
};
