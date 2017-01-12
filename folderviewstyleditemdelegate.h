#ifndef FOLDERVIEWSTYLEDITEMDELEGATE_H
#define FOLDERVIEWSTYLEDITEMDELEGATE_H

#include <QStyledItemDelegate>

class QStyleOptionViewItem;
class QModelIndex;
class QPalette;

class FolderViewStyledItemDelegate : public QStyledItemDelegate
{
public:
    explicit FolderViewStyledItemDelegate(QObject *parent = Q_NULLPTR);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif // FOLDERVIEWSTYLEDITEMDELEGATE_H
