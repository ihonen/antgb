#include "serialviewer.hh"

SerialViewer::SerialViewer(DebugCore* debugger, QWidget* parent) :
    QPlainTextEdit(parent),
    debugger(debugger)
{
    setReadOnly(true);
}

SerialViewer::~SerialViewer()
{

}

void SerialViewer::update()
{

}
