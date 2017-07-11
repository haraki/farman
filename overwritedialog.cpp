#include <QDateTime>
#include <QFileInfo>
#include "overwritedialog.h"
#include "ui_overwritedialog.h"

namespace Farman
{

OverwriteDialog::OverwriteDialog(const QString& srcFilePath,
                                 const QString& dstFilePath,
                                 OverwriteMethodType methodType,
                                 QWidget *parent/* = Q_NULLPTR*/)
    : QDialog(parent)
    , ui(new Ui::OverwriteDialog)
    , m_methodType(methodType)
    , m_renameFileName("")
    , m_keepSetting(false)
{
    ui->setupUi(this);

    QFileInfo srcFileInfo(srcFilePath);
    QFileInfo dstFileInfo(dstFilePath);

    m_renameFileName = dstFileInfo.fileName();

    ui->srcFilePathLabel->setText(QString("Src: ") + srcFileInfo.absoluteFilePath());
    ui->srcInfoLabel->setText(QString("Size: ") + QString("%1").arg(srcFileInfo.size()) + QString(" Byte(s), Last modified: ") + srcFileInfo.lastModified().toString());
    ui->dstFilePathLabel->setText(QString("Dst: ") + dstFileInfo.absoluteFilePath());
    ui->dstInfoLabel->setText(QString("Size: ") + QString("%1").arg(dstFileInfo.size()) + QString(" Byte(s), Last modified: ") + dstFileInfo.lastModified().toString());

    switch(methodType)
    {
    case OverwriteMethodType::Overwrite:
        ui->methodOverwriteRadioButton->setChecked(true);
        ui->keeySetting->setEnabled(true);
        ui->renameLineEdit->setEnabled(false);
        break;
    case OverwriteMethodType::OverwriteIfNewer:
        ui->methodOverwriteIfNewerRadioButton->setChecked(true);
        ui->keeySetting->setEnabled(true);
        ui->renameLineEdit->setEnabled(false);
        break;
    case OverwriteMethodType::Skip:
        ui->methodSkipRadioButton->setChecked(true);
        ui->keeySetting->setEnabled(true);
        ui->renameLineEdit->setEnabled(false);
        break;
    case OverwriteMethodType::Rename:
        ui->methodRenameRadioButton->setChecked(true);
        ui->keeySetting->setEnabled(false);
        ui->renameLineEdit->setEnabled(true);
        break;
    default:
        // ここに来ることはありえない
        break;
    }

    ui->renameLineEdit->setText(m_renameFileName);
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

bool OverwriteDialog::getKeepSetting() const
{
    return m_keepSetting;
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

    m_keepSetting = (ui->keeySetting->isEnabled() && ui->keeySetting->isChecked());

    QDialog::accept();
}

void OverwriteDialog::on_methodOverwriteRadioButton_clicked()
{
    ui->renameLineEdit->setEnabled(false);
    ui->keeySetting->setEnabled(true);
}

void OverwriteDialog::on_methodOverwriteIfNewerRadioButton_clicked()
{
    ui->renameLineEdit->setEnabled(false);
    ui->keeySetting->setEnabled(true);
}

void OverwriteDialog::on_methodSkipRadioButton_clicked()
{
    ui->renameLineEdit->setEnabled(false);
    ui->keeySetting->setEnabled(true);
}

void OverwriteDialog::on_methodRenameRadioButton_clicked()
{
    ui->renameLineEdit->setEnabled(true);
    ui->keeySetting->setEnabled(false);
}

}           // namespace Farman
