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
    folderform.cpp \
    twocolumnform.cpp \
    foldermodel.cpp

HEADERS  += mainwindow.h \
    folderform.h \
    twocolumnform.h \
    foldermodel.h

FORMS    += mainwindow.ui \
    folderform.ui \
    twocolumnform.ui
