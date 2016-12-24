#include "folderview.h"
#include "folderviewstyleditemdelegate.h"

FolderView::FolderView(QWidget *parent/* = Q_NULLPTR*/)
    : QTableView(parent)
{
    setItemDelegate(new FolderViewStyledItemDelegate(this));
}

FolderView::~FolderView()
{

}
