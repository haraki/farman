#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "folderview.h"
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

    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(onFocusChanged(QWidget*,QWidget*)));

    QString path = QDir::currentPath();
    qDebug() << path;

    createFolderPanel(DoubleFolderPanel::ViewMode::Double, path, QDir::AllEntries | QDir::NoSymLinks | QDir::AccessMask | QDir::NoDot, QDir::DirsFirst | QDir::IgnoreCase | QDir::Name);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onCurrentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo)
{
    qDebug() << "MainWindow::onCurrentChanged : old : " << oldFileInfo.filePath() << " new : " << newFileInfo.filePath();

    statusBar()->showMessage(newFileInfo.filePath());
}

void MainWindow::onFocusChanged(QWidget* oldWidget, QWidget* nowWidget)
{
    qDebug() << "MainWindow::onFocusChanged(" <<
                ((oldWidget == nullptr) ? "null" : oldWidget->objectName()) << ", " <<
                ((nowWidget == nullptr) ? "null" : nowWidget->objectName()) << ")";

    if(nowWidget != nullptr)
    {
        FolderView* newFolderView = dynamic_cast<FolderView*>(nowWidget);
        if(newFolderView != nullptr && newFolderView->objectName() == "folderView")
        {
            statusBar()->showMessage(newFolderView->currentPath());
        }
    }

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

    DoubleFolderPanel* doubleFolderPanel = new DoubleFolderPanel(viewMode, path, filterFlags, sortFlags, ui->mainWidget);

    ui->mainWidget->layout()->addWidget(doubleFolderPanel);

    connect(doubleFolderPanel,
            SIGNAL(currentChanged(QFileInfo,QFileInfo)),
            this,
            SLOT(onCurrentChanged(QFileInfo,QFileInfo)));
}
