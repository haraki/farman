#include <QDebug>
#include "sortdialog.h"
#include "ui_sortdialog.h"

namespace Farman
{

SortDialog::SortDialog(SectionType sectionType,
                       SectionType sectionType2nd,
                       SortDirsType dirsType,
                       bool dotFirst,
                       SortCaseSensitivity caseSensitivity,
                       SortOrderType order,
                       QWidget *parent/* = Q_NULLPTR*/) :
    QDialog(parent),
    ui(new Ui::SortDialog),
    m_sortSectionType(sectionType),
    m_sortSectionType2nd(sectionType2nd),
    m_sortDirsType(dirsType),
    m_sortDotFirst(dotFirst),
    m_sortCaseSensitivity(caseSensitivity),
    m_sortOrder(order)
{
    ui->setupUi(this);

    if(sectionType2nd == SectionType::LastModified)
    {
        ui->sort2ndLastModifiedRadioButton->setChecked(true);
    }
    else if(sectionType2nd == SectionType::FileSize)
    {
        ui->sort2ndSizeRadioButton->setChecked(true);
    }
    else if(sectionType2nd == SectionType::FileType)
    {
        ui->sort2ndTypeRadioButton->setChecked(true);
    }
    else if(sectionType2nd == SectionType::FileName)
    {
        ui->sort2ndNameRadioButton->setChecked(true);
    }
    else
    {
        ui->sort2ndNoSpecifyRadioButton->setChecked(true);
    }

    if(sectionType == SectionType::LastModified)
    {
        ui->sortLastModifiedRadioButton->setChecked(true);
        setSort2ndEnabled(true, true, true, false);
    }
    else if(sectionType == SectionType::FileSize)
    {
        ui->sortSizeRadioButton->setChecked(true);
        setSort2ndEnabled(true, true, false, true);
    }
    else if(sectionType == SectionType::FileType)
    {
        ui->sortTypeRadioButton->setChecked(true);
        setSort2ndEnabled(true, false, true, true);
    }
    else
    {
        ui->sortNameRadioButton->setChecked(true);
        setSort2ndEnabled(false, true, true, true);
    }

    if(order == SortOrderType::Descending)
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

    if(caseSensitivity == SortCaseSensitivity::Insensitive)
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

SectionType SortDialog::getSortSectionType2nd()
{
    return m_sortSectionType2nd;
}

SortDirsType SortDialog::getSortDirsType()
{
    return m_sortDirsType;
}

bool SortDialog::getSortDotFirst()
{
    return m_sortDotFirst;
}

SortCaseSensitivity SortDialog::getSortCaseSensitivity()
{
    return m_sortCaseSensitivity;
}

SortOrderType SortDialog::getSortOrder()
{
    return m_sortOrder;
}

void SortDialog::on_sortNameRadioButton_clicked()
{
    setSort2ndEnabled(false, true, true, true);
}

void SortDialog::on_sortTypeRadioButton_clicked()
{
    setSort2ndEnabled(true, false, true, true);
}

void SortDialog::on_sortSizeRadioButton_clicked()
{
    setSort2ndEnabled(true, true, false, true);
}

void SortDialog::on_sortLastModifiedRadioButton_clicked()
{
    setSort2ndEnabled(true, true, true, false);
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

    if(ui->sort2ndLastModifiedRadioButton->isChecked())
    {
        m_sortSectionType2nd = SectionType::LastModified;
    }
    else if(ui->sort2ndSizeRadioButton->isChecked())
    {
        m_sortSectionType2nd = SectionType::FileSize;
    }
    else if(ui->sort2ndTypeRadioButton->isChecked())
    {
        m_sortSectionType2nd = SectionType::FileType;
    }
    else if(ui->sort2ndNameRadioButton->isChecked())
    {
        m_sortSectionType2nd = SectionType::FileName;
    }
    else
    {
        m_sortSectionType2nd = SectionType::Unknown;
    }

    if(ui->orderDescendingRadioButton->isChecked())
    {
        m_sortOrder = SortOrderType::Descending;
    }
    else
    {
        m_sortOrder = SortOrderType::Ascending;
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
        m_sortCaseSensitivity = SortCaseSensitivity::Insensitive;
    }
    else
    {
        m_sortCaseSensitivity = SortCaseSensitivity::Sensitive;
    }

    QDialog::accept();
}

void SortDialog::setSort2ndEnabled(bool name, bool type, bool size, bool lastModified)
{
    if(!name && ui->sort2ndNameRadioButton->isChecked())
    {
        ui->sort2ndNoSpecifyRadioButton->setChecked(true);
    }
    ui->sort2ndNameRadioButton->setEnabled(name);

    if(!type && ui->sort2ndTypeRadioButton->isChecked())
    {
        ui->sort2ndNoSpecifyRadioButton->setChecked(true);
    }
    ui->sort2ndTypeRadioButton->setEnabled(type);

    if(!size && ui->sort2ndSizeRadioButton->isChecked())
    {
        ui->sort2ndNoSpecifyRadioButton->setChecked(true);
    }
    ui->sort2ndSizeRadioButton->setEnabled(size);

    if(!lastModified && ui->sort2ndLastModifiedRadioButton->isChecked())
    {
        ui->sort2ndNoSpecifyRadioButton->setChecked(true);
    }
    ui->sort2ndLastModifiedRadioButton->setEnabled(lastModified);
}

}           // namespace Farman
