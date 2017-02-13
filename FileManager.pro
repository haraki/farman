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
    folderview.cpp \
    folderviewstyleditemdelegate.cpp \
    foldermodel.cpp \
    doublefolderpanel.cpp \
    sortdialog.cpp \
    filterdialog.cpp \
    workingdialog.cpp

HEADERS  += mainwindow.h \
    folderform.h \
    folderviewstyleditemdelegate.h \
    foldermodel.h \
    folderview.h \
    doublefolderpanel.h \
    types.h \
    sortdialog.h \
    filterdialog.h \
    workingdialog.h

FORMS    += mainwindow.ui \
    folderform.ui \
    doublefolderpanel.ui \
    sortdialog.ui \
    filterdialog.ui \
    workingdialog.ui
