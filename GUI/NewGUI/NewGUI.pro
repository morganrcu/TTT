#-------------------------------------------------
#
# Project created by QtCreator 2013-12-14T13:21:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NewGUI
TEMPLATE = app

INCLUDEPATH += "/usr/local/include/vtk-6.1"


SOURCES += main.cpp\
        tttmainwindow.cpp \
    tttprojectform.cpp \
    tttpreferences.cpp \
    tttexplorer.cpp

HEADERS  += tttmainwindow.h \
    tttprojectform.h \
    tttpreferences.h \
    tttexplorer.h

FORMS    += tttmainwindow.ui \
    tttprojectform.ui \
    tttpreferences.ui \
    tttexplorer.ui

LIBS+=vtkViewsQt
