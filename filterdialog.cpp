#include <QDebug>
#include <QPushButton>
#include "filterdialog.h"
#include "ui_filterdialog.h"

namespace Farman
{

FilterDialog::FilterDialog(AttrFilterFlags attrFilterFlags,  const QStringList& nameMaskFilters, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterDialog),
    m_attrFilterFlags(attrFilterFlags),
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

    QString nameMaskFilterString = nameMaskFilters.join(' ');

    ui->nameMaskFilterLineEdit->setText(nameMaskFilterString);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(valicationFilterString(nameMaskFilterString));
}

FilterDialog::~FilterDialog()
{
    delete ui;
}

AttrFilterFlags FilterDialog::getAttrFilterFlags()
{
    return m_attrFilterFlags;
}

QStringList FilterDialog::getNameMaskFilters()
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
