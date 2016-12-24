#include <QDebug>
#include <QKeyEvent>
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

QItemSelectionModel::SelectionFlags FolderView::selectionCommand(const QModelIndex& index, const QEvent* e/* = Q_NULLPTR*/) const
{
    QItemSelectionModel::SelectionFlags ret = QTableView::selectionCommand(index, e);
    const QKeyEvent* keyEv = dynamic_cast<const QKeyEvent*>(e);

    if(keyEv == Q_NULLPTR)
    {
        ret &= ~QItemSelectionModel::SelectionFlag::ClearAndSelect;
    }
    else
    {
        Qt::Key key = static_cast<Qt::Key>(keyEv->key());

        qDebug() << key;

        if(((key & Qt::Key_Shift) != Qt::Key_Shift) && ((key & Qt::Key_Control) != Qt::Key_Control))
        {
            ret &= ~QItemSelectionModel::SelectionFlag::ClearAndSelect;
        }
    }

    return ret;
}

void FolderView::refresh(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    emit dataChanged(topLeft, bottomRight);
}
