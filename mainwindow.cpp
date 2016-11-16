#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "foldermodel.h"
#include "folderform.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_folderModel = new FolderModel();

    QVBoxLayout* vLayout = new QVBoxLayout(ui->mainWidget);
    vLayout->setSpacing(6);
    vLayout->setObjectName(QStringLiteral("vLayout"));
    vLayout->setContentsMargins(0, 0, 0, 0);

    m_folderForm = new FolderForm(ui->mainWidget);
    m_folderForm->setObjectName(QStringLiteral("folderForm"));

    m_folderForm->setModel(m_folderModel);

    vLayout->addWidget(m_folderForm);

    setLayout(vLayout);
}

MainWindow::~MainWindow()
{
    delete ui;
}
