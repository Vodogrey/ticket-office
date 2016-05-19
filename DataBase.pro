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
    dialogShow.cpp

HEADERS += \
    mainwindow.h \
    connectdtabase.h \
    dialogHallType.h \
    dialogBldg.h \
    dialogShow.h

