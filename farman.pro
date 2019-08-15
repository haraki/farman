#-------------------------------------------------
#
# Project created by QtCreator 2016-11-13T00:06:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = farman
TEMPLATE = app

VERSION = 0.5.0
QMAKE_TARGET_COMPANY = MASHSOFT
QMAKE_TARGET_PRODUCT = farman
QMAKE_TARGET_DESCRIPTION = 'farman' is Dual pane File manager.
QMAKE_TARGET_COPYRIGHT = Copyright 2019 MASHSOFT. All right reserved.

REVISION = $$system(git rev-parse HEAD)

DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += APP_REVISION=\\\"$$REVISION\\\"

INCLUDEPATH += ./QHexView/include

SOURCES += main.cpp\
    aboutdialog.cpp \
    bookmarkeditdialog.cpp \
    bookmarkinfomodel.cpp \
    bookmarkmanager.cpp \
    bookmarkmanagerdialog.cpp \
    historymanager.cpp \
    licensedialog.cpp \
    mainwindow.cpp \
    folderform.cpp \
    folderview.cpp \
    folderviewstyleditemdelegate.cpp \
    foldermodel.cpp \
    doublefolderpanel.cpp \
    selectbookmarkdialog.cpp \
    sortdialog.cpp \
    filterdialog.cpp \
    win32.cpp \
    workingdialog.cpp \
    worker.cpp \
    copyworker.cpp \
    overwritedialog.cpp \
    removeworker.cpp \
    renamedialog.cpp \
    settings.cpp \
    fileoperationdialog.cpp \
    fileattributesdialog.cpp \
    preferencesdialog.cpp \
    viewerdispatcher.cpp \
    readfileworker.cpp \
    viewerbase.cpp \
    imageviewer.cpp \
    textviewer.cpp \
    textview.cpp \
    QHexView/src/QHexView.cpp \
    hexviewer.cpp \
    file.cpp \
    xnix.cpp

HEADERS  += mainwindow.h \
    aboutdialog.h \
    bookmarkeditdialog.h \
    bookmarkinfo.h \
    bookmarkinfomodel.h \
    bookmarkmanager.h \
    bookmarkmanagerdialog.h \
    default_settings.h \
    folderform.h \
    folderviewstyleditemdelegate.h \
    foldermodel.h \
    folderview.h \
    doublefolderpanel.h \
    historymanager.h \
    licensedialog.h \
    misc.h \
    selectbookmarkdialog.h \
    types.h \
    sortdialog.h \
    filterdialog.h \
    win32.h \
    workingdialog.h \
    worker.h \
    copyworker.h \
    overwritedialog.h \
    removeworker.h \
    renamedialog.h \
    settings.h \
    fileoperationdialog.h \
    fileattributesdialog.h \
    preferencesdialog.h \
    workerresult.h \
    viewerdispatcher.h \
    readfileworker.h \
    viewerbase.h \
    imageviewer.h \
    textviewer.h \
    textview.h \
    QHexView/include/QHexView.h \
    hexviewer.h \
    file.h \
    xnix.h

FORMS    += mainwindow.ui \
    aboutdialog.ui \
    bookmarkeditdialog.ui \
    bookmarkmanagerdialog.ui \
    folderform.ui \
    doublefolderpanel.ui \
    licensedialog.ui \
    selectbookmarkdialog.ui \
    sortdialog.ui \
    filterdialog.ui \
    workingdialog.ui \
    overwritedialog.ui \
    renamedialog.ui \
    fileoperationdialog.ui \
    fileattributesdialog.ui \
    preferencesdialog.ui \
    imageviewer.ui \
    textviewer.ui \
    hexviewer.ui

RESOURCES += \
    resources.qrc
