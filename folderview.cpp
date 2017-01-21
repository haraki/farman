#include <QDebug>
#include <QKeyEvent>
#include <QItemSelectionModel>
#include "folderview.h"
#include "folderviewstyleditemdelegate.h"
#include "foldermodel.h"

FolderView::FolderView(QWidget *parent/* = Q_NULLPTR*/)
    : QTableView(parent)
{
    setItemDelegate(new FolderViewStyledItemDelegate(this));
}

FolderView::~FolderView()
{

}

QItemSelectionModel::SelectionFlags FolderView::selectionCommand(const QModelIndex& index, const QEvent* e/* = Q_NULLPTR*/) const
{
    QItemSelectionModel::SelectionFlags ret = QTableView::selectionCommand(index, e);

    // ここでは選択処理は行わない
    ret &= ~(QItemSelectionModel::SelectionFlag::ClearAndSelect | QItemSelectionModel::SelectionFlag::Toggle);

    return ret;
}

void FolderView::refresh(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    emit dataChanged(topLeft, bottomRight);
}
