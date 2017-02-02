#include <QDebug>
#include "filterdialog.h"
#include "ui_filterdialog.h"

namespace Farman
{

FilterDialog::FilterDialog(QDir::Filters filterFlags, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterDialog),
    m_filterFlags(filterFlags)
{
    ui->setupUi(this);

    if(filterFlags & QDir::Filter::Hidden)
    {
        ui->showHiddenCheckBox->setChecked(true);
    }

    if(filterFlags & QDir::Filter::System)
    {
        ui->showSystemCheckBox->setChecked(true);
    }
}

FilterDialog::~FilterDialog()
{
    delete ui;
}

QDir::Filters FilterDialog::getFilterFlags()
{
    return m_filterFlags;
}

void FilterDialog::accept()
{
    m_filterFlags &= ~(QDir::Filter::Hidden | QDir::Filter::System);
    if(ui->showHiddenCheckBox->isChecked())
    {
        m_filterFlags |= QDir::Filter::Hidden;
    }

    if(ui->showSystemCheckBox->isChecked())
    {
        m_filterFlags |= QDir::Filter::System;
    }

    qDebug() << "filterFlags == " << m_filterFlags;

    QDialog::accept();
}

}
