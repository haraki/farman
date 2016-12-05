#include "folderview.h"
#include "folderviewstyleditemdelegate.h"

FolderView::FolderView(QWidget *parent)
    : QTableView(parent)
{

    setItemDelegate(new FolderViewStyledItemDelegate());
}

FolderView::~FolderView()
{

}
