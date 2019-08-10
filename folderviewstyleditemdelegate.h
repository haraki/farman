#ifndef FOLDERVIEWSTYLEDITEMDELEGATE_H
#define FOLDERVIEWSTYLEDITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QColor>
#include <QPen>

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

    void setCursorAppearance(int width, const QColor& activeColor, const QColor& inactiveColor);

private:
    void makePen();

    int m_cursorWidth;
    QColor m_activeColor;
    QColor m_inactiveColor;
    QPen m_activeCursorPen;
    QPen m_inactiveCursorPen;
};

}           // namespace Farman

#endif // FOLDERVIEWSTYLEDITEMDELEGATE_H
