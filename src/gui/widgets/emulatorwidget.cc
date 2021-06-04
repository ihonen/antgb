#include "emulatorwidget.hh"

#include <QFile>
#include <QThread>

#include "emulator/fileio.hh"

void EmulatorWidget::toggle_debug_mode()
{
    instruction_viewer->setHidden(!instruction_viewer->isHidden());
    stack_viewer->setHidden(!stack_viewer->isHidden());
    data_viewer->setHidden(!data_viewer->isHidden());
    register_viewer->setHidden(!register_viewer->isHidden());
    vram_viewer->setHidden(!vram_viewer->isHidden());
}

EmulatorWidget::EmulatorWidget(Emulator* emulator,
                               DebugCore* debugger,
                               QWidget* parent) :
    QWidget(parent),
    emu(emulator),
    debugger(debugger)
{
    display.scene = new DisplayScene(debugger, this);
    display.view = new QGraphicsView(display.scene, this);
    display.view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    display.view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    display.view->setFrameStyle(QFrame::Box);
    display.view->setFixedSize(DisplayScene::REAL_RES_X / 2 + display.view->frameWidth(),
                               DisplayScene::REAL_RES_Y / 2 + display.view->frameWidth());

    data_viewer = new DataViewer(debugger, this);

    auto instruction_model = new InstructionModel(debugger, this);
    instruction_viewer = new InstructionViewer(debugger, instruction_model, this);

    vram_viewer = new VramViewer(debugger, this);
    auto register_model = new RegisterModel(debugger, this);
    register_viewer = new RegisterViewer(debugger, register_model, this);
    stack_viewer = new StackViewer(debugger, this);

    memory_viewer = new MemoryViewer(debugger, this);

    button_input_widget = new ButtonInputWidget(debugger, this);

    main_layout = new QGridLayout(this);

    serial_viewer = new SerialViewer(debugger, this);

    cartridge_viewer = new CartridgeViewer(debugger, this);

    main_layout->addWidget(display.view, 0, 0);
    main_layout->addWidget(button_input_widget, 0, 1);
    main_layout->addWidget(vram_viewer, 1, 0, 1, 2);
    main_layout->addWidget(instruction_viewer, 2, 0, 1, 3);
    main_layout->addWidget(data_viewer, 2, 3, 1, 2);
    main_layout->addWidget(serial_viewer, 2, 5, 1, 1);
    main_layout->addWidget(register_viewer, 0, 2, 2, 1);
    main_layout->addWidget(stack_viewer, 0, 3, 2, 1);
    main_layout->addWidget(memory_viewer, 0, 4, 2, 1);
    main_layout->addWidget(cartridge_viewer, 0, 5, 2, 3);
    setLayout(main_layout);
}

EmulatorWidget::~EmulatorWidget()
{

}

void EmulatorWidget::load_rom(QString& filepath)
{
    if (filepath == "") return;

    debugger->load_rom(filepath.toStdString());
}
