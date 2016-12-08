#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "foldermodel.h"
#include "folderform.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_nameFilters(),
    m_filterFlags(QDir::AllEntries | QDir::NoSymLinks | QDir::AccessMask | QDir::NoDot),
    m_sortFlags(QDir::DirsFirst | QDir::IgnoreCase | QDir::Name)
{
    ui->setupUi(this);

    QString path = QDir::currentPath();
    qDebug() << path;

    QVBoxLayout* vLayout = new QVBoxLayout(ui->mainWidget);
    vLayout->setSpacing(6);
    vLayout->setObjectName(QStringLiteral("vLayout"));
    vLayout->setContentsMargins(0, 0, 0, 0);

    m_folderForm = new FolderForm(m_filterFlags, m_sortFlags, ui->mainWidget);
    m_folderForm->setObjectName(QStringLiteral("folderForm"));

    m_folderForm->setPath(path);

    vLayout->addWidget(m_folderForm);

    ui->mainWidget->setLayout(vLayout);
}

MainWindow::~MainWindow()
{
    delete ui;
}
