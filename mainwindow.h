#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qdir.h>
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
    explicit MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow();

private Q_SLOTS:
    void onStatusChanged(const QString& statusString);

    void on_actionSingleView_triggered();
    void on_actionDoubleView_triggered();
    void on_actionSortSettings_triggered();
    void on_actionFilterSettings_triggered();
    void on_actionCopy_triggered();

private:
    Ui::MainWindow *ui;

    QStringList m_nameFilters;
};

}           // namespace Farman

#endif // MAINWINDOW_H
