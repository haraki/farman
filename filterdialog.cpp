#include <QDebug>
#include <QPushButton>
#include "filterdialog.h"
#include "ui_filterdialog.h"

namespace Farman
{

FilterDialog::FilterDialog(AttrFilterFlags attrFilterFlags,
                           FileFolderFilterType fileFolderFilterType,
                           const QStringList& nameMaskFilters,
                           QWidget *parent/* = Q_NULLPTR*/) :
    QDialog(parent),
    ui(new Ui::FilterDialog),
    m_attrFilterFlags(attrFilterFlags),
    m_fileFolderFilterType(fileFolderFilterType),
    m_nameMaskFilters(nameMaskFilters)
{
    ui->setupUi(this);

    if(attrFilterFlags & AttrFilterFlag::Hidden)
    {
        ui->showHiddenCheckBox->setChecked(true);
    }
#ifdef Q_OS_WIN
    if(attrFilterFlags & AttrFilterFlag::System)
    {
        ui->showSystemCheckBox->setChecked(true);
    }
#else
    ui->showSystemCheckBox->setVisible(false);
#endif

    if(fileFolderFilterType == FileFolderFilterType::File)
    {
        ui->showFileOnlyRadioButton->setChecked(true);
    }
    else if(fileFolderFilterType == FileFolderFilterType::Folder)
    {
        ui->showFolderOnlyRadioButton->setChecked(true);
    }
    else
    {
        ui->showAllRadioButton->setChecked(true);
    }

    QString nameMaskFilterString = nameMaskFilters.join(' ');

    ui->nameMaskFilterLineEdit->setText(nameMaskFilterString);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(valicationFilterString(nameMaskFilterString));
}

FilterDialog::~FilterDialog()
{
    delete ui;
}

AttrFilterFlags FilterDialog::getAttrFilterFlags() const
{
    return m_attrFilterFlags;
}

FileFolderFilterType FilterDialog::getFileFolderFilterType() const
{
    return m_fileFolderFilterType;
}

QStringList FilterDialog::getNameMaskFilters() const
{
    return m_nameMaskFilters;
}

void FilterDialog::accept()
{
    m_attrFilterFlags = AttrFilterFlag::None;
    if(ui->showHiddenCheckBox->isChecked())
    {
        m_attrFilterFlags |= AttrFilterFlag::Hidden;
    }
#ifdef Q_OS_WIN
    if(ui->showSystemCheckBox->isChecked())
    {
        m_attrFilterFlags |= AttrFilterFlag::System;
    }
#endif

    m_fileFolderFilterType = FileFolderFilterType::All;
    if(ui->showFileOnlyRadioButton->isChecked())
    {
        m_fileFolderFilterType = FileFolderFilterType::File;
    }
    else if(ui->showFolderOnlyRadioButton->isChecked())
    {
        m_fileFolderFilterType = FileFolderFilterType::Folder;
    }
    else
    {
        m_fileFolderFilterType = FileFolderFilterType::All;
    }

    m_nameMaskFilters = ui->nameMaskFilterLineEdit->text().simplified().split(' ', QString::SkipEmptyParts);
    if(m_nameMaskFilters.isEmpty())
    {
        m_nameMaskFilters = QStringList("*");
    }

    qDebug() << "attrFilterFlags : " << m_attrFilterFlags << ", nameMaskFilters : " << m_nameMaskFilters;

    QDialog::accept();
}

void FilterDialog::on_nameMaskFilterLineEdit_textChanged(const QString &arg1)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(valicationFilterString(arg1));
}

bool FilterDialog::valicationFilterString(const QString &nameMaskFilterString)
{
    QStringList nameMaskFilters = nameMaskFilterString.simplified().split(' ', QString::SkipEmptyParts);
    for(auto filter : nameMaskFilters)
    {
        if(filter == "." || filter == "..")
        {
            return false;
        }
    }

    return true;
}

}
