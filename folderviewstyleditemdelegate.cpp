#include "folderviewstyleditemdelegate.h"

#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QDebug>
#include <QPainter>
#include <QApplication>

FolderViewStyledItemDelegate::FolderViewStyledItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

void FolderViewStyledItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_ASSERT(index.isValid());

//    qDebug() << "FolderViewStyledItemDelegate::paint(" << painter << "," << hex << (unsigned long)option.state << "," << index << ")";
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    opt.state &= ~QStyle::State_Selected;           // カーソル位置の背景・文字を通常と同じ色にする

    const QWidget *widget = option.widget;
    QStyle *style = widget ? widget->style() : QApplication::style();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

    if(option.state & QStyle::State_Selected)
    {
        // カーソル位置をアンダーラインで表示
        painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
    }
}
