#include "folderpanelbase.h"
#include "folderform.h"

FolderPanelBase::FolderPanelBase(QWidget *parent/* = Q_NULLPTR*/)
    : QWidget(parent)
{

}

FolderPanelBase::~FolderPanelBase()
{

}

QString FolderPanelBase::getPath()
{
    FolderForm* folderForm = getFolderForm();
    if(folderForm == Q_NULLPTR)
    {
        return QString("");
    }

    return folderForm->getPath();
}

QDir::Filters FolderPanelBase::getFilterFlags()
{
    FolderForm* folderForm = getFolderForm();
    if(folderForm == Q_NULLPTR)
    {
        return QDir::AllEntries | QDir::NoSymLinks | QDir::AccessMask | QDir::NoDot;
    }

    return folderForm->getFilterFlags();
}

QDir::SortFlags FolderPanelBase::getSortFlags()
{
    FolderForm* folderForm = getFolderForm();
    if(folderForm == Q_NULLPTR)
    {
        return QDir::DirsFirst | QDir::IgnoreCase | QDir::Name;
    }

    return folderForm->getSortFlags();
}
