#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qdir.h>
#include "doublefolderpanel.h"

class QStringList;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow();

private slots:
    void on_actionSingleView_triggered();
    void on_actionDoubleView_triggered();

private:
    void createFolderPanel(DoubleFolderPanel::ViewMode viewMode, QString& path, QDir::Filters filterFlags, QDir::SortFlags sortFlags);

    Ui::MainWindow *ui;

    QStringList m_nameFilters;
};

#endif // MAINWINDOW_H
