#-------------------------------------------------
#
# Project created by QtCreator 2016-01-22T11:27:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = graphmaker
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    graph.cpp \
    vertex.cpp \
    line.cpp \
    createnewgraphdialog.cpp \
    propertieswindow.cpp

HEADERS  += mainwindow.h \
    graph.h \
    vertex.h \
    line.h \
    createnewgraphdialog.h \
    propertieswindow.h

RESOURCES += \
    resources.qrc
