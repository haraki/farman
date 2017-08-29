#include "renamedialog.h"
#include "ui_renamedialog.h"

namespace Farman
{

RenameDialog::RenameDialog(const QString& currentFileName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RenameDialog)
{
    ui->setupUi(this);

    ui->currentFileNameFileNameLabel->setText(currentFileName);
    ui->newFileNameLineEdit->setText(currentFileName);

    ui->newFileNameLineEdit->setFocus();
}

RenameDialog::~RenameDialog()
{
    delete ui;
}

QString RenameDialog::getNewFileName() const
{
    return m_newFileName;
}

void RenameDialog::accept()
{
    m_newFileName = ui->newFileNameLineEdit->text();

    QDialog::accept();
}

}           // namespace Farman
