#include "renamedialog.h"
#include "ui_renamedialog.h"

namespace Farman
{

RenameDialog::RenameDialog(const QString& currentName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RenameDialog),
    m_newName(currentName)
{
    ui->setupUi(this);

    ui->currentNameLabel->setText(currentName);
    ui->newNameLineEdit->setText(currentName);

    ui->newNameLineEdit->setFocus();
}

RenameDialog::~RenameDialog()
{
    delete ui;
}

QString RenameDialog::getNewName() const
{
    return m_newName;
}

void RenameDialog::accept()
{
    m_newName = ui->newNameLineEdit->text();

    QDialog::accept();
}

}           // namespace Farman
