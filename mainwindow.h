#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qdir.h>
#include "types.h"
#include "doublefolderpanel.h"

class QStringList;

namespace Ui {
class MainWindow;
}

namespace Farman
{

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static void create();
    static MainWindow* getInstance();

public Q_SLOTS:
    void onOpen(ViewerType viewerType);
    void onOpen(const QString& path, ViewerType viewerType = ViewerType::Auto);
    void onCloseViewer(const QString& viewerName);
    void onOpenWithApp(const QString& path);
    void onStatusChanged(const QString& statusString);
    void onOutputConsole(const QString& consoleString);

private:
    explicit MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow();

    void initialize();
    void closeEvent(QCloseEvent* event) Q_DECL_OVERRIDE;

    void initFont();
    void initPalette();

    void updateSettings();
    void openCurrentFile(ViewerType viewerType = ViewerType::Auto);

private Q_SLOTS:
    void on_actionOpen_triggered();
    void on_actionOpenWithApp_triggered();
    void on_actionOpenWithTextViewer_triggered();
    void on_actionOpenWithHexViewer_triggered();
    void on_actionOpenWithImageViewer_triggered();
    void on_actionPreferences_triggered();
    void on_actionQuit_triggered();
    void on_actionSingleView_triggered();
    void on_actionDoubleView_triggered();
    void on_actionSortSettings_triggered();
    void on_actionFilterSettings_triggered();
    void on_actionCopy_triggered();
    void on_actionMove_triggered();
    void on_actionRemove_triggered();
    void on_actionMakeDirectory_triggered();
    void on_actionRename_triggered();
    void on_actionAttributes_triggered();

private:
    static MainWindow* s_instance;

    Ui::MainWindow *ui;

    QStringList m_nameFilters;
};

}           // namespace Farman

#endif // MAINWINDOW_H
