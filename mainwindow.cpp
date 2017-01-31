#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "folderview.h"
#include "foldermodel.h"
#include "folderform.h"
#include "doublefolderpanel.h"
#include <QDebug>

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
            SIGNAL(currentChanged(QFileInfo,QFileInfo)),
            this,
            SLOT(onCurrentChanged(QFileInfo,QFileInfo)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onCurrentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo)
{
    qDebug() << "MainWindow::onCurrentChanged : old : " << oldFileInfo.filePath() << " new : " << newFileInfo.filePath();

    statusBar()->showMessage(newFileInfo.absoluteFilePath());
}

void MainWindow::on_actionSingleView_triggered()
{
    qDebug() << "MainWindow::on_actionSingleView_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    doubleFolderPanel->changeViewMode(ViewMode::Single);
}

void MainWindow::on_actionDoubleView_triggered()
{
    qDebug() << "MainWindow::on_actionDoubleView_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    doubleFolderPanel->changeViewMode(ViewMode::Double);
}

}           // namespace Farman
