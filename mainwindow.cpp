#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "folderview.h"
#include "foldermodel.h"
#include "folderform.h"
#include "doublefolderpanel.h"
#include <QDebug>

using namespace Farman;

MainWindow::MainWindow(QWidget *parent/* = Q_NULLPTR*/)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_nameFilters()
{
    ui->setupUi(this);

    QString path = QDir::currentPath();
    qDebug() << path;

    DoubleFolderPanel* doubleFolderPanel = new DoubleFolderPanel(ViewMode::Double,
                                                                 path, QDir::AllEntries | QDir::NoSymLinks | QDir::AccessMask | QDir::NoDot, QDir::DirsFirst | QDir::IgnoreCase | QDir::Name,
                                                                 path, QDir::AllEntries | QDir::NoSymLinks | QDir::AccessMask | QDir::NoDot, QDir::DirsFirst | QDir::IgnoreCase | QDir::Name,
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

    DoubleFolderPanel* folderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    folderPanel->changeViewMode(ViewMode::Single);
}

void MainWindow::on_actionDoubleView_triggered()
{
    qDebug() << "MainWindow::on_actionDoubleView_triggered()";

    DoubleFolderPanel* folderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    folderPanel->changeViewMode(ViewMode::Double);
}
