#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "foldermodel.h"
#include "folderform.h"
#include "dualpanelform.h"
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

    QVBoxLayout* vLayout = new QVBoxLayout(ui->mainWidget);
    vLayout->setSpacing(6);
    vLayout->setObjectName(QStringLiteral("vLayout"));
    vLayout->setContentsMargins(0, 0, 0, 0);

    if(0)
    {
        FolderForm* folderForm = new FolderForm(m_filterFlags, m_sortFlags, ui->mainWidget);
        folderForm->setObjectName(QStringLiteral("folderForm"));
        folderForm->setPath(path);

        vLayout->addWidget(folderForm);
    }
    else
    {
        DualPanelForm* dualPanelForm = new DualPanelForm(path, m_filterFlags, m_sortFlags, ui->mainWidget);

        vLayout->addWidget(dualPanelForm);
    }

    ui->mainWidget->setLayout(vLayout);
}

MainWindow::~MainWindow()
{
    delete ui;
}
