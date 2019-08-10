#include "folderviewstyleditemdelegate.h"

#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QDebug>
#include <QPainter>
#include <QApplication>
#include "folderview.h"
#include "default_settings.h"

namespace Farman
{

FolderViewStyledItemDelegate::FolderViewStyledItemDelegate(QObject *parent/* = Q_NULLPTR*/)
    : QStyledItemDelegate(parent)
    , m_cursorWidth(DEFAULT_CURSOR_WIDTH)
    , m_activeColor(DEFAULT_COLOR_SETTINGS["folderView_cursor"])
    , m_inactiveColor(DEFAULT_COLOR_SETTINGS["folderView_cursor_inactive"])
{
    makePen();
}

void FolderViewStyledItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    opt.state &= ~QStyle::State_Selected;       // FolderModel の TextColorRole・BackgroundRole の Brush を使用するため、ここでは Selected を無効にする
    opt.state &= ~QStyle::State_HasFocus;       // カーソル位置の枠を表示しないようにするため、ここでは Has_Focus を無効にする(Windows用)

    QStyledItemDelegate::paint(painter, opt, index);

    FolderView* parent = qobject_cast<FolderView*>(this->parent());
    if(parent != Q_NULLPTR)
    {
        if(parent->currentIndex().row() == index.row())
        {
            // カーソル位置をアンダーラインで表示
            painter->save();
            painter->setPen((option.state & QStyle::State_Active) ? m_activeCursorPen : m_inactiveCursorPen);
            painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
            painter->restore();
        }
    }
}

void FolderViewStyledItemDelegate::setCursorAppearance(int width, const QColor& activeColor, const QColor& inactiveColor)
{
    m_cursorWidth = width;
    m_activeColor = activeColor;
    m_inactiveColor = inactiveColor;

    makePen();
}

void FolderViewStyledItemDelegate::makePen()
{
    m_activeCursorPen = QPen(m_activeColor, static_cast<qreal>(m_cursorWidth));
    m_inactiveCursorPen = QPen(m_inactiveColor, static_cast<qreal>(m_cursorWidth));
}

}           // namespace Farman
