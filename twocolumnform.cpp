#include <QVBoxLayout>
#include <qdir.h>
#include "twocolumnform.h"
#include "ui_twocolumnform.h"
#include "folderform.h"

TwoColumnForm::TwoColumnForm(QString& path, QDir::Filters filterFlags, QDir::SortFlags sortFlags, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TwoColumnForm)
{
    ui->setupUi(this);

    QVBoxLayout* l_vLayout = new QVBoxLayout(this);
    l_vLayout->setSpacing(6);
    l_vLayout->setObjectName(QStringLiteral("l_vLayout"));
    l_vLayout->setContentsMargins(0, 0, 0, 0);

    FolderForm* l_folderForm = new FolderForm(filterFlags, sortFlags, this);
    l_folderForm->setObjectName(QStringLiteral("l_folderForm"));
    l_folderForm->setPath(path);

    l_vLayout->addWidget(l_folderForm);

    ui->leftPanel->setLayout(l_vLayout);

    QVBoxLayout* r_vLayout = new QVBoxLayout(this);
    r_vLayout->setSpacing(6);
    r_vLayout->setObjectName(QStringLiteral("r_vLayout"));
    r_vLayout->setContentsMargins(0, 0, 0, 0);

    FolderForm* r_folderForm = new FolderForm(filterFlags, sortFlags, this);
    r_folderForm->setObjectName(QStringLiteral("r_folderForm"));
    r_folderForm->setPath(path);

    r_vLayout->addWidget(r_folderForm);

    ui->rightPanel->setLayout(r_vLayout);
}

TwoColumnForm::~TwoColumnForm()
{
    delete ui;
}
