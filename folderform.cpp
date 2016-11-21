#include <QDebug>
#include <QResizeEvent>
#include "folderform.h"
#include "ui_folderform.h"
#include "foldermodel.h"

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

void FolderForm::setModel(FolderModel *model)
{
    m_folderModel = model;
    ui->folderView->setModel(model);
}

void FolderForm::setPath(QString& path)
{
    if(m_folderModel != nullptr)
    {
        QModelIndex index = m_folderModel->index(path);
        ui->folderView->setRootIndex(index);

        ui->lineEdit->setText(path);
    }
}

void FolderForm::resizeEvent(QResizeEvent *event)
{
    QSize diff = event->size() - event->oldSize();

    qDebug() << "<<<< RESIZE >>>> " << event->size();

    ui->folderView->setColumnWidth(0, event->size().width() - ui->folderView->columnWidth(1) - ui->folderView->columnWidth(2) - ui->folderView->columnWidth(3));
}
