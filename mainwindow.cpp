#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "foldermodel.h"
#include "folderform.h"
#include "doublefolderpanel.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent/* = Q_NULLPTR*/)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_nameFilters()
{
    ui->setupUi(this);

    QString path = QDir::currentPath();
    qDebug() << path;

    createFolderPanel(DoubleFolderPanel::ViewMode::Double, path, QDir::AllEntries | QDir::NoSymLinks | QDir::AccessMask | QDir::NoDot, QDir::DirsFirst | QDir::IgnoreCase | QDir::Name);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionSingleView_triggered()
{
    qDebug() << "MainWindow::on_actionSingleView_triggered()";

    DoubleFolderPanel* folderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    folderPanel->changeViewMode(DoubleFolderPanel::ViewMode::Single);
}

void MainWindow::on_actionDoubleView_triggered()
{
    qDebug() << "MainWindow::on_actionDoubleView_triggered()";

    DoubleFolderPanel* folderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    folderPanel->changeViewMode(DoubleFolderPanel::ViewMode::Double);
}

void MainWindow::createFolderPanel(DoubleFolderPanel::ViewMode viewMode, QString& path, QDir::Filters filterFlags, QDir::SortFlags sortFlags)
{
    qDebug() << "create DoubleFolderPanel";

    QWidget* panelForm = new DoubleFolderPanel(viewMode, path, filterFlags, sortFlags, ui->mainWidget);

    ui->mainWidget->layout()->addWidget(panelForm);
}
