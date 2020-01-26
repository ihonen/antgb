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
    gfx/background.cc \
    gfx/renderer.cc \
    gfx/sprite.cc \
    gfx/sprites.cc \
    gfx/tile.cc \
    mach/cartridge.cc \
    mach/cpu.cc \
    mach/interrupts.cc \
    mach/joypad.cc \
    mach/memory.cc \
    mach/ppu.cc \
    mach/timer.cc \
    main.cc \
    mach/cpu_flags.cc \
    mach/cpu_opmap.cc \
    mach/cpu_opcodes.cc \
    mach/cpu_operations.cc \
    mach/machine.cc \
    ui/display.cc \
    ui/mainwindow.cc \
    util/bitmanip.cc \
    util/disassembler.cc \
    util/fileio.cc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    error/exception.hh \
    gfx/background.hh \
    gfx/renderer.hh \
    gfx/sprite.hh \
    gfx/sprites.hh \
    gfx/tile.hh \
    mach/cartridge.hh \
    mach/cpu.hh \
    mach/interrupts.hh \
    mach/joypad.hh \
    mach/machine.hh \
    mach/memory.hh \
    mach/ppu.hh \
    mach/timer.hh \
    ui/display.hh \
    ui/keybindings.hh \
    ui/mainwindow.hh \
    util/bitmanip.hh \
    util/disassembler.hh \
    util/fileio.hh \
    util/typedefs.hh

RESOURCES += \
    memdump.qrc
