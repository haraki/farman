#include <QDebug>
#include "filterdialog.h"
#include "ui_filterdialog.h"

namespace Farman
{

FilterDialog::FilterDialog(FilterFlags filterFlags, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterDialog),
    m_filterFlags(filterFlags)
{
    ui->setupUi(this);

    if(filterFlags & FilterFlag::Hidden)
    {
        ui->showHiddenCheckBox->setChecked(true);
    }

    if(filterFlags & FilterFlag::System)
    {
        ui->showSystemCheckBox->setChecked(true);
    }
}

FilterDialog::~FilterDialog()
{
    delete ui;
}

FilterFlags FilterDialog::getFilterFlags()
{
    return m_filterFlags;
}

void FilterDialog::accept()
{
    m_filterFlags &= ~(FilterFlag::Hidden | FilterFlag::System);
    if(ui->showHiddenCheckBox->isChecked())
    {
        m_filterFlags |= FilterFlag::Hidden;
    }

    if(ui->showSystemCheckBox->isChecked())
    {
        m_filterFlags |= FilterFlag::System;
    }

    qDebug() << "filterFlags == " << m_filterFlags;

    QDialog::accept();
}

}
