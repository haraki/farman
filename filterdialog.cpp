#include <QDebug>
#include "filterdialog.h"
#include "ui_filterdialog.h"

namespace Farman
{

FilterDialog::FilterDialog(FilterFlags filterFlags,  const QStringList& nameMaskFilters, QWidget *parent) :
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

    ui->nameMaskFilterLineEdit->setText(nameMaskFilters.join(' '));
}

FilterDialog::~FilterDialog()
{
    delete ui;
}

FilterFlags FilterDialog::getFilterFlags()
{
    return m_filterFlags;
}

QStringList FilterDialog::getNameMaskFilters()
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
    m_nameMaskFilters = ui->nameMaskFilterLineEdit->text().simplified().split(' ', QString::SkipEmptyParts);
    if(m_nameMaskFilters.isEmpty())
    {
        m_nameMaskFilters = QStringList("*");
    }

    qDebug() << "filterFlags : " << m_filterFlags << ", nameMaskFilters : " << m_nameMaskFilters;

    QDialog::accept();
}

}
