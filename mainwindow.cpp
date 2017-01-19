#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "foldermodel.h"
#include "folderform.h"
#include "singlefolderpanel.h"
#include "doublefolderpanel.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent/* = Q_NULLPTR*/)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_viewMode(ViewMode::Double)
    , m_nameFilters()
    , m_filterFlags(QDir::AllEntries | QDir::NoSymLinks | QDir::AccessMask | QDir::NoDot)
    , m_sortFlags(QDir::DirsFirst | QDir::IgnoreCase | QDir::Name)
{
    ui->setupUi(this);

    QString path = QDir::currentPath();
    qDebug() << path;

    createFolderPanel(m_viewMode, path, m_filterFlags, m_sortFlags);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionSingleView_triggered()
{
    qDebug() << "MainWindow::on_actionSingleView_triggered()";

    if(m_viewMode == ViewMode::Single)
    {
        // 既に単画面モードなので何もしない
        return;
    }

    QString         path        = QDir::currentPath();
    QDir::Filters   filterFlags = QDir::AllEntries | QDir::NoSymLinks | QDir::AccessMask | QDir::NoDot;
    QDir::SortFlags sortFlags   = QDir::DirsFirst | QDir::IgnoreCase | QDir::Name;

    destroyFolderPanel(path, filterFlags, sortFlags);

    qDebug() << "create SinglePanel";

    createFolderPanel(ViewMode::Single, path, filterFlags, sortFlags);
}

void MainWindow::on_actionDoubleView_triggered()
{
    qDebug() << "MainWindow::on_actionDoubleView_triggered()";

    if(m_viewMode == ViewMode::Double)
    {
        // 既に2画面モードなので何もしない
        return;
    }

    QString         path        = QDir::currentPath();
    QDir::Filters   filterFlags = QDir::AllEntries | QDir::NoSymLinks | QDir::AccessMask | QDir::NoDot;
    QDir::SortFlags sortFlags   = QDir::DirsFirst | QDir::IgnoreCase | QDir::Name;

    destroyFolderPanel(path, filterFlags, sortFlags);

    qDebug() << "create DoublePanel";

    createFolderPanel(ViewMode::Double, path, filterFlags, sortFlags);
}

void MainWindow::createFolderPanel(ViewMode viewMode, QString& path, QDir::Filters filterFlags, QDir::SortFlags sortFlags)
{
    QWidget* panelForm = nullptr;

    if(viewMode == ViewMode::Single)
    {
        panelForm = new SingleFolderPanel(path, filterFlags, sortFlags, ui->mainWidget);
    }
    else
    {
        panelForm = new DoubleFolderPanel(path, filterFlags, sortFlags, ui->mainWidget);
    }
    panelForm->setObjectName("FolderPanel");

    ui->mainWidget->layout()->addWidget(panelForm);

    m_viewMode = viewMode;
}

void MainWindow::destroyFolderPanel(QString& path, QDir::Filters& filterFlags, QDir::SortFlags& sortFlags)
{
    FolderPanelBase* folderPanel = ui->mainWidget->findChild<FolderPanelBase*>("FolderPanel");
    if(folderPanel != Q_NULLPTR)
    {
        qDebug() << "remove DoublePanel";

        path        = folderPanel->getPath();
        filterFlags = folderPanel->getFilterFlags();
        sortFlags   = folderPanel->getSortFlags();

        ui->mainWidget->layout()->removeWidget(folderPanel);
        delete folderPanel;
    }
}
