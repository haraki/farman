#include <QDebug>
#include "sortdialog.h"
#include "ui_sortdialog.h"

namespace Farman
{

SortDialog::SortDialog(SectionType sectionType,
                       SortDirsType dirsType,
                       bool dotFirst,
                       Qt::CaseSensitivity caseSensitivity,
                       Qt::SortOrder order,
                       QWidget *parent/* = Q_NULLPTR*/) :
    QDialog(parent),
    ui(new Ui::SortDialog),
    m_sortSectionType(sectionType),
    m_sortDirsType(dirsType),
    m_sortDotFirst(dotFirst),
    m_sortCaseSensitivity(caseSensitivity),
    m_sortOrder(order)
{
    ui->setupUi(this);

    if(sectionType == SectionType::LastModified)
    {
        ui->sortLastModifiedRadioButton->setChecked(true);
    }
    else if(sectionType == SectionType::FileSize)
    {
        ui->sortSizeRadioButton->setChecked(true);
    }
    else if(sectionType == SectionType::FileType)
    {
        ui->sortTypeRadioButton->setChecked(true);
    }
    else
    {
        ui->sortNameRadioButton->setChecked(true);
    }

    if(order == Qt::DescendingOrder)
    {
        ui->orderDescendingRadioButton->setChecked(true);
    }
    else
    {
        ui->orderAscendingRadioButton->setChecked(true);
    }

    if(dirsType == SortDirsType::First)
    {
        ui->dirFirstRadioButton->setChecked(true);
    }
    else if(dirsType == SortDirsType::Last)
    {
        ui->dirLastRadioButton->setChecked(true);
    }
    else
    {
        ui->dirNoSpecifyRadioButton->setChecked(true);
    }

    if(caseSensitivity == Qt::CaseInsensitive)
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

SectionType SortDialog::getSortSectionType()
{
    return m_sortSectionType;
}

SortDirsType SortDialog::getSortDirsType()
{
    return m_sortDirsType;
}

bool SortDialog::getSortDotFirst()
{
    return m_sortDotFirst;
}

Qt::CaseSensitivity SortDialog::getSortCaseSensitivity()
{
    return m_sortCaseSensitivity;
}

Qt::SortOrder SortDialog::getSortOrder()
{
    return m_sortOrder;
}

void SortDialog::accept()
{
    if(ui->sortLastModifiedRadioButton->isChecked())
    {
        m_sortSectionType = SectionType::LastModified;
    }
    else if(ui->sortSizeRadioButton->isChecked())
    {
        m_sortSectionType = SectionType::FileSize;
    }
    else if(ui->sortTypeRadioButton->isChecked())
    {
        m_sortSectionType = SectionType::FileType;
    }
    else
    {
        m_sortSectionType = SectionType::FileName;
    }

    if(ui->orderDescendingRadioButton->isChecked())
    {
        m_sortOrder = Qt::DescendingOrder;
    }
    else
    {
        m_sortOrder = Qt::AscendingOrder;
    }

    if(ui->dirFirstRadioButton->isChecked())
    {
        m_sortDirsType = SortDirsType::First;
    }
    else if(ui->dirLastRadioButton->isChecked())
    {
        m_sortDirsType = SortDirsType::Last;
    }
    else
    {
        m_sortDirsType = SortDirsType::NoSpecify;
    }

    if(ui->caseInsensitiveRadioButton->isChecked())
    {
        m_sortCaseSensitivity = Qt::CaseInsensitive;
    }
    else
    {
        m_sortCaseSensitivity = Qt::CaseSensitive;
    }

    QDialog::accept();
}

}           // namespace Farman
