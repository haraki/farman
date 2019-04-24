﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <qdir.h>
#include "types.h"
#include "doublefolderpanel.h"

class QStringList;

namespace Ui {
class MainWindow;
}

namespace Farman
{

class File;
class ViewerDispatcher;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow() Q_DECL_OVERRIDE;

public Q_SLOTS:
    void onOpenFile(ViewerType viewerType);
    void onOpenFile(const QString& path, ViewerType viewerType = ViewerType::Auto);
    void onCloseViewer(const QString& viewerName);
    void onOpenWithApp(const QString& path);
    void onOpenWithTextEditor(const QString& dirPath, const QStringList& filePaths);
    void onStatusChanged(const QString& statusString);
    void onCreateNewFileFinished(const QString& filePath);
    void onOutputConsole(const QString& consoleString);

private:
    void closeEvent(QCloseEvent* event) Q_DECL_OVERRIDE;

    void initFont();
    void initPalette();

    void updateSettings();

    void setVisibleConsole(bool visible);

    QDialog::DialogCode launchPreferencesDialog(PreferencesDialogTabPage page = PreferencesDialogTabPage::General);
    bool launchExternalApp(const QString& command, const QString dirPath);

    void about();

private Q_SLOTS:
    void on_actionOpen_triggered();
    void on_actionOpenWithTextViewer_triggered();
    void on_actionOpenWithHexViewer_triggered();
    void on_actionOpenWithImageViewer_triggered();
    void on_actionOpenWithApp_triggered();
    void on_actionOpenWithTextEditor_triggered();
    void on_actionCreateNewFile_triggered();
    void on_actionPreferences_triggered();
    void on_actionQuit_triggered();
    void on_actionSinglePane_triggered();
    void on_actionDualPane_triggered();
    void on_actionSortSettings_triggered();
    void on_actionFilterSettings_triggered();
    void on_actionCopy_triggered();
    void on_actionMove_triggered();
    void on_actionRemove_triggered();
    void on_actionMakeDirectory_triggered();
    void on_actionRename_triggered();
    void on_actionAttributes_triggered();
    void on_actionConsole_triggered(bool checked);
    void on_consoleDockWidget_visibilityChanged(bool visible);
    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;

    File* m_file;
    ViewerDispatcher* m_viewerDispatcher;

    QStringList m_nameFilters;
};

}           // namespace Farman

#endif // MAINWINDOW_H
