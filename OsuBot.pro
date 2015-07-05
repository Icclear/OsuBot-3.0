#-------------------------------------------------
#
# Project created by QtCreator 2015-06-29T20:17:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = OsuBot
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Osu/osumanagement.cpp \
    program.cpp \
    Include/aobscan.cpp \
    Osu/beatmap.cpp \
    Play/playmanagement.cpp \
    Play/relax.cpp \
    Play/auto.cpp

HEADERS  += mainwindow.hpp \
    Osu/osumanagement.hpp \
    program.hpp \
    Include/aobscan.hpp \
    Osu/beatmap.hpp \
    Include/splitstring.hpp \
    Play/playmanagement.hpp \
    Play/play.hpp \
    Play/relax.hpp \
    Play/auto.hpp

FORMS    += mainwindow.ui

DEFINES -= UNICODE

LIBS += -L "C:\Program Files (x86)\Windows Kits\8.1\Lib\winv6.3\um\x86" -lpsapi
