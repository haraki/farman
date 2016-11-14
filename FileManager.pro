#-------------------------------------------------
#
# Project created by QtCreator 2016-11-13T00:06:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FileManager
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    forderform.cpp \
    twocolumnform.cpp

HEADERS  += mainwindow.h \
    forderform.h \
    twocolumnform.h

FORMS    += mainwindow.ui \
    forderform.ui \
    twocolumnform.ui
