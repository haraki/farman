#-------------------------------------------------
#
# Project created by QtCreator 2016-11-13T00:06:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = farman
TEMPLATE = app
INCLUDEPATH += ./QHexView/include

SOURCES += main.cpp\
        mainwindow.cpp \
    folderform.cpp \
    folderview.cpp \
    folderviewstyleditemdelegate.cpp \
    foldermodel.cpp \
    doublefolderpanel.cpp \
    sortdialog.cpp \
    filterdialog.cpp \
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
    file.cpp

HEADERS  += mainwindow.h \
    folderform.h \
    folderviewstyleditemdelegate.h \
    foldermodel.h \
    folderview.h \
    doublefolderpanel.h \
    types.h \
    sortdialog.h \
    filterdialog.h \
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
    file.h

FORMS    += mainwindow.ui \
    folderform.ui \
    doublefolderpanel.ui \
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