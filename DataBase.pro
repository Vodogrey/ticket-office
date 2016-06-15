QT += core gui widgets sql

TARGET = DataBase
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    mainwindow.cpp \
    connectdtabase.cpp \
    dialogHallType.cpp \
    dialogBldg.cpp \
    dialogShow.cpp \
    dialogTimes.cpp \
    dialogMap.cpp \
    dialogZone.cpp \
    dialogTicket.cpp

HEADERS += \
    mainwindow.h \
    connectdtabase.h \
    dialogHallType.h \
    dialogBldg.h \
    dialogShow.h \
    dialogTimes.h \
    dialogMap.h \
    dialogZone.h \
    dialogTicket.h

