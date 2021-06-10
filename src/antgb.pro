#-------------------------------------------------
#
# Project created by QtCreator 2019-09-10T19:36:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = antgb
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17

SOURCES += \
    debugger/cartridgeheader.cc \
    emulator/cpu/decode.cc \
    emulator/cpu/operations.cc \
    emulator/peripherals/apuregisters.cc \
    emulator/memory/cartridge.cc \
    emulator/cpu/cpuregisters.cc \
    emulator/memory/dma.cc \
    emulator/peripherals/joypadregisters.cc \
    emulator/memory/memorybus.cc \
    emulator/ppu/ppuregisters.cc \
    emulator/peripherals/serialregisters.cc \
    emulator/peripherals/timerregisters.cc \
    main.cc \
    debugger/breakpoint.cc \
    debugger/debugcore.cc \
    debugger/disassembler.cc \
    debugger/instructions.cc \
    emulator/peripherals/apu.cc \
    emulator/cpu/cpu.cc \
    emulator/emulator.cc \
    emulator/common/fileio.cc \
    emulator/cpu/interrupts.cc \
    emulator/peripherals/joypad.cc \
    emulator/ppu/ppu.cc \
    emulator/peripherals/serial.cc \
    emulator/peripherals/timer.cc \
    gui/models/breakpointdelegate.cc \
    gui/models/breakpointmodel.cc \
    gui/models/datadelegate.cc \
    gui/models/datamodel.cc \
    gui/models/displayscene.cc \
    gui/models/instructiondelegate.cc \
    gui/models/instructionmodel.cc \
    gui/models/memorydelegate.cc \
    gui/models/memorymodel.cc \
    gui/models/registermodel.cc \
    gui/models/stackdelegate.cc \
    gui/models/stackmodel.cc \
    gui/util/colors.cc \
    gui/util/fonts.cc \
    gui/util/helper.cc \
    gui/util/pixmaps.cc \
    gui/widgets/breakpointview.cc \
    gui/widgets/button.cc \
    gui/widgets/buttoninputwidget.cc \
    gui/widgets/cartridgeviewer.cc \
    gui/widgets/dataview.cc \
    gui/widgets/dataviewer.cc \
    gui/widgets/emulatorwidget.cc \
    gui/widgets/instructionview.cc \
    gui/widgets/instructionviewer.cc \
    gui/widgets/mainwindow.cc \
    gui/widgets/memoryviewer.cc \
    gui/widgets/registerviewer.cc \
    gui/widgets/serialviewer.cc \
    gui/widgets/stackviewer.cc \
    gui/widgets/vramviewer.cc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    debugger/addresses.hh \
    debugger/breakpoint.hh \
    debugger/cartridgeheader.hh \
    debugger/constants.hh \
    debugger/debugcore.hh \
    debugger/debugevent.hh \
    debugger/disassembler.hh \
    debugger/idebugobserver.hh \
    debugger/iemulator.hh \
    debugger/ifrontend.hh \
    debugger/instructions.hh \
    debugger/types.hh \
    emulator/memory/addresses.hh \
    emulator/peripherals/apu.hh \
    emulator/peripherals/apuregisters.hh \
    emulator/common/bitmanip.hh \
    emulator/memory/bootrom.hh \
    emulator/memory/cartridge.hh \
    emulator/cpu/cpu.hh \
    emulator/cpu/cpuregisters.hh \
    emulator/memory/dma.hh \
    emulator/memory/echoram.hh \
    emulator/emulator.hh \
    emulator/common/exceptions.hh \
    emulator/common/fileio.hh \
    emulator/memory/hram.hh \
    emulator/interfaces/imemorybusnode.hh \
    emulator/cpu/interrupts.hh \
    emulator/peripherals/joypad.hh \
    emulator/peripherals/joypadregisters.hh \
    emulator/common/macros.hh \
    emulator/memory/memorybase.hh \
    emulator/memory/memorybus.hh \
    emulator/memory/oam.hh \
    emulator/ppu/ppu.hh \
    emulator/ppu/ppuregisters.hh \
    emulator/common/registerbase.hh \
    emulator/peripherals/serial.hh \
    emulator/peripherals/serialregisters.hh \
    emulator/peripherals/timer.hh \
    emulator/peripherals/timerregisters.hh \
    emulator/common/types.hh \
    emulator/memory/vram.hh \
    emulator/memory/wram0.hh \
    emulator/memory/wram1.hh \
    gui/models/breakpointdelegate.hh \
    gui/models/breakpointmodel.hh \
    gui/models/datadelegate.hh \
    gui/models/datamodel.hh \
    gui/models/displayscene.hh \
    gui/models/instructiondelegate.hh \
    gui/models/instructionmodel.hh \
    gui/models/memorydelegate.hh \
    gui/models/memorymodel.hh \
    gui/models/registermodel.hh \
    gui/models/stackdelegate.hh \
    gui/models/stackmodel.hh \
    gui/util/cartridge.hh \
    gui/util/colors.hh \
    gui/util/fonts.hh \
    gui/util/helper.hh \
    gui/util/keybindings.hh \
    gui/util/pixmaps.hh \
    gui/widgets/breakpointview.hh \
    gui/widgets/button.hh \
    gui/widgets/buttoninputwidget.hh \
    gui/widgets/cartridgeviewer.hh \
    gui/widgets/dataview.hh \
    gui/widgets/dataviewer.hh \
    gui/widgets/emulatorwidget.hh \
    gui/widgets/instructionview.hh \
    gui/widgets/instructionviewer.hh \
    gui/widgets/mainwindow.hh \
    gui/widgets/memoryviewer.hh \
    gui/widgets/registerviewer.hh \
    gui/widgets/serialviewer.hh \
    gui/widgets/stackviewer.hh \
    gui/widgets/vramviewer.hh \
    util/macros.hh

RESOURCES += \
    gui/assets/dark.qrc \
    gui/assets/img.qrc \
    memdump.qrc

FORMS +=
