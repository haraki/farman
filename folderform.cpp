#include "folderform.h"
#include "ui_folderform.h"

FolderForm::FolderForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FolderForm)
{
    ui->setupUi(this);
}

FolderForm::~FolderForm()
{
    delete ui;
}

void FolderForm::setModel(QAbstractItemModel *model)
{
    ui->folderView->setModel(model);
}
