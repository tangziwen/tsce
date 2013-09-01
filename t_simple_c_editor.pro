TEMPLATE = app
QT       += core gui
QT += widgets
CONFIG += console
RESOURCES     = application.qrc
SOURCES += main.cpp \
    codeeditor.cpp \
    mainwindow.cpp \
    tabbar.cpp \
    highlighter.cpp

HEADERS += \
    codeeditor.h \
    mainwindow.h \
    tabbar.h \
    highlighter.h

