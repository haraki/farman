#include "overwritedialog.h"
#include "ui_overwritedialog.h"

namespace Farman
{

OverwriteDialog::OverwriteDialog(OverwriteMethodType methodType, const QString& renameText, QWidget *parent/* = Q_NULLPTR*/)
    : QDialog(parent)
    , ui(new Ui::OverwriteDialog)
    , m_methodType(methodType)
    , m_renameFileName(renameText)
{
    ui->setupUi(this);

    switch(methodType)
    {
    case OverwriteMethodType::Overwrite:
        ui->methodOverwriteRadioButton->setChecked(true);
        break;
    case OverwriteMethodType::OverwriteIfNewer:
        ui->methodOverwriteIfNewerRadioButton->setChecked(true);
        break;
    case OverwriteMethodType::Skip:
        ui->methodSkipRadioButton->setChecked(true);
        break;
    case OverwriteMethodType::Rename:
        ui->methodRenameRadioButton->setChecked(true);
        break;
    default:
        ui->methodOverwriteRadioButton->setChecked(true);
        break;
    }

    ui->renameLineEdit->setText(renameText);
}

OverwriteDialog::~OverwriteDialog()
{
    delete ui;
}

OverwriteMethodType OverwriteDialog::getMethodType() const
{
    return m_methodType;
}

QString OverwriteDialog::getRenameFileName() const
{
    return m_renameFileName;
}

void OverwriteDialog::accept()
{
    m_methodType = OverwriteMethodType::Overwrite;

    if(ui->methodOverwriteRadioButton->isChecked())
    {
        m_methodType = OverwriteMethodType::Overwrite;
        m_renameFileName = "";
    }
    else if(ui->methodOverwriteIfNewerRadioButton->isChecked())
    {
        m_methodType = OverwriteMethodType::OverwriteIfNewer;
        m_renameFileName = "";
    }
    else if(ui->methodSkipRadioButton->isChecked())
    {
        m_methodType = OverwriteMethodType::Skip;
        m_renameFileName = "";
    }
    else if(ui->methodRenameRadioButton->isChecked())
    {
        m_methodType = OverwriteMethodType::Rename;
        m_renameFileName = ui->renameLineEdit->text();
    }

    QDialog::accept();
}

void OverwriteDialog::on_methodOverwriteRadioButton_clicked()
{
    ui->renameLineEdit->setEnabled(false);
}

void OverwriteDialog::on_methodOverwriteIfNewerRadioButton_clicked()
{
    ui->renameLineEdit->setEnabled(false);
}

void OverwriteDialog::on_methodSkipRadioButton_clicked()
{
    ui->renameLineEdit->setEnabled(false);
}

void OverwriteDialog::on_methodRenameRadioButton_clicked()
{
    ui->renameLineEdit->setEnabled(true);
}

}           // namespace Farman
