#include "folderviewstyleditemdelegate.h"

#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QDebug>
#include <QPainter>
#include <QApplication>
#include "folderview.h"
#include "settings.h"

namespace Farman
{

FolderViewStyledItemDelegate::FolderViewStyledItemDelegate(QObject *parent/* = Q_NULLPTR*/)
    : QStyledItemDelegate(parent)
{
}

void FolderViewStyledItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    opt.state &= ~QStyle::State_Selected;       // FolderModel の TextColorRole・BackgroundRole の Brush を使用するため、ここでは Selected を無効にする
    opt.state &= ~QStyle::State_HasFocus;       // カーソル位置の枠を表示しないようにするため、ここでは Has_Focus を無効にする(Windows用)

    QStyledItemDelegate::paint(painter, opt, index);

    FolderView* parent = dynamic_cast<FolderView*>(this->parent());
    if(parent != Q_NULLPTR)
    {
        if(parent->currentIndex().row() == index.row())
        {
            // カーソル位置をアンダーラインで表示
            painter->save();
            QColor color = (option.state & QStyle::State_Active) ?
                               Settings::getInstance()->getColorSetting("folderView_cursor") : Settings::getInstance()->getColorSetting("folderView_cursor_inactive");
            int width = Settings::getInstance()->getCursorWidth();
            QPen pen(color, static_cast<qreal>(width));
            painter->setPen(pen);
            painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
            painter->restore();
        }
    }
}

}           // namespace Farman
