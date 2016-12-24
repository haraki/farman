#include "folderviewstyleditemdelegate.h"

#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QDebug>
#include <QPainter>
#include <QApplication>

#include "folderview.h"

FolderViewStyledItemDelegate::FolderViewStyledItemDelegate(QObject *parent/* = Q_NULLPTR*/)
    : QStyledItemDelegate(parent)
{
}

void FolderViewStyledItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);

    FolderView* parent = dynamic_cast<FolderView*>(this->parent());
    if(parent != Q_NULLPTR)
    {
        if(parent->currentIndex().row() == index.row())
        {
            // カーソル位置をアンダーラインで表示
            const QWidget *widget = option.widget;
            painter->save();
            QPen pen((option.state & QStyle::State_Active) ? widget->palette().highlight() : widget->palette().dark(), 1);
            painter->setPen(pen);
            painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
            painter->restore();
        }
    }
}
