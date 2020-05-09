#include <QDebug>
#include <QPushButton>
#include "filterdialog.h"
#include "ui_filterdialog.h"

namespace Farman
{

FilterDialog::FilterDialog(FilterFlags filterFlags,
                           const QStringList& nameMaskFilters,
                           QWidget *parent/* = Q_NULLPTR*/) :
    QDialog(parent),
    ui(new Ui::FilterDialog),
    m_filterFlags(filterFlags),
    m_nameMaskFilters(nameMaskFilters)
{
    ui->setupUi(this);

    if(filterFlags & FilterFlag::Hidden)
    {
        ui->showHiddenCheckBox->setChecked(true);
    }
#ifdef Q_OS_WIN
    if(filterFlags & FilterFlag::System)
    {
        ui->showSystemCheckBox->setChecked(true);
    }
#else
    ui->showSystemCheckBox->setVisible(false);
#endif

    if(filterFlags & FilterFlag::Files && !(filterFlags & FilterFlag::Dirs))
    {
        ui->showFileOnlyRadioButton->setChecked(true);
    }
    else if(filterFlags & FilterFlag::Dirs && !(filterFlags & FilterFlag::Files))
    {
        ui->showFileOnlyRadioButton->setChecked(true);
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

FilterFlags FilterDialog::getFilterFlags() const
{
    return m_filterFlags;
}

QStringList FilterDialog::getNameMaskFilters() const
{
    return m_nameMaskFilters;
}

void FilterDialog::accept()
{
    m_filterFlags = FilterFlag::None;
    if(ui->showHiddenCheckBox->isChecked())
    {
        m_filterFlags |= FilterFlag::Hidden;
    }
#ifdef Q_OS_WIN
    if(ui->showSystemCheckBox->isChecked())
    {
        m_filterFlags |= FilterFlag::System;
    }
#endif

    if(ui->showFileOnlyRadioButton->isChecked())
    {
        m_filterFlags |= FilterFlag::Files;
    }
    else if(ui->showFolderOnlyRadioButton->isChecked())
    {
        m_filterFlags |= FilterFlag::Dirs;
    }
    else
    {
        m_filterFlags |= (FilterFlag::Files | FilterFlag::Dirs);
    }

    m_nameMaskFilters = ui->nameMaskFilterLineEdit->text().simplified().split(' ', QString::SkipEmptyParts);
    if(m_nameMaskFilters.isEmpty())
    {
        m_nameMaskFilters = QStringList("*");
    }

    qDebug() << "filterFlags : " << m_filterFlags << ", nameMaskFilters : " << m_nameMaskFilters;

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
