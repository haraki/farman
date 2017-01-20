#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qdir.h>

class QStringList;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum class ViewMode : int
    {
        Single = 0,
        Double = 1,

        ViewModeNum,
    };

    explicit MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow();

private slots:
    void on_actionSingleView_triggered();
    void on_actionDoubleView_triggered();

private:
    void createFolderPanel(ViewMode viewMode, QString& path, QDir::Filters filterFlags, QDir::SortFlags sortFlags);
    void destroyFolderPanel(QString& path, QDir::Filters& filterFlags, QDir::SortFlags& sortFlags);

    Ui::MainWindow *ui;

    ViewMode m_viewMode;

    QStringList m_nameFilters;
};

#endif // MAINWINDOW_H
