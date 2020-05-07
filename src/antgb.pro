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
    debugger/disassembler.cc \
    frontend/qt/displaywidget.cc \
    frontend/qt/main.cc \
    frontend/qt/mainwindow.cc

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
    debugger/disassembler.hh \
    frontend/qt/displaywidget.hh \
    frontend/qt/keybindings.hh \
    frontend/qt/mainwindow.hh \
    util/macros.hh

RESOURCES += \
    memdump.qrc

FORMS += \
    frontend/qt/debugwidget.ui
