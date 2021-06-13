#pragma once

#include "debugger/debugcore.hh"
#include <QFrame>
#include <QLabel>
#include <QMetaType>
#include <QGridLayout>
#include <QPlainTextEdit>

Q_DECLARE_METATYPE(uint8_t)

class SerialViewer : public QPlainTextEdit
{
    Q_OBJECT
public:
    DebugCore* debugger = nullptr;

    SerialViewer(DebugCore* debugger, QWidget* parent = nullptr);
    virtual ~SerialViewer() override;
    void update();

    void serial_callback(uint8_t byte);
signals:
    void serial_data_received(uint8_t byte);
protected slots:
    void on_serial_data_received(uint8_t byte);
};
