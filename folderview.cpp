#include <QDebug>
#include <QKeyEvent>
#include <QItemSelectionModel>
#include <QHeaderView>
#include "folderview.h"
#include "folderviewstyleditemdelegate.h"
#include "foldermodel.h"

namespace Farman
{

FolderView::FolderView(QWidget *parent/* = Q_NULLPTR*/)
    : QTableView(parent)
{
    setItemDelegate(new FolderViewStyledItemDelegate(this));
}

FolderView::~FolderView()
{

}

void FolderView::setModel(QAbstractItemModel *model)
{
    QTableView::setModel(model);

    horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
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

void FolderView::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_Up:
    case Qt::Key_Down:
    case Qt::Key_PageUp:
    case Qt::Key_PageDown:
        QTableView::keyPressEvent(e);
        return;
    default:
        break;
    }

    if(e->modifiers() & Qt::ShiftModifier && !e->text().isEmpty())
    {
        QString text = e->text();

        if(e->key() == Qt::Key_Greater)
        {
            // SHIFT + '.'キーを押すと '>' となってしまうため、強制的に '.' にする
            text = ".";
        }

        keyboardSearch(text);

        e->accept();

        return;
    }

    e->ignore();
}

}           // namespace Farman
