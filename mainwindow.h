#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qdir.h>

class QStringList;

namespace Ui {
class MainWindow;
}

class FolderForm;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    FolderForm* m_folderForm;

    QStringList m_nameFilters;
    QDir::Filters m_filterFlags;
    QDir::SortFlags m_sortFlags;
};

#endif // MAINWINDOW_H
