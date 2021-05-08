#pragma once

#include "cartridgeviewer.hh"
#include "displayscene.hh"
#include "dataviewer.hh"
#include "instructionviewer.hh"
#include "buttoninputwidget.hh"
#include "memoryviewer.hh"
#include "registermodel.hh"
#include "registerviewer.hh"
#include "serialviewer.hh"
#include "stackviewer.hh"
#include "vramviewer.hh"
#include "../emulator/emulator.hh"
#include "../debugger/debugcore.hh"

#include <QGraphicsView>
#include <QGridLayout>
#include <QWidget>

class EmulatorWidget : public QWidget
{
    Q_OBJECT
public slots:
    void toggle_debug_mode();
public:
    struct Status
    {
        bool debug_mode = false;
        bool emulate = false;
    } status;

    struct Display
    {
        DisplayScene* scene;
        QGraphicsView* view;
    } display;

    CartridgeViewer* cartridge_viewer = nullptr;
    ButtonInputWidget* button_input_widget = nullptr;
    DataViewer* data_viewer = nullptr;
    InstructionViewer* instruction_viewer = nullptr;
    MemoryViewer* memory_viewer = nullptr;
    RegisterViewer* register_viewer = nullptr;
    SerialViewer* serial_viewer = nullptr;
    StackViewer* stack_viewer = nullptr;
    VramViewer* vram_viewer = nullptr;
    Emulator* emu = nullptr;
    DebugCore* debugger = nullptr;
    QGridLayout* main_layout;
    QThread* emu_thread;

    EmulatorWidget(Emulator* emulator,
                   DebugCore* debugger,
                   QWidget* parent = nullptr);
    ~EmulatorWidget();
    void load_rom(QString& filepath);
};
