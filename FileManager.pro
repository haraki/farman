#-------------------------------------------------
#
# Project created by QtCreator 2016-11-13T00:06:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_MAC_SDK = macosx10.12

TARGET = FileManager
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    folderform.cpp \
    twocolumnform.cpp \
    foldermodel.cpp \
    folderview.cpp

HEADERS  += mainwindow.h \
    folderform.h \
    twocolumnform.h \
    foldermodel.h \
    folderview.h

FORMS    += mainwindow.ui \
    folderform.ui \
    twocolumnform.ui
