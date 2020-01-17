#-------------------------------------------------
#
# Project created by QtCreator 2014-12-15T15:50:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia

QT    += gui declarative

TARGET = Pacman
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

SOURCES += main.cpp\
    ball.cpp \
    ghost.cpp \
    pacmanboard.cpp \
    options.cpp \
    powerball.cpp \
    textdrawing.cpp \
    pacmanwindow.cpp \
    pacman.cpp \
    list.cpp

HEADERS  += \
    ball.h \
    ghost.h \
    pacmanboard.h \
    options.h \
    powerball.h \
    textdrawing.h \
    pacmanwindow.h \
    pacman.h \
    list.h

FORMS    += \
    pacmanwindow.ui \
    options.ui

RESOURCES += \
    Resources.qrc \
    Images.qrc \
    Sounds.qrc
