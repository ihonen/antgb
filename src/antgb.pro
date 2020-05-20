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

CONFIG += c++14

SOURCES += \
    antdbg/src/core/breakpoint.cc \
    antdbg/src/core/cartridge.cc \
    antdbg/src/core/debugcore.cc \
    antdbg/src/core/disassembler.cc \
    antdbg/src/frontend/qt/breakpointdelegate.cc \
    antdbg/src/frontend/qt/breakpointmodel.cc \
    antdbg/src/frontend/qt/breakpointview.cc \
    antdbg/src/frontend/qt/button.cc \
    antdbg/src/frontend/qt/buttoninputwidget.cc \
    antdbg/src/frontend/qt/cartridgeviewer.cc \
    antdbg/src/frontend/qt/colors.cc \
    antdbg/src/frontend/qt/datadelegate.cc \
    antdbg/src/frontend/qt/datamodel.cc \
    antdbg/src/frontend/qt/dataview.cc \
    antdbg/src/frontend/qt/dataviewer.cc \
    antdbg/src/frontend/qt/displayscene.cc \
    antdbg/src/frontend/qt/emulatorwidget.cc \
    antdbg/src/frontend/qt/fonts.cc \
    antdbg/src/frontend/qt/helper.cc \
    antdbg/src/frontend/qt/instructiondelegate.cc \
    antdbg/src/frontend/qt/instructionmodel.cc \
    antdbg/src/frontend/qt/instructionview.cc \
    antdbg/src/frontend/qt/instructionviewer.cc \
    antdbg/src/frontend/qt/main.cc \
    antdbg/src/frontend/qt/mainwindow.cc \
    antdbg/src/frontend/qt/memorymodel.cc \
    antdbg/src/frontend/qt/memoryviewer.cc \
    antdbg/src/frontend/qt/pixmaps.cc \
    antdbg/src/frontend/qt/registermodel.cc \
    antdbg/src/frontend/qt/registerviewer.cc \
    antdbg/src/frontend/qt/serialviewer.cc \
    antdbg/src/frontend/qt/stackmodel.cc \
    antdbg/src/frontend/qt/stackviewer.cc \
    antdbg/src/frontend/qt/vramviewer.cc \
    core/apu.cc \
    core/background.cc \
    core/bitmanip.cc \
    core/cpu.cc \
    core/cpu_flags.cc \
    core/cpu_opcodes.cc \
    core/cpu_operations.cc \
    core/cpu_opmap.cc \
    core/emulator.cc \
    core/fileio.cc \
    core/interrupts.cc \
    core/joypad.cc \
    core/memory.cc \
    core/ppu.cc \
    core/renderer.cc \
    core/serial.cc \
    core/sprite.cc \
    core/sprites.cc \
    core/tile.cc \
    core/timer.cc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    antdbg/src/core/breakpoint.hh \
    antdbg/src/core/cartridge.hh \
    antdbg/src/core/constants.hh \
    antdbg/src/core/debugcore.hh \
    antdbg/src/core/debugevent.hh \
    antdbg/src/core/disassembler.hh \
    antdbg/src/core/idebugobserver.hh \
    antdbg/src/core/iemulator.hh \
    antdbg/src/core/macros.hh \
    antdbg/src/core/types.hh \
    antdbg/src/frontend/cartridge.hh \
    antdbg/src/frontend/qt/breakpointdelegate.hh \
    antdbg/src/frontend/qt/breakpointmodel.hh \
    antdbg/src/frontend/qt/breakpointview.hh \
    antdbg/src/frontend/qt/button.hh \
    antdbg/src/frontend/qt/buttoninputwidget.hh \
    antdbg/src/frontend/qt/cartridgeviewer.hh \
    antdbg/src/frontend/qt/colors.hh \
    antdbg/src/frontend/qt/datadelegate.hh \
    antdbg/src/frontend/qt/datamodel.hh \
    antdbg/src/frontend/qt/dataview.hh \
    antdbg/src/frontend/qt/dataviewer.hh \
    antdbg/src/frontend/qt/displayscene.hh \
    antdbg/src/frontend/qt/emulatorwidget.hh \
    antdbg/src/frontend/qt/fonts.hh \
    antdbg/src/frontend/qt/helper.hh \
    antdbg/src/frontend/qt/instructiondelegate.hh \
    antdbg/src/frontend/qt/instructionmodel.hh \
    antdbg/src/frontend/qt/instructionview.hh \
    antdbg/src/frontend/qt/instructionviewer.hh \
    antdbg/src/frontend/qt/keybindings.hh \
    antdbg/src/frontend/qt/mainwindow.hh \
    antdbg/src/frontend/qt/memorymodel.hh \
    antdbg/src/frontend/qt/memoryviewer.hh \
    antdbg/src/frontend/qt/pixmaps.hh \
    antdbg/src/frontend/qt/registermodel.hh \
    antdbg/src/frontend/qt/registerviewer.hh \
    antdbg/src/frontend/qt/serialviewer.hh \
    antdbg/src/frontend/qt/stackmodel.hh \
    antdbg/src/frontend/qt/stackviewer.hh \
    antdbg/src/frontend/qt/vramviewer.hh \
    core/apu.hh \
    core/background.hh \
    core/bitmanip.hh \
    core/cpu.hh \
    core/emulator.hh \
    core/exceptions.hh \
    core/fileio.hh \
    core/interrupts.hh \
    core/joypad.hh \
    core/memory.hh \
    core/ppu.hh \
    core/renderer.hh \
    core/serial.hh \
    core/sprite.hh \
    core/sprites.hh \
    core/tile.hh \
    core/timer.hh \
    core/types.hh \
    util/macros.hh

RESOURCES += \
    antdbg/src/frontend/qt/dark.qrc \
    antdbg/src/frontend/qt/img.qrc \
    memdump.qrc

FORMS +=
