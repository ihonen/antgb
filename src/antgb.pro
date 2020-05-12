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
    core/background.cc \
    core/bitmanip.cc \
    core/cartridge.cc \
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
    core/timer.cc \
    debugger/debugcore.cc \
    debugger/disassembler.cc \
    frontend/qt/breakpointdelegate.cc \
    frontend/qt/breakpointmodel.cc \
    frontend/qt/breakpointview.cc \
    frontend/qt/button.cc \
    frontend/qt/buttoninputwidget.cc \
    frontend/qt/colors.cc \
    frontend/qt/datamodel.cc \
    frontend/qt/dataviewer.cc \
    frontend/qt/displayscene.cc \
    frontend/qt/emulatorwidget.cc \
    frontend/qt/fonts.cc \
    frontend/qt/helper.cc \
    frontend/qt/instructiondelegate.cc \
    frontend/qt/instructionmodel.cc \
    frontend/qt/instructionview.cc \
    frontend/qt/instructionviewer.cc \
    frontend/qt/main.cc \
    frontend/qt/mainwindow.cc \
    frontend/qt/memorymodel.cc \
    frontend/qt/memoryviewer.cc \
    frontend/qt/pixmaps.cc \
    frontend/qt/registermodel.cc \
    frontend/qt/registerviewer.cc \
    frontend/qt/serialviewer.cc \
    frontend/qt/stackmodel.cc \
    frontend/qt/stackviewer.cc \
    frontend/qt/vramviewer.cc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    core/background.hh \
    core/bitmanip.hh \
    core/cartridge.hh \
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
    debugger/debugcore.hh \
    debugger/debugobserver.hh \
    debugger/disassembler.hh \
    frontend/qt/breakpointdelegate.hh \
    frontend/qt/breakpointmodel.hh \
    frontend/qt/breakpointview.hh \
    frontend/qt/button.hh \
    frontend/qt/buttoninputwidget.hh \
    frontend/qt/colors.hh \
    frontend/qt/datamodel.hh \
    frontend/qt/dataviewer.hh \
    frontend/qt/displayscene.hh \
    frontend/qt/emulatorwidget.hh \
    frontend/qt/fonts.hh \
    frontend/qt/helper.hh \
    frontend/qt/instructiondelegate.hh \
    frontend/qt/instructionmodel.hh \
    frontend/qt/instructionview.hh \
    frontend/qt/instructionviewer.hh \
    frontend/qt/keybindings.hh \
    frontend/qt/mainwindow.hh \
    frontend/qt/memorymodel.hh \
    frontend/qt/memoryviewer.hh \
    frontend/qt/pixmaps.hh \
    frontend/qt/registermodel.hh \
    frontend/qt/registerviewer.hh \
    frontend/qt/serialviewer.hh \
    frontend/qt/stackmodel.hh \
    frontend/qt/stackviewer.hh \
    frontend/qt/vramviewer.hh \
    util/macros.hh

RESOURCES += \
    frontend/qt/dark.qrc \
    frontend/qt/img.qrc \
    memdump.qrc

FORMS += \
    frontend/qt/form.ui
