﻿#include <QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QItemSelectionModel>
#include <QHeaderView>
#include <QMimeData>
#include <QMenu>
#include "folderview.h"
#include "folderviewstyleditemdelegate.h"
#include "foldermodel.h"
#include "file.h"
#include "settings.h"
#include "types.h"

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

void FolderView::setModel(FolderModel *model)
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

void FolderView::selectCurrent()
{
    FolderModel* folderModel = dynamic_cast<FolderModel*>(model());
    Q_ASSERT(folderModel);

    const QModelIndex index = currentIndex();
    if(folderModel->fileName(index) != "..")
    {
        folderModel->setSelect(index.row(), QItemSelectionModel::Toggle, index.parent());
    }
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

        // keyboradSearch() 内で文字を連結させず、必ず1文字目として検索させるようにするため、一時的にキー入力のインターバル値を0にする
        int backupInterval = QApplication::keyboardInputInterval();
        QApplication::setKeyboardInputInterval(0);

        keyboardSearch(text);

        QApplication::setKeyboardInputInterval(backupInterval);

        e->accept();

        return;
    }

    e->ignore();
}

void FolderView::mousePressEvent(QMouseEvent *e)
{
    qDebug() << "FolderView::mousePressEvent()";

    QTableView::mousePressEvent(e);

    if(e->buttons() & Qt::LeftButton && indexAt(e->pos()).isValid())
    {
        selectCurrent();
    }
}

void FolderView::dragEnterEvent(QDragEnterEvent *e)
{
    qDebug() << "FolderView::dragEnterEvent()";

    FolderView* source = dynamic_cast<FolderView*>(e->source());
    if(this != source)
    {
        e->accept();
    }
}

void FolderView::dragMoveEvent(QDragMoveEvent *e)
{
    qDebug() << "FolderView::dragMoveEvent()";

    FolderView* source = dynamic_cast<FolderView*>(e->source());
    if(this != source)
    {
        e->accept();
    }
}

void FolderView::dropEvent(QDropEvent *e)
{
    qDebug() << "FolderView::dropEvent()";

    FolderView* source = dynamic_cast<FolderView*>(e->source());
    if(this != source)
    {
        FolderModel* folderModel = dynamic_cast<FolderModel*>(model());
        Q_ASSERT(folderModel);

        QString dstDirPath = folderModel->filePath(rootIndex());
        QStringList srcPaths;
        for(QUrl url : e->mimeData()->urls())
        {
            QString srcPath = QFileInfo(url.toLocalFile()).absoluteFilePath();
            if(!srcPath.isNull() && !srcPath.isEmpty())
            {
                // 末尾が'/'だとコピーできない
                while(srcPath.endsWith('/'))
                {
                    srcPath.chop(1);
                }
                srcPaths.push_back(srcPath);
            }
        }

        qDebug() << "srcPaths : " << srcPaths;
        qDebug() << "dstDirPath : " << dstDirPath;

        DragAndDropBehaviorType behaviorType = Settings::getInstance()->getDragAndDropBehaviorType();
        if(behaviorType == DragAndDropBehaviorType::Select)
        {
            // コピー or 移動選択用ポップアップメニュー表示
            QMenu menu(this);
            QAction *actionCopy = menu.addAction(tr("Copy"));
            QAction *actionMove = menu.addAction(tr("Move"));
            menu.addSeparator();
            menu.addAction(tr("Cancel"));

            QAction *selected = menu.exec(mapToGlobal(e->pos()));
            if(selected == actionCopy)
            {
                behaviorType = DragAndDropBehaviorType::Copy;
            }
            else if(selected == actionMove)
            {
                behaviorType = DragAndDropBehaviorType::Move;
            }
        }

        if(behaviorType == DragAndDropBehaviorType::Copy)
        {
            File::getInstance()->copyFile(srcPaths, dstDirPath);
        }
        else if(behaviorType == DragAndDropBehaviorType::Move)
        {
            File::getInstance()->moveFile(srcPaths, dstDirPath);
        }

        e->accept();
    }
}

}           // namespace Farman
