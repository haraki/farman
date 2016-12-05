#ifndef FOLDERVIEWSTYLEDITEMDELEGATE_H
#define FOLDERVIEWSTYLEDITEMDELEGATE_H

#include <QStyledItemDelegate>

class QStyleOptionViewItem;
class QModelIndex;
class QPalette;

class FolderViewStyledItemDelegate : public QStyledItemDelegate
{
public:
    FolderViewStyledItemDelegate(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
//    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // FOLDERVIEWSTYLEDITEMDELEGATE_H
