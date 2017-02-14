#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "folderview.h"
#include "foldermodel.h"
#include "folderform.h"
#include "doublefolderpanel.h"
#include "sortdialog.h"
#include "filterdialog.h"

namespace Farman
{

MainWindow::MainWindow(QWidget *parent/* = Q_NULLPTR*/)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_nameFilters()
{
    ui->setupUi(this);

    QString path = QDir::currentPath();
    qDebug() << path;

    DoubleFolderPanel* doubleFolderPanel = new DoubleFolderPanel(ViewMode::Double,
                                                                 path, DEFAULT_FILTER_FLAGS, DEFAULT_SORT_FLAGS,
                                                                 path, DEFAULT_FILTER_FLAGS, DEFAULT_SORT_FLAGS,
                                                                 ui->mainWidget);

    ui->mainWidget->layout()->addWidget(doubleFolderPanel);

    connect(doubleFolderPanel,
            SIGNAL(statusChanged(const QString)),
            this,
            SLOT(onStatusChanged(const QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onStatusChanged(const QString& statusString)
{
    qDebug() << "MainWindow::onStatusChanged : " << statusString;

    statusBar()->showMessage(statusString);
}

void MainWindow::on_actionSingleView_triggered()
{
    qDebug() << "MainWindow::on_actionSingleView_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    if(doubleFolderPanel != Q_NULLPTR)
    {
        doubleFolderPanel->changeViewMode(ViewMode::Single);
    }
}

void MainWindow::on_actionDoubleView_triggered()
{
    qDebug() << "MainWindow::on_actionDoubleView_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    if(doubleFolderPanel != Q_NULLPTR)
    {
        doubleFolderPanel->changeViewMode(ViewMode::Double);
    }
}

void Farman::MainWindow::on_actionSortSettings_triggered()
{
    qDebug() << "MainWindow::on_actionSortSetting_triggered()";

    QDir::SortFlags sortFlags = DEFAULT_SORT_FLAGS;

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    if(doubleFolderPanel != Q_NULLPTR)
    {
        FolderForm* activeFolderForm = doubleFolderPanel->getActiveFolderForm();
        if(activeFolderForm != Q_NULLPTR)
        {
            sortFlags = activeFolderForm->getSortFlags();

            SortDialog dialog(sortFlags, this);
            if(dialog.exec())
            {
                sortFlags = dialog.getSortFlags();
                activeFolderForm->setSortFlags(sortFlags);
            }
        }
    }
}

void MainWindow::on_actionFilterSettings_triggered()
{
    qDebug() << "MainWindow::on_actionFilterSettings_triggered()";

    QDir::Filters filterFlags = DEFAULT_FILTER_FLAGS;

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    if(doubleFolderPanel != Q_NULLPTR)
    {
        FolderForm* activeFolderForm = doubleFolderPanel->getActiveFolderForm();
        if(activeFolderForm != Q_NULLPTR)
        {
            filterFlags = activeFolderForm->getFilterFlags();

            FilterDialog dialog(filterFlags, this);
            if(dialog.exec())
            {
                filterFlags = dialog.getFilterFlags();
                activeFolderForm->setFilterFlags(filterFlags);
            }
        }
    }
}

void MainWindow::on_actionCopy_triggered()
{
    qDebug() << "MainWindow::on_actionCopy_triggered()";


}

}           // namespace Farman
