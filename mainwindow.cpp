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
    , m_nameFilters()
    , m_filterFlags(QDir::AllEntries | QDir::NoSymLinks | QDir::AccessMask | QDir::NoDot)
    , m_sortFlags(QDir::DirsFirst | QDir::IgnoreCase | QDir::Name)
{
    ui->setupUi(this);

    QString path = QDir::currentPath();
    qDebug() << path;

    QWidget* panelForm = nullptr;

    if(0)
    {
        panelForm = new SingleFolderPanel(path, m_filterFlags, m_sortFlags, ui->mainWidget);
    }
    else
    {
        panelForm = new DoubleFolderPanel(path, m_filterFlags, m_sortFlags, ui->mainWidget);
    }

    ui->mainWidget->layout()->addWidget(panelForm);
}

MainWindow::~MainWindow()
{
    delete ui;
}
