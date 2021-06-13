#include "serialviewer.hh"

#include "gui/util/fonts.hh"

SerialViewer::SerialViewer(DebugCore* debugger, QWidget* parent) :
    QPlainTextEdit(parent),
    debugger(debugger)
{
    qRegisterMetaType<uint8_t>("uint8_t");

    setReadOnly(true);

    connect(this, &SerialViewer::serial_data_received,
            this, &SerialViewer::on_serial_data_received);

    setFont(*Fonts::COURIER);
}

SerialViewer::~SerialViewer()
{

}

void SerialViewer::update()
{

}

void SerialViewer::serial_callback(uint8_t byte)
{
    emit serial_data_received(byte);
}

void SerialViewer::on_serial_data_received(uint8_t byte)
{
    insertPlainText(QString(static_cast<char>(byte)));
    std::cerr << static_cast<char>(byte);
}
