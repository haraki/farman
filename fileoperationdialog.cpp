#include <QStandardItemModel>
#include <QFileDialog>
#include "fileoperationdialog.h"
#include "ui_fileoperationdialog.h"

namespace Farman
{

FileOperationDialog::FileOperationDialog(OperationType type,
                                         const QString& srcDirPath,
                                         const QList<QFileInfo>& srcFileInfos,
                                         const QString& dstDirPath,
                                         QWidget *parent/* = Q_NULLPTR*/) :
    QDialog(parent),
    ui(new Ui::FileOperationDialog)
{
    ui->setupUi(this);

    this->setWindowTitle((type == OperationType::Copy) ? tr("Copy file(s)") : (type == OperationType::Move) ? tr("Move file(s)") : tr("Delete file(s)"));

    ui->srcFolderPathLabel->setText(srcDirPath);

    QStandardItemModel* model = new QStandardItemModel();
    foreach(auto &fileInfo, srcFileInfos)
    {
        QStandardItem* item = new QStandardItem();
        QString fileName = fileInfo.fileName();
        if(fileInfo.isDir())
        {
            fileName += " <Folder>";
        }
        item->setText(fileName);
        item->setEditable(false);

        model->appendRow(item);
    }
    ui->srcFileNamesListView->setModel(model);

    if(type == OperationType::Remove)
    {
        ui->dstLabel->setVisible(false);
        ui->dstFolderPathEdit->setVisible(false);
        ui->dstFolderPathSelectButton->setVisible(false);
    }
    else
    {
        ui->dstLabel->setVisible(true);
        ui->dstFolderPathEdit->setVisible(true);
        ui->dstFolderPathSelectButton->setVisible(true);

        ui->dstFolderPathEdit->setText(dstDirPath);
    }
}

FileOperationDialog::~FileOperationDialog()
{
    delete ui;
}

QString FileOperationDialog::getDstDirPath() const
{
    return ui->dstFolderPathEdit->text();
}

void FileOperationDialog::on_dstFolderPathSelectButton_clicked()
{
    QString dirPath = QFileDialog::getExistingDirectory(this,
                                                        tr("Choose folder."),
                                                        getDstDirPath(),
                                                        QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly);

    if(!dirPath.isEmpty())
    {
        ui->dstFolderPathEdit->setText(dirPath);
    }
}

}           // namespace Farman
