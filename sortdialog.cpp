#include <QDebug>
#include "sortdialog.h"
#include "ui_sortdialog.h"

SortDialog::SortDialog(QDir::SortFlags sortFlags, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SortDialog),
    m_sortFlags(sortFlags)
{
    ui->setupUi(this);

    if(sortFlags & QDir::SortFlag::Time)
    {
        ui->lastModifiedRadioButton->setChecked(true);
    }
    else if(sortFlags & QDir::SortFlag::Size)
    {
        ui->sizeRadioButton->setChecked(true);
    }
    else if(sortFlags & QDir::SortFlag::Type)
    {
        ui->typeRadioButton->setChecked(true);
    }
    else
    {
        ui->nameRadioButton->setChecked(true);
    }

    if(sortFlags & QDir::SortFlag::Reversed)
    {
        ui->descendingRadioButton->setChecked(true);
    }
    else
    {
        ui->ascendingRadioButton->setChecked(true);
    }

    if(sortFlags & QDir::SortFlag::DirsFirst)
    {
        ui->dirFirstRadioButton->setChecked(true);
    }
    else if(sortFlags & QDir::SortFlag::DirsLast)
    {
        ui->dirLastRadioButton->setChecked(true);
    }
    else
    {
        ui->dirNoSpecifyRadioButton->setChecked(true);
    }

    if(sortFlags & QDir::SortFlag::IgnoreCase)
    {
        ui->caseInsensitiveRadioButton->setChecked(true);
    }
    else
    {
        ui->caseSensitiveRadioButton->setChecked(true);
    }
}

SortDialog::~SortDialog()
{
    delete ui;
}

QDir::SortFlags SortDialog::getSortFlags()
{
    return m_sortFlags;
}

void SortDialog::accept()
{
    m_sortFlags &= ~(QDir::SortFlag::Time | QDir::SortFlag::Size | QDir::SortFlag::Type);
    if(ui->lastModifiedRadioButton->isChecked())
    {
        m_sortFlags |= QDir::SortFlag::Time;
    }
    else if(ui->sizeRadioButton->isChecked())
    {
        m_sortFlags |= QDir::SortFlag::Size;
    }
    else if(ui->typeRadioButton->isChecked())
    {
        m_sortFlags |= QDir::SortFlag::Type;
    }
    else
    {
        m_sortFlags |= QDir::SortFlag::Name;
    }

    m_sortFlags &= ~(QDir::SortFlag::Reversed);
    if(ui->descendingRadioButton->isChecked())
    {
        m_sortFlags |= QDir::SortFlag::Reversed;
    }

    m_sortFlags &= ~(QDir::SortFlag::DirsFirst | QDir::SortFlag::DirsLast);
    if(ui->dirFirstRadioButton->isChecked())
    {
        m_sortFlags |= QDir::SortFlag::DirsFirst;
    }
    else if(ui->dirLastRadioButton->isChecked())
    {
        m_sortFlags |= QDir::SortFlag::DirsLast;
    }

    m_sortFlags &= ~QDir::SortFlag::IgnoreCase;
    if(ui->caseInsensitiveRadioButton->isChecked())
    {
        m_sortFlags |= QDir::SortFlag::IgnoreCase;
    }

    qDebug() << "sortFlag == " << m_sortFlags;

    QDialog::accept();
}
