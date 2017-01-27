#ifndef FOLDERVIEWSTYLEDITEMDELEGATE_H
#define FOLDERVIEWSTYLEDITEMDELEGATE_H

#include <QStyledItemDelegate>

class QStyleOptionViewItem;
class QModelIndex;
class QPalette;

namespace Farman
{

class FolderViewStyledItemDelegate : public QStyledItemDelegate
{
public:
    explicit FolderViewStyledItemDelegate(QObject *parent = Q_NULLPTR);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
};

}           // namespace Farman

#endif // FOLDERVIEWSTYLEDITEMDELEGATE_H
