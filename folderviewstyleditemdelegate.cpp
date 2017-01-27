#include "folderviewstyleditemdelegate.h"

#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QDebug>
#include <QPainter>
#include <QApplication>

#include "folderview.h"

namespace Farman
{

FolderViewStyledItemDelegate::FolderViewStyledItemDelegate(QObject *parent/* = Q_NULLPTR*/)
    : QStyledItemDelegate(parent)
{
}

void FolderViewStyledItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    opt.state &= ~QStyle::State_Selected;       // FolderModel の TextColorRole・BackgroundRole の Brush を使用するため、ここでは Selected を無効にするｓ

    QStyledItemDelegate::paint(painter, opt, index);

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

}           // namespace Farman
