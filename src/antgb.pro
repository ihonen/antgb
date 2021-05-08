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
    debugger/breakpoint.cc \
    debugger/cartridge.cc \
    debugger/debugcore.cc \
    debugger/disassembler.cc \
    debugger/instructions.cc \
    gui/breakpointdelegate.cc \
    gui/breakpointmodel.cc \
    gui/breakpointview.cc \
    gui/button.cc \
    gui/buttoninputwidget.cc \
    gui/cartridgeviewer.cc \
    gui/colors.cc \
    gui/datadelegate.cc \
    gui/datamodel.cc \
    gui/dataview.cc \
    gui/dataviewer.cc \
    gui/displayscene.cc \
    gui/emulatorwidget.cc \
    gui/fonts.cc \
    gui/helper.cc \
    gui/instructiondelegate.cc \
    gui/instructionmodel.cc \
    gui/instructionview.cc \
    gui/instructionviewer.cc \
    gui/main.cc \
    gui/mainwindow.cc \
    gui/memorydelegate.cc \
    gui/memorymodel.cc \
    gui/memoryviewer.cc \
    gui/pixmaps.cc \
    gui/registermodel.cc \
    gui/registerviewer.cc \
    gui/serialviewer.cc \
    gui/stackdelegate.cc \
    gui/stackmodel.cc \
    gui/stackviewer.cc \
    gui/vramviewer.cc \
    emulator/apu.cc \
    emulator/background.cc \
    emulator/bitmanip.cc \
    emulator/cpu.cc \
    emulator/cpu_opcodes.cc \
    emulator/cpu_operations.cc \
    emulator/emulator.cc \
    emulator/fileio.cc \
    emulator/interrupts.cc \
    emulator/joypad.cc \
    emulator/memory.cc \
    emulator/ppu.cc \
    emulator/renderer.cc \
    emulator/serial.cc \
    emulator/sprite.cc \
    emulator/sprites.cc \
    emulator/tile.cc \
    emulator/timer.cc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    debugger/addresses.hh \
    debugger/breakpoint.hh \
    debugger/cartridge.hh \
    debugger/constants.hh \
    debugger/debugcore.hh \
    debugger/debugevent.hh \
    debugger/disassembler.hh \
    debugger/idebugobserver.hh \
    debugger/iemulator.hh \
    debugger/instructions.hh \
    debugger/macros.hh \
    debugger/types.hh \
    gui/cartridge.hh \
    gui/breakpointdelegate.hh \
    gui/breakpointmodel.hh \
    gui/breakpointview.hh \
    gui/button.hh \
    gui/buttoninputwidget.hh \
    gui/cartridgeviewer.hh \
    gui/colors.hh \
    gui/datadelegate.hh \
    gui/datamodel.hh \
    gui/dataview.hh \
    gui/dataviewer.hh \
    gui/displayscene.hh \
    gui/emulatorwidget.hh \
    gui/fonts.hh \
    gui/helper.hh \
    gui/instructiondelegate.hh \
    gui/instructionmodel.hh \
    gui/instructionview.hh \
    gui/instructionviewer.hh \
    gui/keybindings.hh \
    gui/mainwindow.hh \
    gui/memorydelegate.hh \
    gui/memorymodel.hh \
    gui/memoryviewer.hh \
    gui/pixmaps.hh \
    gui/registermodel.hh \
    gui/registerviewer.hh \
    gui/serialviewer.hh \
    gui/stackdelegate.hh \
    gui/stackmodel.hh \
    gui/stackviewer.hh \
    gui/vramviewer.hh \
    emulator/apu.hh \
    emulator/background.hh \
    emulator/bitmanip.hh \
    emulator/cpu.hh \
    emulator/emulator.hh \
    emulator/exceptions.hh \
    emulator/fileio.hh \
    emulator/interrupts.hh \
    emulator/joypad.hh \
    emulator/memory.hh \
    emulator/ppu.hh \
    emulator/renderer.hh \
    emulator/serial.hh \
    emulator/sprite.hh \
    emulator/sprites.hh \
    emulator/tile.hh \
    emulator/timer.hh \
    emulator/types.hh \
    util/macros.hh

RESOURCES += \
    gui/dark.qrc \
    gui/img.qrc \
    memdump.qrc

FORMS +=
